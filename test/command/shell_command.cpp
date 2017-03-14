#include <iostream>
#include <iomanip>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

using namespace std;

RetValue	ShellCommandTest
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager, DataManager>* _shell
);

ShellCommand<ObjectManager, DataManager>	test_shell_commands[] = 
{
	ShellCommand<ObjectManager, DataManager>("test", "test", "test", ShellCommandTest)
};

int	test_shell_command_count = sizeof(test_shell_commands) / sizeof(ShellCommand<ObjectManager, DataManager>);
