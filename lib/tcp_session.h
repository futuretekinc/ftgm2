#ifndef	TCP_SESSION_H_
#define	TCP_SESSION_H_

#include <sys/socket.h> 
#include <arpa/inet.h>
#include <list>
#include "message_process.h"

class	TCPServer;

class	TCPSession : public MessageProcess
{
public:
	TCPSession(TCPServer *_server, int	_socket, struct sockaddr_in *_addr_info, uint32_t _timeout);
	~TCPSession();

	RetValue	Disconnect();
	RetValue	Send(void *data, uint32_t len);

protected:
	void		PreProcess();
	void		Process();
	void		PostProcess();

	TCPServer	*server_;
	int			socket_;
	struct sockaddr_in addr_info_;
	uint32_t	timeout_;

	uint8_t	*	receive_buffer_;
	uint32_t	receive_buffer_len_;
	uint32_t	receive_len_;
	uint8_t *	send_buffer_;	
	uint32_t	send_buffer_len_;
	uint32_t	send_len_;
};

#endif
