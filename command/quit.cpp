#include <iostream>
#include <iomanip>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

using namespace std;

RetValue	ShellCommandQuit
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
)
{
	_shell->Stop();

	return	RET_VALUE_OK;
}

ShellCommand<ObjectManager>	object_manager_command_quit =
{
	.command	= "quit",
	.help		= "quit",
	.short_help	= "quit",
	.function	= ShellCommandQuit
};
