#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

using namespace std;

RetValue	ShellCommandDebug
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
)
{
	RetValue	ret_value;
	size_t		title_width = 16;

	if (_count == 1)
	{
		_shell->Out() << setw(title_width) << "Trace : " << ((debug.Trace())?"on":"off") << endl;
		_shell->Out() << setw(title_width) << "Error : " << ((debug.Error())?"on":"off") << endl;
	}
	else 
	{
		if (strcasecmp(_arguments[1].c_str(), "trace") == 0)
		{
			if (_count == 3)
			{
				if (strcasecmp(_arguments[2].c_str(), "on") == 0)
				{
					TRACE_ON();	
				}
				else if (strcasecmp(_arguments[2].c_str(), "off") == 0)
				{
					TRACE_OFF();	
				}
				else
				{
					ret_value = RET_VALUE_INVALID_ARGUMENTS;	
				}
			}
			else
			{
				ret_value = RET_VALUE_INVALID_ARGUMENTS;	
			}
		}
		else if (strcasecmp(_arguments[1].c_str(), "error") == 0)
		{
			if (_count == 3)
			{
				if (strcasecmp(_arguments[2].c_str(), "on") == 0)
				{
					ERROR_ON();	
				}
				else if (strcasecmp(_arguments[2].c_str(), "off") == 0)
				{
					ERROR_OFF();
				}
				else
				{
					ret_value = RET_VALUE_INVALID_ARGUMENTS;	
				}
			}
			else
			{
				ret_value = RET_VALUE_INVALID_ARGUMENTS;	
			}
		}
		else
		{
			ret_value = RET_VALUE_NOT_SUPPORTED_FUNCTION;
		}
	}

	return	ret_value;
}
