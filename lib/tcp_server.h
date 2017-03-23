#ifndef	TCP_SERVER_H_
#define	TCP_SERVER_H_

#include <sys/socket.h> 
#include <arpa/inet.h>
#include <list>
#include "message_process.h"

class	TCPSession;

class	TCPServer : public MessageProcess
{
public:
	TCPServer(uint16_t port = 8888);
	~TCPServer();

protected:

	void		PreProcess();
	void		Process();
	void		PostProcess();

	int						socket_;	
	uint16_t				port_;
	std::list<TCPSession*>	session_list_;
};

#endif
