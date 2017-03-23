#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp_server.h"
#include "tcp_client.h"
#include "tcp_session.h"
#include "trace.h"

using namespace std;

TCPClient::Frame::Frame
(
	uint8_t *_data, 
	uint32_t _len
)
:	data(NULL), len(_len)
{
	if (len != 0)
	{
		data = new uint8_t[len];
		memcpy(data, _data, len);
	}
}

TCPClient::Frame::~Frame()
{
	if (data != NULL)
	{
		delete data;	
	}
}

TCPClient::TCPClient
(
	const string&	_server_ip,
	uint16_t		_server_port
)
{
	INFO(this, "TCP Client[%d] created", _server_port);
	server_ip_ 	= _server_ip;
	server_port_= _server_port;
}

TCPClient::~TCPClient()
{
	locker_.Lock();

	while(receive_packet_list_.size())
	{
		Frame* frame = receive_packet_list_.front()	;

		delete frame;

		receive_packet_list_.pop_front();
	}

	locker_.Unlock();

}

RetValue	TCPClient::Connect()
{
	RetValue	ret_value;

	int		client_socket;
	
	struct sockaddr_in	server;

	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1)
	{
		ret_value = RET_VALUE_ERROR;
		ERROR(this, ret_value, "Failed to create socket.\n");
		return	ret_value;
	}

#if 0
 	if( fcntl(socket_, F_SETFL, O_NONBLOCK) == -1 )
	{
		ret_value = RET_VALUE_ERROR;
       	ERROR(this, ret_value, "Failed to set nonblocking socket.\n");
       	return;
	}
#endif			 

	server.sin_family 		= AF_INET;
	server.sin_addr.s_addr	= inet_addr(server_ip_.c_str());
	server.sin_port 		= htons(server_port_);

	ret_value = connect( client_socket, (struct sockaddr *)&server, sizeof(server));
	if (ret_value < 0)
	{
		close(client_socket);
		ret_value = RET_VALUE_SOCKET_CONNECTION_FAILED;
		ERROR(this, ret_value, "Failed to connect to %s:%d", server_ip_.c_str(), server_port_);
		return ret_value;
	}

	socket_ = client_socket;

	return	RET_VALUE_OK;
}

RetValue	TCPClient::Disconnect()
{
	if (socket_ > 0)
	{
		shutdown(socket_, 0);
		socket_ = 0;
	}

	return	RET_VALUE_OK;
}

RetValue	TCPClient::Send
(
	const void*	_frame,
	uint32_t	_frame_len
)
{
	RetValue	ret_value = RET_VALUE_OK;
	uint32_t	send_len;

	send_len = send(socket_, _frame, _frame_len, MSG_DONTWAIT);
	if (send_len != _frame_len)
	{
		ret_value = RET_VALUE_SEND_FAILED;
		ERROR(this, ret_value, "Failed to send frame!");
		return	ret_value;	
	}

	return	ret_value;
}

uint32_t	TCPClient::Receive
(
	void*	_frame, 
	uint32_t	_frame_len
)
{
	uint32_t	received_len = 0;

	locker_.Lock();

	if (receive_packet_list_.size() != 0)
	{
		Frame* frame = receive_packet_list_.front()	;

		if (frame->len < _frame_len)
		{
			memcpy(_frame, frame->data, frame->len);	
			received_len = frame->len;

			delete frame;

			receive_packet_list_.pop_front();
		}
	}

	locker_.Unlock();

	return	received_len;
}

uint32_t	TCPClient::GetReceivedPacketCount()
{
	return	receive_packet_list_.size();
}

void	TCPClient::Process()
{
	RetValue	ret_value;

	uint8_t		buffer[4096];
	uint32_t	receive_len;

	if (socket_ > 0)
	{
		receive_len = recv(socket_, buffer, sizeof(buffer), MSG_DONTWAIT);
		if (receive_len < 0)
		{
			stop_ = true;
			ERROR(this, RET_VALUE_SOCKET_ERROR, "The socket has terminated abnormally.");
		}
		else if (receive_len > 0)
		{
			INFO(this, "Packet received!");
			locker_.Lock();
			
			if (receive_packet_list_.size() < 10)
			{
				Frame	*frame = new Frame(buffer, receive_len);

				receive_packet_list_.push_back(frame);
			}
			else
			{
				INFO(this, "Receive packet buffer full!");
			}
			locker_.Unlock();
		}
	}
}

void	TCPClient::PostProcess()
{
	for(auto it = receive_packet_list_.begin(); it != receive_packet_list_.end() ; it++)
	{
		delete *it;
	}
}


