#include "message.h"
#include <string>

using namespace std;

struct	MessageTypeString
{
	Message::Type	type;
	string			name;
}
message_type_string[] =
{
	{ Message::TYPE_UNKNOWN,	"Unknown" },
	{ Message::TYPE_START,		"start"	},
	{ Message::TYPE_STARTED,	"started"	},
	{ Message::TYPE_STOP,		"stop"	},
	{ Message::TYPE_STOPPED,	"stopped"	},
	{ Message::TYPE_ACTIVATED,	"activated" },
	{ Message::TYPE_DEACTIVATED,"deactivated" },
	{ Message::TYPE_TEST,		"test"	},
	{ Message::TYPE_QUIT,		"quit"	},
	{ Message::TYPE_SESSION_DISCONNECTED, "Session disconnected" },
	{ Message::TYPE_PACKET_RECEIVED,	"Packet received" }
};

const 
string&	ToString
(
	Message* _message
)
{
	static	string	unknown("unknown");

	for(int i = 0 ; sizeof(message_type_string) / sizeof(MessageTypeString) ; i++)
	{
	 	if (_message->type == message_type_string[i].type)
		{
			return	message_type_string[i].name;
		}
	}

	return	unknown;
}


Message::Type	Message::ToType
(
	const std::string& _string
)
{
	for(int i = 0 ; sizeof(message_type_string) / sizeof(MessageTypeString) ; i++)
	{
	 	if (_string == message_type_string[i].name)
		{
			return	message_type_string[i].type;
		}
	}

	return	TYPE_UNKNOWN;
}
	

const std::string&	Message::ToString
(
	Type _type
)
{
	for(int i = 0 ; sizeof(message_type_string) / sizeof(MessageTypeString) ; i++)
	{
	 	if (_type == message_type_string[i].type)
		{
			return	message_type_string[i].name;
		}
	}

	return	message_type_string[0].name;
}

MessageTest::MessageTest
(
	const string& _message
)
: Message(TYPE_TEST), message(_message)
{
}

MessageActivated::MessageActivated()
: Message(TYPE_ACTIVATED)
{
}

MessageDeactivated::MessageDeactivated()
: Message(TYPE_DEACTIVATED)
{
}

MessageQuit::MessageQuit()
: Message(TYPE_QUIT)
{
}
