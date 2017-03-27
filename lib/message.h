#ifndef	MESSAGE_H_
#define MESSAGE_H_

#include <string>


struct	Message
{

	enum	Type
	{
		TYPE_START,
		TYPE_STARTED,
		TYPE_STOP,
		TYPE_STOPPED,
		TYPE_ACTIVATED,
		TYPE_DEACTIVATED,
		TYPE_TEST,
		TYPE_QUIT,
		TYPE_UNKNOWN,
		TYPE_SESSION_DISCONNECTED,
		TYPE_PACKET_RECEIVED
	};

	Type	type;
	std::string	id;

					Message(const Message& _message) : type(_message.type), id(_message.id) {};
					Message(Type _type = TYPE_UNKNOWN) : type(_type), id("") {};
					Message(Type _type, const std::string& _id) : type(_type), id(_id) {};

	friend	const 
	std::string&	ToString(Message* _message);

	static
	Type			ToType(const std::string& _string);
	static const 
	std::string&	ToString(Type _type);
};

struct	MessageStart : Message
{

	MessageStart() : Message(TYPE_START) {};
	MessageStart(const std::string& _id) : Message(TYPE_START, _id) {};
};

struct	MessageStarted : Message
{

	MessageStarted() : Message(TYPE_STARTED) {};
	MessageStarted(const std::string& _id) : Message(TYPE_STARTED, _id) {};
};

struct	MessageStop : Message
{

	MessageStop() : Message(TYPE_STOP) {};
	MessageStop(const std::string& _id) : Message(TYPE_STOP, _id) {};
};

struct	MessageStopped : Message
{

	MessageStopped() : Message(TYPE_STOPPED) {};
	MessageStopped(const std::string& _id) : Message(TYPE_STOPPED, _id) {};
};

struct	MessageTest : Message
{

	std::string	message;

	MessageTest(const std::string& _message);
};

struct	MessageActivated : Message
{

	MessageActivated();
};

struct	MessageDeactivated : Message
{

	MessageDeactivated();
};

struct	MessageQuit : Message
{

	MessageQuit();
};

#endif
