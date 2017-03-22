#include <iostream>
#include <iomanip>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"
#include "string_utils.h"

using namespace std;

RetValue	ShellCommandConfig
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
)
{
	RetValue	ret_value = RET_VALUE_OK;

	ObjectManager*object_manager = _shell->Data();

	if (_count == 1)
	{
	}
	else if (_count == 3)
	{
		if (IsCorrectOption(_arguments[1], "save"))
		{
			if (IsCorrectOption(_arguments[2], "device"))
			{
				ret_value = object_manager->SaveLoadedDevices();
			}
			else if (IsCorrectOption(_arguments[2], "endpoint"))
			{
				ret_value = object_manager->SaveLoadedEndpoints();
			}
			else if (IsCorrectOption(_arguments[2], "all"))
			{
				ret_value = object_manager->SaveLoadedDevices();
				ret_value = object_manager->SaveLoadedEndpoints();
			}
		}
		else if (IsCorrectOption(_arguments[1], "load"))
		{
			if (IsCorrectOption(_arguments[2], "show"))
			{
				list<string>	device_list;
				list<string>	endpoint_list;

				object_manager->GetLoadedDeviceList(device_list);

				cout << "[ Loaded Device List ]" <<endl;
				cout << setw(16) << "ID" << setw(16) << "Name" << endl;
				for(auto it = device_list.begin(); it != device_list.end(); it++)
				{
					Device*	device = object_manager->GetDevice(*it);

					cout << setw(16) << device->GetID() << setw(16) << device->GetName() << endl;
				}

				object_manager->GetLoadedEndpointList(endpoint_list);

				cout << endl << "[ Loaded Endpoint List ]" <<endl;
				cout << setw(16) << "ID" << setw(16) << "Name" << setw(16) << "Device ID" << endl;
				for(auto it = endpoint_list.begin(); it != endpoint_list.end() ; it++)
				{
					Endpoint*	endpoint = object_manager->GetEndpoint(*it);

					cout << setw(16) << endpoint->GetID() << setw(16) << endpoint->GetName() << setw(16) << endpoint->GetDeviceID() << endl;
				}
			}
			else
			{
				ret_value = object_manager->Load(_arguments[2]);	

				if (ret_value == RET_VALUE_OK)
				{
					cout << "Settings loaded" << endl;
				}
				else
				{
					cout << "Failed to load settings." << endl;	
				}
			}
		}
	}

	return	ret_value;
}

ShellCommand<ObjectManager>	object_manager_command_config =
{
	.name		= "config",
	.help		= "<command>\n"
				  "\tManage settings.\n"
				  "COMMANDS:\n"
				  "\tload <file>\n"
				  "\t\tLoad settings from file.\n"
				  "\tshow\n"
				  "\t\tShow settings.\n"
				  "OPTIONS:\n"
				  "\tfile\tJSON based configuration file.",
	.short_help	= "Configuration settings.",
	.function	= ShellCommandConfig
};

