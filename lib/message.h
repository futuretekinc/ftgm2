#ifndef	MESSAGE_H_
#define MESSAGE_H_

#include <string>


class	Message
{
public:
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
		TYPE_SESSION_DISCONNECTED
	};

	Type	type;
	std::string	id;

					Message(const Message& _message) : type(_message.type), id(_message.id) {};
					Message(Type _type = TYPE_UNKNOWN) : type(_type), id("") {};
					Message(Type _type, const std::string& _id) : type(_type), id(_id) {};
	friend	const 
	std::string&	ToString(Message& _message);
	friend	const 
	std::string&	ToString(Message* _message);
};

class	MessageStart : public Message
{
public:
	MessageStart() : Message(TYPE_START) {};
	MessageStart(const std::string& _id) : Message(TYPE_START, _id) {};
};

class	MessageStarted : public Message
{
public:
	MessageStarted() : Message(TYPE_STARTED) {};
	MessageStarted(const std::string& _id) : Message(TYPE_STARTED, _id) {};
};

class	MessageStop : public Message
{
public:
	MessageStop() : Message(TYPE_STOP) {};
	MessageStop(const std::string& _id) : Message(TYPE_STOP, _id) {};
};

class	MessageStopped : public Message
{
public:
	MessageStopped() : Message(TYPE_STOPPED) {};
	MessageStopped(const std::string& _id) : Message(TYPE_STOPPED, _id) {};
};

class	MessageTest : public Message
{
public:
	std::string	message;

	MessageTest(const std::string& _message);
};

class	MessageActivated : public Message
{
public:
	MessageActivated();
};

class	MessageDeactivated : public Message
{
public:
	MessageDeactivated();
};

class	MessageQuit : public Message
{
public:
	MessageQuit();
};

#endif
