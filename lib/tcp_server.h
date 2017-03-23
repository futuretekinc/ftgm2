#ifndef	TCP_SERVER_H_
#define	TCP_SERVER_H_

#include <sys/socket.h> 
#include <arpa/inet.h>
#include <map>
#include <libjson/libjson.h>
#include "message_process.h"
#include "message.h"

class	TCPSession;

class	TCPServer : public MessageProcess
{
public:
	class	MessageSessionDisconnected : public ::Message
	{
	public:
		uint16_t	port;
		MessageSessionDisconnected(uint16_t _port) : Message(TYPE_SESSION_DISCONNECTED), port(_port) {};
	};


	struct	Properties
	{
		uint16_t	port;
		uint32_t	max_session_count;
		uint32_t	timeout;

		Properties();

		RetValue	Set(const JSONNode& _node);
		RetValue	Set(const Properties* _properties);
	};

	TCPServer(uint16_t _port = 8888);
	~TCPServer();

	RetValue	Set(Properties* _properties);

	RetValue	SessionDisconnected(uint16_t port);

	void		OnMessage(Message *_message);
protected:

	void		PreProcess();
	void		Process();
	void		PostProcess();

	Properties	properties_;

	int						socket_;	
	std::map<uint16_t, TCPSession*>	session_map_;
};

#endif
