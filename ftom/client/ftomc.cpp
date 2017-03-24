#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <mutex>
#include <unistd.h>
#include "trace.h"
#include "snmp_object.h"
#include "snmp_client.h"
#include "device.h"
#include "device_fte.h"
#include "endpoint.h"
#include "shell.h"

using namespace std;

extern	Shell::Command*	object_manager_shell_commands[];
extern	int	object_manager_shell_command_count;

Trace	trace;

int main
(
	int		argc,
	char*	argv[]
)
{
	int		option;
	bool	start_shell = true;
	bool	show_usage = false;
	bool	load_from_file = false;
	string	config_file_name = string(program_invocation_short_name) + string(".conf");

	while((option = getopt(argc, argv, "di?")) != -1) 
	{   
		switch(option)
		{   
		case    'd':
			{   
				start_shell = false;
			}   
			break;

		case    'i':
			{   
				load_from_file = true;
				config_file_name = optarg;
			}   
			break;

		case    '?':
		default:
			show_usage = true;
		}   
	}   

	if (start_shell)
	{
		Shell	shell(object_manager_shell_commands, object_manager_shell_command_count, NULL);

		shell.Start(true);
	}

	return	0;	
}


