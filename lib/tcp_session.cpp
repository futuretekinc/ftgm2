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
#include "trace.h"


TCPSession::TCPSession
(
	int	_socket,
	struct sockaddr *_addr_info
)
{
	ASSERT(addr_info != NULL);

	RetValue	ret_value = RET_VALUE_OK;

	receive_buffer_len_ = 4096;
	receive_buffer_ = new uint8_t[receive_buffer_len_];

	send_buffer_len_ = 4096;
	send_buffer_ = new uint8_t[send_buffer_len_];

	socket_ = _socket;
	addr_info_ = *_addr_info;
}

TCPSession::~TCPSession()
{
	delete receive_buffer_;
	delete send_buffer_;
}	

void	TCPSession::Process()
{
	receive_len_ = recv(socket_, receive_buffer_, receive_buffer_len_, 0);
	if (receive_len_ < 0)
	{
		stop_ = true;
		ERROR(this, RET_VALUE_SOCKET_ERROR, "The socket has terminated abnormally.");
	}
	else if (receive_len_ == 0)
	{
		INFO(this, "The socket has been terminated.");
	}
	else
	{
		send_len_ = send(socket_, send_buffer_, send_len_, MSG_DONTWAIT);
		if (send_len_ < 0)
		{
			stop_ = true;
			ERROR(this, RET_VALUE_SOCKET_ERROR, "The socket has terminated abnormally.");
		}
		else if (send_len_ == 0)
		{
			INFO(this, "The socket has been terminated.");
		}
	}
}

