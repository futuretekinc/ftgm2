#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <mutex>
#include <unistd.h>
#include <errno.h>
#include "trace.h"
#include "snmp_object.h"
#include "snmp_client.h"
#include "device.h"
#include "device_fte.h"
#include "endpoint.h"
#include "shell.h"
#include "shell_command.h"
#include "data_manager.h"

using namespace std;

int main
(
	int		argc,
	char*	argv[]
)
{
	int		option;
	bool	start_shell = false;
	bool	show_usage = false;
	bool	load_from_file = false;
	string	config_file_name = string(program_invocation_short_name) + string(".conf");

	while((option = getopt(argc, argv, "si?")) != -1) 
	{   
		switch(option)
		{   
		case    's':
			{   
				start_shell = true;
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

	DataManager*	dm = new DataManager;

	TRACE_ON();
	ERROR_ON();

	dm->Load(config_file_name);

	dm->Start(1000);

	if (start_shell)
	{
		Shell<DataManager>	shell(data_manager_shell_commands, data_manager_shell_command_count, dm);

		shell.Start(true);
	}

	dm->Stop();

	delete dm;

	return	0;	
}


