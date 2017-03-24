#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

using namespace std;

extern	Shell::Command	object_manager_command_client;
extern	Shell::Command	object_manager_command_trace;
extern	Shell::Command	object_manager_command_help;
extern	Shell::Command	object_manager_command_help2;
extern	Shell::Command	object_manager_command_quit;

Shell::Command*	object_manager_shell_commands[] = 
{
	&object_manager_command_client,
	&object_manager_command_trace,
	&object_manager_command_help,
	&object_manager_command_help2,
	&object_manager_command_quit
};

int	object_manager_shell_command_count = sizeof(object_manager_shell_commands) / sizeof(Shell::Command *);

