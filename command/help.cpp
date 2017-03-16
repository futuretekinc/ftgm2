#include <iostream>
#include <iomanip>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

using namespace std;

RetValue	ShellCommandHelp
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
		for(uint32 i =  0 ; i < _shell->GetCommandCount(); i++)
		{
			ShellCommand<ObjectManager>*	command = _shell->GetCommandAt(i);
			if (command != NULL)
			{
				cout << setw(16) << command->command << " " << command->short_help << endl;
			}
		
		}
	}
	else 
	{
	}

	return	ret_value;
}
