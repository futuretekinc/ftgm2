#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <mutex>
#include <unistd.h>
#include "trace.h"
#include "object_manager.h"
#include "snmp_object.h"
#include "snmp_client.h"
#include "device.h"
#include "device_fte.h"
#include "endpoint.h"
#include "shell.h"
#include "shell_command.h"
#include "data_manager.h"

using namespace std;

extern	ShellCommand<ObjectManager>*	object_manager_shell_commands[];
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

	ObjectManager*	om = new ObjectManager;
	DataManager*	dm = new DataManager;

	TRACE_SET_LEVEL(om, Trace::Level::INFO);
	TRACE_SET_LEVEL(dm, Trace::Level::INFO);
	TRACE_SET_LEVEL("global", Trace::Level::INFO);

	om->Load(config_file_name);
	om->Connect(dm);
	
	om->Start();
	if (start_shell)
	{
		Shell<ObjectManager>	shell(object_manager_shell_commands, object_manager_shell_command_count, om);

		shell.Start(true);
	}

	om->Stop();
	om->Disconnect(dm);

	delete dm;
	delete om;

	return	0;	
}


