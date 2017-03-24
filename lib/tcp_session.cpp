#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp_session.h"
#include "tcp_server.h"
#include "trace.h"


TCPSession::TCPSession
(
	TCPServer*	_server,
	int	_socket,
	struct sockaddr_in *_addr_info,
	uint32_t	_timeout
)
{
	ASSERT(addr_info != NULL);

	RetValue	ret_value = RET_VALUE_OK;

	server_	= _server;
	socket_ = _socket;
	information_.addr_info = *_addr_info;
	information_.timeout = _timeout;
	information_.start_time.GetCurrentTime();

	receive_buffer_len_ = 4096;
	receive_buffer_ = new uint8_t[receive_buffer_len_];

	send_buffer_len_ = 4096;
	send_buffer_ = new uint8_t[send_buffer_len_];
}

TCPSession::~TCPSession()
{
	delete receive_buffer_;
	delete send_buffer_;
}	

RetValue	TCPSession::Disconnect()
{
	if (socket_ != 0)
	{
		shutdown(socket_, 0);
		socket_ = 0;
		if (server_ != NULL)
		{
			server_->SessionDisconnected(information_.addr_info.sin_port);
		}
	}

	return	RET_VALUE_OK;
}

void	TCPSession::Process()
{
	if (socket_)
	{
		receive_len_ = recv(socket_, receive_buffer_, receive_buffer_len_, MSG_DONTWAIT);
		if (receive_len_ < 0)
		{
			if (errno != EAGAIN)
			{
				Disconnect();
				ERROR(this, RET_VALUE_SOCKET_ERROR, "The socket has terminated abnormally.");
			}
		}
		else if (receive_len_ == 0)
		{
			Disconnect();
			ERROR(this, RET_VALUE_SOCKET_ERROR, "The socket has terminated.");
		}
		else
		{
			server_->PacketReceived(ntohs(information_.addr_info.sin_port), receive_buffer_, receive_len_);
		}
	}
}

void	TCPSession::PostProcess()
{
	Disconnect();
}

RetValue	TCPSession::Send
(
	void*		data, 
	uint32_t 	len
)
{
	ASSERT(data != NULL);

	RetValue	ret_value = RET_VALUE_OK;
	uint32_t	send_len;

	send_len = send(socket_, data, len, MSG_DONTWAIT);
	if (len != send_len)
	{
		ret_value = RET_VALUE_SOCKET_SEND_FAILED;
		ERROR(this, ret_value, "The socket failed to send data.");
	}

	return	ret_value;
}

const
TCPSession::Information&	TCPSession::GetInformation()
{
	return	information_;
}
