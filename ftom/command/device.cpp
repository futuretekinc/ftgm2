#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"
#include "string_utils.h"

using namespace std;

RetValue	ShellCommandDevice
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell* _shell
)
{
	ASSERT(_arguments != NULL);
	ASSERT(_shell != NULL);

	RetValue	ret_value = RET_VALUE_OK;
	ObjectManager*	object_manager = (ObjectManager*)_shell->Data();

	ASSERT(object_manager != NULL);

	if (_count == 1)
	{
		object_manager->ShowDeviceList();
		goto finished;
	}

	if (IsCorrectOption(_arguments[1], "create"))
	{
		if (_count < 3)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;
			goto finished;
		}

		Device::Type	type = Device::StringToType(_arguments[2]);
		if (type == Device::TYPE_UNKNOWN)
		{
			_shell->Out() << "Failed to create device[" << _arguments[2] <<"]!" << endl;
		}
		else
		{
			uint32	index = 3;

			Device::Properties	*properties = Device::Properties::Create(type);
			if (properties == NULL)
			{
				_shell->Out() << "Invalid device type [" << type << "]" << endl;
				ret_value = RET_VALUE_INVALID_ARGUMENTS;	
			}
			else
			{
				while(index < _count)
				{
					if (IsCorrectOption(_arguments[index], "--id"))
					{
						if(index+1 < _count)
						{
							properties->id = _arguments[index+1];	
						}
						else
						{
							ret_value = RET_VALUE_INVALID_ARGUMENTS;
							_shell->Out() << "Invalid argument option[" << _arguments[index+1] << "]" << endl;
							break;	
						}

						index += 2;
					}
					else if (IsCorrectOption(_arguments[index], "--name"))
					{
						if(index+1 < _count)
						{
							properties->name = _arguments[index+1];	
						}
						else
						{
							ret_value = RET_VALUE_INVALID_ARGUMENTS;
							_shell->Out() << "Invalid argument option[" << _arguments[index+1] << "]" << endl;
							break;	
						}

						index += 2;
					}
					else if (IsCorrectOption(_arguments[index], "--peer"))
					{
						if(index+1 < _count)
						{
							properties->name = _arguments[index+1];	
						}
						else
						{
							ret_value = RET_VALUE_INVALID_ARGUMENTS;
							_shell->Out() << "Invalid argument option[" << _arguments[index+1] << "]" << endl;
							break;	
						}

						index += 2;
					}
					else
					{
						ret_value = RET_VALUE_INVALID_ARGUMENTS;
						_shell->Out() << "Invalid argument option[" << _arguments[index] << "]" << endl;
						break;	
					}
				}

				if (ret_value == RET_VALUE_OK)
				{
					ret_value = object_manager->CreateDevice(properties);
					if (ret_value != RET_VALUE_OK)
					{
						_shell->Out() << "Failed to create device[" << _arguments[2] << "] at object manager!" << endl;
					}
				}

				delete properties;
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "delete"))
	{
		if (_count < 3)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;	
			goto finished;
		}

		uint32	index;

		for(index = 2; index < _count ; index++)
		{
			ret_value = object_manager->DestroyDevice(_arguments[index]);
			if (ret_value == RET_VALUE_OK)
			{
				_shell->Out() << "The device[" << _arguments[index] << "] was detached." << endl;
			}
			else
			{
				_shell->Out() << "Failed to detach the device[" << _arguments[index] << "]." << endl;
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "show"))
	{
		if (_count < 3)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;	
			goto finished;
		}
	
		for(uint32_t i = 2 ; i < _count ; i++)
		{
			Device*	device = object_manager->GetDevice(_arguments[i]);	
			if (device != NULL)
			{
				device->Show(_shell->Out());
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "start"))
	{
		if (_count < 3)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;	
			goto finished;
		}
	
		for(uint32_t i = 2 ; i < _count ; i++)
		{
			ret_value = object_manager->SetDeviceActivation(_arguments[i], true);
			if (ret_value == RET_VALUE_OK)
			{
				_shell->Out() << "The device[" << _arguments[i] << "] was started." << endl;
			}
			else
			{
				_shell->Out() << "Error : The device[" << _arguments[i] << "] is failed to start." << endl;
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "stop"))
	{
		if (_count < 3)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;	
			goto finished;
		}
	
		for(uint32_t i = 2 ; i < _count ; i++)
		{
			ret_value = object_manager->SetDeviceActivation(_arguments[i], false);
			if (ret_value == RET_VALUE_OK)
			{
				_shell->Out() << "The device[" << _arguments[i] << "] was stopped." << endl;
			}
			else
			{
				_shell->Out() << "Error : The device[" << _arguments[i] << "] is failed to stop." << endl;
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "enable"))
	{
		if (_count < 3)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;		
			goto finished;
		}

		for(uint32_t i = 2 ; i < _count ; i++)
		{
			ret_value = object_manager->SetDeviceProperty(_arguments[i], _arguments[1], true);
			if (ret_value == RET_VALUE_OK)
			{
				_shell->Out() << "The device[" << _arguments[i] << "] was enabled." << endl;
			}
			else
			{
				_shell->Out() << "The device[" << _arguments[i] << "] is failed to enable." << endl;
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "disable"))
	{
		if (_count < 3)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;		
			goto finished;
		}

		for(uint32_t i = 2 ; i < _count ; i++)
		{
			ret_value = object_manager->SetDeviceProperty(_arguments[i], _arguments[1], false);
			if (ret_value == RET_VALUE_OK)
			{
				_shell->Out() << "The device[" << _arguments[i] << "] was disabled." << endl;
			}
			else
			{
				_shell->Out() << "The device[" << _arguments[i] << "] is failed to disable." << endl;
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "set"))
	{	
		if (_count < 5)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;	
			goto finished;
		}
	
		Device*	device = object_manager->GetDevice(_arguments[2]);	
		if (device == NULL)
		{
			_shell->Out() << "Device[" << _arguments[2] << "] not found!" << endl;
			ret_value = RET_VALUE_OBJECT_NOT_FOUND;	
			goto finished;
		}

		for(uint32_t i = 3 ; i + 1 < _count ; i+=2)
		{
			if (IsCorrectOption(_arguments[i], "--name") || IsCorrectOption(_arguments[i], "--peer") || IsCorrectOption(_arguments[i], "--community"))
			{
				ret_value = object_manager->SetDeviceProperty(_arguments[2], _arguments[i].substr(2), _arguments[i+1]);
				if (ret_value == RET_VALUE_OK)
				{
					_shell->Out() << "The deive[" << _arguments[2] << "] " << _arguments[i].substr(2) << " was changed to " << _arguments[i+1] << endl;
				}
				else 
				{
					_shell->Out() << "Failed to change " << _arguments[i].substr(2) << " of deive[" << _arguments[2] << "]" << endl;
				}
			}
			else if (IsCorrectOption(_arguments[i], "--enable"))
			{
				bool	enable = false;

				if (IsCorrectOption(_arguments[i+1], "true") || IsCorrectOption(_arguments[i+1], "yes") ||IsCorrectOption(_arguments[i+1], "on"))
				{	
					enable = true;
				}
				else if (IsCorrectOption(_arguments[i+1], "false") || IsCorrectOption(_arguments[i+1], "no") ||IsCorrectOption(_arguments[i+1], "off"))
				{	
					enable = false;
				}

				ret_value = object_manager->SetEndpointProperty(_arguments[2], _arguments[i].substr(2), enable);
				if (ret_value == RET_VALUE_OK)
				{
					_shell->Out() << "The device[" << _arguments[2] << "] was " << ((enable)?"enabled":"disabled") << endl;
				}
				else 
				{
					_shell->Out() << "The device[" << _arguments[2] << "] is failed to " << _arguments[i].substr(2) << endl;
				}
			}
		}
	}

finished:

	if (ret_value == RET_VALUE_NOT_SUPPORTED_FUNCTION)
	{
		_shell->Out() << "Not supported command : ";

		for(uint32_t i = 0 ; i < _count ; i++)
		{
			_shell->Out() << _arguments[i] << " ";	
		}

		_shell->Out() << endl;
	}


	return	ret_value;
}

Shell::Command	object_manager_command_device =
{
	.name		= "device",
	.help		= "<command>\n"
				"\tManagement of device.\n"
				"COMMANDS:\n"
				"\tcreate <TYPE> [OPTIONS]\n"
				"\t\tCreate new device\n"
				"\tstart <ID> ...\n"
				"\t\tActivate the devices.\n"
				"\tstop <ID> ...\n"
				"\t\tDeactivate the devices.\n"
				"\tset <ID> [OPTIONS]\n"
				"\t\tSet device attributes.\n"
				"OPTIONS:\n"
				"\t--id <ID>\n"
				"\t\tUnique identifier\n"
				"\t--name <NAME>\n"
				"\t\tName\n"
				"\t--peer <ADDRESS>\n"
				"\t\tTarget device address(IP, Port, ID, ...)\n"
				"\t--community <COMMUNITY>\n"
				"\t\tCommunity for the SNMP\n",
	.short_help	= "Management of device.",
	.function	= ShellCommandDevice
};

