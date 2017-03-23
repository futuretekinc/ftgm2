#ifndef	TCP_CLIENT_H_
#define	TCP_CLIENT_H_

#include <sys/socket.h> 
#include <arpa/inet.h>
#include <list>
#include "message_process.h"
#include "locker.h"

class	TCPSession;

class	TCPClient : public MessageProcess
{
public:
	struct	Frame
	{
		uint32_t	len;
		uint8_t*	data;

		Frame(uint8_t *_data, uint32_t _len);
		~Frame();
	};	

	TCPClient(const std::string& _server_ip, uint16_t _server_port = 8888);
	~TCPClient();

	RetValue	Connect();
	RetValue	Disconnect();
	RetValue	Send(const void*	_frame, uint32_t	_frame_len);
	uint32_t	Receive(void*	_frame, uint32_t	_frame_len);
	uint32_t	GetReceivedPacketCount();
protected:

	void		Process();
	void		PostProcess();

	Locker					locker_;
	int						socket_;	
	std::string				server_ip_;
	uint16_t				server_port_;
	std::list<Frame*>		receive_packet_list_;
};

#endif
