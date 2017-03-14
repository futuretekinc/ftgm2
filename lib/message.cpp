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
	{ Message::TYPE_START,	"start"	},
	{ Message::TYPE_STARTED,"started"	},
	{ Message::TYPE_STOP,	"stop"	},
	{ Message::TYPE_STOPPED,"stopped"	},
	{ Message::TYPE_ACTIVATED,	"activated" },
	{ Message::TYPE_DEACTIVATED,	"deactivated" },
	{ Message::TYPE_TEST,		"test"	},
	{ Message::TYPE_QUIT,	"quit"	}
};

const 
string&	ToString
(
	Message& _message
)
{
	static	string	unknown("unknown");

	for(int i = 0 ; sizeof(message_type_string) / sizeof(MessageTypeString) ; i++)
	{
	 	if (_message.type == message_type_string[i].type)
		{
			return	message_type_string[i].name;
		}
	}

	return	unknown;
}

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
