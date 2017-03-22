#include <iostream>
#include <iomanip>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"
#include "device_snmp.h"
#include "endpoint.h"
#include "string_utils.h"

using namespace std;

RetValue	ShellCommandHelp
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
)
{
	RetValue	ret_value;

	if (_count == 1)
	{
		for(uint32 i =  0 ; i < _shell->GetCommandCount(); i++)
		{
			ShellCommand<ObjectManager>*	command = _shell->GetCommandAt(i);
			if (command != NULL)
			{
				cout << setw(_shell->GetCommandWidth()) << command->name << " " << command->short_help << endl;
			}
		}
	}
	else if (_count == 2)
	{
		ShellCommand<ObjectManager>* command = _shell->GetCommand(_arguments[1]);
		if (command == NULL)
		{
			cout << "No command " << _arguments[1] << " found" << endl;	
		}
		else 
		{
			cout << command->name << " " << command->help << endl;
		}
	}
	else
	{
		Device::Properties		*snmp_properties = new DeviceSNMP::Properties;
		Endpoint::Properties	*endpoint_properties = new EndpointSensor::Properties(Endpoint::TEMPERATURE_SENSOR);

		cout << "SNMP Properties : " << dynamic_cast<DeviceSNMP::Properties*>(snmp_properties) <<endl;
		cout << "Endpoint Properties : " << dynamic_cast<DeviceSNMP::Properties*>(endpoint_properties) <<endl;

	}

	return	ret_value;
}

ShellCommand<ObjectManager>	object_manager_command_help =
{
	.name		= "help",
	.help		= "help",
	.short_help	= "help",
	.function	= ShellCommandHelp
};

ShellCommand<ObjectManager>	object_manager_command_help2 =
{
	.name		= "?",
	.help		= "help",
	.short_help	= "help",
	.function	= ShellCommandHelp
};
