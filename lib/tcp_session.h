#ifndef	TCP_SESSION_H_
#define	TCP_SESSION_H_

#include <sys/socket.h> 
#include <arpa/inet.h>
#include <list>
#include "message_process.h"

class	TCPSession : public MessageProcess
{
public:
	TCPSession(int	_socket, struct sockaddr *_addr_info);
	~TCPSession();


protected:
	void		Process();

	int			socket_;
	struct sockaddr addr_info_;

	uint8_t	*	receive_buffer_;
	uint32_t	receive_buffer_len_;
	uint32_t	receive_len_;
	uint8_t *	send_buffer_;	
	uint32_t	send_buffer_len_;
	uint32_t	send_len_;
};

#endif
