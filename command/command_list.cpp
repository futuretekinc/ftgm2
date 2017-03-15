#include <iostream>
#include <iomanip>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

using namespace std;

RetValue	ShellCommandDevice
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
);

RetValue	ShellCommandEndpoint
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
);

RetValue	ShellCommandDatabase
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
);

RetValue	ShellCommandDebug
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
);

RetValue	ShellCommandHelp
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
);

RetValue	ShellCommandQuit
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
);

ShellCommand<ObjectManager>	object_manager_shell_commands[] = 
{
	ShellCommand<ObjectManager>("device", 	"devcice", 	"device", 	ShellCommandDevice),
	ShellCommand<ObjectManager>("endpoint","endpoint", "endpoint", 	ShellCommandEndpoint),
	ShellCommand<ObjectManager>("debug", 	"debug", 	"debug", 	ShellCommandDebug),
	ShellCommand<ObjectManager>("help", 	"help", 	"help", 	ShellCommandHelp),
	ShellCommand<ObjectManager>("?", 		"help", 	"help", 	ShellCommandHelp),
	ShellCommand<ObjectManager>("quit",		"quit", 	"quit",		ShellCommandQuit)
};

int	object_manager_shell_command_count = sizeof(object_manager_shell_commands) / sizeof(ShellCommand<ObjectManager>);

