#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"
#include "string_utils.h"

using namespace std;

RetValue	ShellCommandTrace
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell* _shell
)
{
	RetValue	ret_value;
	size_t		title_width = 16;

	if (_count == 1)
	{
		Trace::Properties *properties;
		uint32	index = 0;

		cout << setw(16) << "Class" << " " << setw(16) << "Level" << endl;
		while((properties = _trace.GetPropertiesAt(index)) != NULL)
		{
			cout << setw(16) << properties->name << " " << setw(16) <<  Trace::GetLevelString(properties->level) << endl;
			index++;	
		}


	}
	else if (_count == 3)
	{
		_trace.SetLevel(_arguments[1], _arguments[2]);		
	}
	else
	{
		if (caseInsCompare(_arguments[1], "trace"))
		{
			if (_count == 3)
			{
				if (caseInsCompare(_arguments[2], "on"))
				{
				}
				else if (caseInsCompare(_arguments[2], "off"))
				{
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

Shell::Command	object_manager_command_trace =
{
	.command	= "trace",
	.help		= "trace",
	.short_help	= "trace",
	.function	= ShellCommandTrace
};
