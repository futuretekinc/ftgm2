#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp_server.h"
#include "tcp_session.h"
#include "trace.h"
#include "message.h"


TCPServer::MessagePacketReceived::MessagePacketReceived
(
	uint16_t _port, 
	void* _data, 
	uint32_t _length
)
: Message(TYPE_PACKET_RECEIVED), port(_port), data(NULL), length(0)
{
	try
	{
		if (_length != 0)
		{
			data = new uint8_t[_length];

			memcpy(data, _data, _length);
		}

		length = _length;
	}
	catch(std::bad_alloc &e)
	{
		ERROR(NULL, RET_VALUE_NOT_ENOUGH_MEMORY, "Failed to create message!");	
	}
}

TCPServer::MessagePacketReceived::~MessagePacketReceived()
{
	if (data != NULL)
	{
		delete data;	
	}
}

TCPServer::Properties::Properties()
: port(8888), max_session_count(10), timeout(60000)
{
}

RetValue	TCPServer::Properties::Set
(
	const JSONNode& _node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (strcasecmp(_node.name().c_str(), "port") == 0)
	{
		port = _node.as_int();
	}
	else if (strcasecmp(_node.name().c_str(), "max_session") == 0)
	{
		max_session_count = _node.as_int();
	}
	else if (strcasecmp(_node.name().c_str(), "timeout") == 0)
	{
		timeout = _node.as_int();
	}
	else if(_node.type() == JSON_NODE)
	{
		for(size_t i = 0 ; i < _node.size() ; i++)
		{
			Set(_node[i]);	
		}
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FIELD;
	}

	return	ret_value;
}

RetValue	TCPServer::Properties::Set
(
	const Properties* _properties
)
{
	ASSERT(_properties != NULL);

	port = _properties->port;
	max_session_count = _properties->max_session_count;
	timeout = _properties->timeout;

	return	RET_VALUE_OK;
}

/////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////
TCPServer::TCPServer
(
	void*	_data
)
{
	data_ = _data;	
}

TCPServer::~TCPServer()
{
	session_map_locker_.Lock();
	for(auto it = session_map_.begin(); it != session_map_.end(); it++)
	{
		delete it->second;
	}
	session_map_locker_.Unlock();
}

RetValue	TCPServer::Set
(
	Properties*	_properties
)
{
	ASSERT(_properties != NULL);
	
	properties_.Set(_properties);

	return	RET_VALUE_OK;
}

void*	TCPServer::GetData()
{
	return	data_;
}

void	TCPServer::PreProcess()
{
	RetValue	ret_value;

	struct sockaddr_in	server;

	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ == -1)
	{
		ret_value = RET_VALUE_ERROR;
		ERROR(this, ret_value, "Failed to create socket.\n");
		return;
	}

 	if( fcntl(socket_, F_SETFL, O_NONBLOCK) == -1 )
	{
		ret_value = RET_VALUE_ERROR;
       	ERROR(this, ret_value, "Failed to set nonblocking socket.\n");
       	return;
	}

	server.sin_family 		= AF_INET;
	server.sin_addr.s_addr	= INADDR_ANY;
	server.sin_port 		= htons(properties_.port);

	ret_value = bind( socket_, (struct sockaddr *)&server, sizeof(server));
	if (ret_value < 0)
	{
		ret_value = RET_VALUE_SOCKET_BIND_FAILED;
		ERROR(this, ret_value, "Failed to socket binding");
		return;
	}

	listen(socket_, 3);
}

void	TCPServer::Process()
{
	RetValue	ret_value;
	time_t		current_time;


	int		client_socket;
	const int		client_len = sizeof(struct sockaddr_in);	
	struct sockaddr_in	client;

	if (session_map_.size() < properties_.max_session_count)
	{
		client_socket = accept(socket_, (struct sockaddr *)&client, (socklen_t *)&client_len);
		if (client_socket > 0)
		{
			TCPSession*	session = new TCPSession(this, client_socket, &client, properties_.timeout);
			if (session == NULL)
			{
				ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;
				ERROR(this, ret_value, "Failed to create tcp session.");
			}
			else
			{
				ret_value = session->Start();
				if (ret_value != RET_VALUE_OK)
				{
					ERROR(this, ret_value, "Failed to start session!");
					delete session;
				}
				else
				{
					session_map_locker_.Lock();

					session_map_[ntohs(client.sin_port)] = session;

					session_map_locker_.Unlock();

					INFO(this, "New session created[%s:%d]", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

				}
			}
		}
	}
}

void	TCPServer::PostProcess()
{
	session_map_locker_.Lock();

	for(auto it = session_map_.begin(); it != session_map_.end() ; it++)
	{
		delete it->second;
	}

	session_map_locker_.Unlock();
}


RetValue	TCPServer::SessionDisconnected
(
	uint16_t port
)
{
	RetValue	ret_value = RET_VALUE_OK;

	try
	{
		Message	*message = new MessageSessionDisconnected(port);
		Post(message);
	}
	catch(std::bad_alloc &e)
	{
		ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;	
	}

	return	ret_value;
}

RetValue	TCPServer::PacketReceived
(
	uint16_t _port, 
	void* 	_data, 
	uint32_t _length
)
{
	RetValue	ret_value = RET_VALUE_OK;

	try
	{
		Message	*message = new MessagePacketReceived(_port, _data, _length);
		Post(message);
	}
	catch(std::bad_alloc &e)
	{
		ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;	
	}

	return	ret_value;
}

uint32_t	TCPServer::GetSessionCount()
{
	return	session_map_.size();
}

RetValue	TCPServer::GetSessionInformationList
(
	std::list<TCPSession::Information>& _information_list
)
{
	session_map_locker_.Lock();

	for(auto it = session_map_.begin() ; it != session_map_.end() ; it++)
	{
		_information_list.push_back(it->second->GetInformation());
	}

	session_map_locker_.Unlock();

	return	RET_VALUE_OK;
}

void	TCPServer::OnMessage
(
	Message *_base_message
)
{
	switch(_base_message->type)
	{
	case	Message::TYPE_SESSION_DISCONNECTED:
		{
			MessageSessionDisconnected* message = (MessageSessionDisconnected*)_base_message;

			if (message != NULL)
			{
				session_map_locker_.Lock();

				TCPSession *session = session_map_[message->port];		
				session_map_.erase(message->port);

				session_map_locker_.Unlock();
				
				session->Stop();

				delete session;

			}
		}
		break;

	case	Message::TYPE_PACKET_RECEIVED:
		{
			OnPacketReceived((MessagePacketReceived *)_base_message);
		}
		break;


	default:
		{
			MessageProcess::OnMessage(_base_message);	
		}
	}
}

void	TCPServer::OnPacketReceived
(
	MessagePacketReceived* message
)
{
	ASSERT(message != NULL);
	RetValue	ret_value = RET_VALUE_OK;
	TCPSession *session;
	auto it = session_map_.find(message->port);

	if (it != session_map_.end())
	{
		session = it->second;

		ret_value = session->Send(message->data, message->length);
		if (ret_value != RET_VALUE_OK)
		{
			ERROR(this, ret_value, "Failed to send data from session[%d]", message->port);	
		}
	}
	else
	{
		ret_value = RET_VALUE_SESSION_NOT_FOUND;
		INFO(this, "session_map_.size() = %d", session_map_.size());
		ERROR(this, ret_value , "Failed to get session[%d]", message->port);
		return;	
	}
}
