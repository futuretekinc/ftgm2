#include <iostream>
#include <iomanip>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

using namespace std;

extern	ShellCommand<ObjectManager>	object_manager_command_config;
extern	ShellCommand<ObjectManager>	object_manager_command_device;
extern	ShellCommand<ObjectManager>	object_manager_command_endpoint;
extern	ShellCommand<ObjectManager>	object_manager_command_trace;
extern	ShellCommand<ObjectManager>	object_manager_command_help;
extern	ShellCommand<ObjectManager>	object_manager_command_help2;
extern	ShellCommand<ObjectManager>	object_manager_command_quit;

ShellCommand<ObjectManager>*	object_manager_shell_commands[] = 
{
	&object_manager_command_config,
	&object_manager_command_device,
	&object_manager_command_endpoint,
	&object_manager_command_trace,
	&object_manager_command_help,
	&object_manager_command_help2,
	&object_manager_command_quit
};

int	object_manager_shell_command_count = sizeof(object_manager_shell_commands) / sizeof(ShellCommand<ObjectManager> *);

