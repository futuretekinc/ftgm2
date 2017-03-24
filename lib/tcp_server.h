#ifndef	TCP_SERVER_H_
#define	TCP_SERVER_H_

#include <sys/socket.h> 
#include <arpa/inet.h>
#include <map>
#include <list>
#include <libjson/libjson.h>
#include "message_process.h"
#include "message.h"
#include "locker.h"
#include "tcp_session.h"

class	TCPSession;

class	TCPServer : public MessageProcess
{
public:

	struct MessageSessionDisconnected : Message
	{
		uint16_t	port;
		MessageSessionDisconnected(uint16_t _port) : Message(TYPE_SESSION_DISCONNECTED), port(_port) {};
	};

	struct MessagePacketReceived : Message	
	{
		uint16_t	port;

		uint8_t*	data;
		uint32_t	length;
		MessagePacketReceived(uint16_t _port, void* _pdata, uint32_t _length);
		~MessagePacketReceived();
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

	TCPServer(void* _data);
	~TCPServer();

	RetValue	Set(Properties* _properties);

	void*		GetData();

	RetValue	SessionDisconnected(uint16_t _port);
	RetValue	PacketReceived(uint16_t _port, void* _data, uint32_t _length);

	uint32_t	GetSessionCount();
	RetValue	GetSessionInformationList(std::list<TCPSession::Information>& _information_list);

	void		OnMessage(Message *_message);
	void		OnPacketReceived(MessagePacketReceived *_message);
protected:

	void		PreProcess();
	void		Process();
	void		PostProcess();

	Properties	properties_;

	void*		data_;
	int			socket_;	

	Locker		session_map_locker_;
	std::map<uint16_t, TCPSession*>	session_map_;
};

#endif
