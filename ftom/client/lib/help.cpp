#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"
#include "device_snmp.h"
#include "endpoint.h"
#include "string_utils.h"

using namespace std;

RetValue	ShellCommandHelp
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell* _shell
)
{
	RetValue	ret_value;

	if (_count == 1)
	{
		for(uint32 i =  0 ; i < _shell->GetCommandCount(); i++)
		{
			Shell::Command*	command = _shell->GetCommandAt(i);
			if (command != NULL)
			{
				cout << setw(_shell->GetCommandWidth()) << command->name << " " << command->short_help << endl;
			}
		}
	}
	else if (_count == 2)
	{
		Shell::Command* command = _shell->GetCommand(_arguments[1]);
		if (command == NULL)
		{
			cout << "No command " << _arguments[1] << " found" << endl;	
		}
		else 
		{
			cout << command->name << " " << command->help << endl;
		}
	}
	else
	{
		ret_value = RET_VALUE_NOT_SUPPORTED_FUNCTION;
	}

	return	ret_value;
}

Shell::Command	object_manager_command_help =
{
	.name		= "help",
	.help		= "help",
	.short_help	= "help",
	.function	= ShellCommandHelp
};

Shell::Command	object_manager_command_help2 =
{
	.name		= "?",
	.help		= "help",
	.short_help	= "help",
	.function	= ShellCommandHelp
};
