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

TCPServer::TCPServer
(
	uint16_t	_port
)
{
	INFO(this, "TCP Server[%d] created", _port);
	port_ = _port;
}

TCPServer::~TCPServer()
{
	for(auto it = session_list_.begin(); it != session_list_.end(); it++)
	{
		delete *it;
	}
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

#if 0
 	if( fcntl(socket_, F_SETFL, O_NONBLOCK) == -1 )
	{
		ret_value = RET_VALUE_ERROR;
       	ERROR(this, ret_value, "Failed to set nonblocking socket.\n");
       	return;
	}
#endif				 
	server.sin_family 		= AF_INET;
	server.sin_addr.s_addr	= INADDR_ANY;
	server.sin_port 		= htons(port_);

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
	int		client_len;	
	struct sockaddr_in	client;

	client_len = sizeof(struct sockaddr_in);	

	INFO(this, "Waiting for connection");
	client_socket = accept(socket_, (struct sockaddr *)&client, (socklen_t *)&client_len);
	if (client_socket > 0)
	{
		INFO(this, "Accept new connection[ %s:%d ]", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		TCPSession*	session = new TCPSession(client_socket, (struct sockaddr *)&client);
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
				session_list_.push_back(session);
			}
		}
	}
}

void	TCPServer::PostProcess()
{
	for(auto it = session_list_.begin(); it != session_list_.end() ; it++)
	{
		delete *it;
	}
}


