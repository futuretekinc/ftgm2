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
	Shell<ObjectManager, DataManager>* _shell
);

RetValue	ShellCommandEndpoint
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager, DataManager>* _shell
);

RetValue	ShellCommandDatabase
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager, DataManager>* _shell
);

RetValue	ShellCommandDebug
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager, DataManager>* _shell
);

RetValue	ShellCommandHelp
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager, DataManager>* _shell
);

ShellCommand<ObjectManager, DataManager>	object_manager_shell_commands[] = 
{
	ShellCommand<ObjectManager, DataManager>("device", 	"devcice", 	"device", 	ShellCommandDevice),
	ShellCommand<ObjectManager, DataManager>("endpoint","endpoint", "endpoint", ShellCommandEndpoint),
	ShellCommand<ObjectManager, DataManager>("database","database",	"database",	ShellCommandDatabase),
	ShellCommand<ObjectManager, DataManager>("debug", 	"debug", 	"debug", 	ShellCommandDebug),
	ShellCommand<ObjectManager, DataManager>("help", 	"help", 	"help", 	ShellCommandHelp),
	ShellCommand<ObjectManager, DataManager>("?", 	"help", 	"help", 		ShellCommandHelp)
};

int	object_manager_shell_command_count = sizeof(object_manager_shell_commands) / sizeof(ShellCommand<ObjectManager, DataManager>);
