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
	addr_info_ = *_addr_info;
	timeout_ = _timeout;

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
			server_->SessionDisconnected(addr_info_.sin_port);
		}
	}

	return	RET_VALUE_OK;
}

void	TCPSession::Process()
{
	if (socket_)
	{
		receive_len_ = recv(socket_, receive_buffer_, receive_buffer_len_, 0);
		if (receive_len_ < 0)
		{
			Disconnect();
			ERROR(this, RET_VALUE_SOCKET_ERROR, "The socket has terminated abnormally.");
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
