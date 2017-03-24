#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

using namespace std;

extern	Shell::Command	object_manager_command_config;
extern	Shell::Command	object_manager_command_device;
extern	Shell::Command	object_manager_command_endpoint;
extern	Shell::Command	object_manager_command_trace;
extern	Shell::Command	object_manager_command_help;
extern	Shell::Command	object_manager_command_help2;
extern	Shell::Command	object_manager_command_server;
extern	Shell::Command	object_manager_command_quit;

Shell::Command*	object_manager_shell_commands[] = 
{
	&object_manager_command_config,
	&object_manager_command_device,
	&object_manager_command_endpoint,
	&object_manager_command_trace,
	&object_manager_command_help,
	&object_manager_command_help2,
	&object_manager_command_server,
	&object_manager_command_quit
};

int	object_manager_shell_command_count = sizeof(object_manager_shell_commands) / sizeof(Shell::Command *);

