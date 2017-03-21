#include <iostream>
#include <iomanip>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"
#include "string_utils.h"

using namespace std;

RetValue	ShellCommandDevice
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
)
{
	RetValue	ret_value = RET_VALUE_OK;
	ObjectManager*	object_manager = _shell->Data();

	if (object_manager == NULL)
	{
		ret_value = RET_VALUE_OBJECT_MANAGER_NOT_INITIALIZED;	
	}
	else 
	{
		if (_count == 1)
		{
			object_manager->ShowDeviceList();
		}
		else if ((_count >= 3) && (_arguments[1] == "create"))
		{
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
						if (caseInsCompare(_arguments[index], "--id"))
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
						else if (caseInsCompare(_arguments[index], "--name"))
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
						else if (caseInsCompare(_arguments[index], "--peer"))
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
		else if ((_count >= 3) && (_arguments[1] == "delete"))
		{
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
		else 
		{
			Device*	device = object_manager->GetDevice(_arguments[1]);	
			if (device == NULL)
			{
				_shell->Out() << "Device[" << _arguments[1] << "] not found!" << endl;
				ret_value = RET_VALUE_OBJECT_NOT_FOUND;	
			}
			else
			{
				switch(_count)
				{
				case	3:
					{
						if (caseInsCompare(_arguments[2], "type"))
						{
							_shell->Out() << "The deive[" << _arguments[1] << "] type is " << Device::TypeToString(device->GetType()) << endl;
						}
						else if (caseInsCompare(_arguments[2], "name"))
						{
							_shell->Out() << "The deive[" << _arguments[1] << "] name is " << device->GetName() << endl;
						}
						else if (caseInsCompare(_arguments[2], "state"))
						{
							_shell->Out() << "The deive[" << _arguments[1] << "] state is " << (device->GetEnable()?"enabled":"disabled") << endl;
						}
						else if (caseInsCompare(_arguments[2],"enable"))
						{
							device->SetEnable(true);
							_shell->Out() << "The device[" << _arguments[1] << "] is enabled." << endl;
						}
						else if (caseInsCompare(_arguments[2], "disable"))
						{
							device->SetEnable(false);
							_shell->Out() << "The device[" << _arguments[1] << "] is disabled." << endl;
						}
						else if (caseInsCompare(_arguments[2], "activate"))
						{
							ret_value = object_manager->SetDeviceActivation(_arguments[1], true);
							if (ret_value == RET_VALUE_OK)
							{
								_shell->Out() << "The device[" << _arguments[1] << "] is activated." << endl;
							}
							else
							{
								_shell->Out() << "The device[" << _arguments[1] << "] is failed to activation." << endl;
							}
						}
						else if (caseInsCompare(_arguments[2], "deactivate"))
						{
							ret_value = object_manager->SetDeviceActivation(_arguments[1], false);
							if (ret_value == RET_VALUE_OK)
							{
								_shell->Out() << "The device[" << _arguments[1] << "] is deactivated." << endl;
							}
							else
							{
								_shell->Out() << "The device[" << _arguments[1] << "] is failed to deactivation." << endl;
							}
						}
						else
						{
							ret_value = RET_VALUE_NOT_SUPPORTED_FUNCTION;	
						}
					}
					break;

				case	4:
					{
						if (caseInsCompare(_arguments[2], "name"))
						{
							ret_value = object_manager->SetDeviceProperty(_arguments[1], _arguments[2], _arguments[3]);
							if (ret_value == RET_VALUE_OK)
							{
								_shell->Out() << "The deive[" << _arguments[1] << "] name was changed to  " << _arguments[3] << endl;
							}
							else 
							{
								_shell->Out() << "Failed to change deive[" << _arguments[1] << "] name." << endl;
							}
						}

						if (caseInsCompare(_arguments[2], "name"))
						{
							ret_value = object_manager->SetDeviceProperty(_arguments[1], _arguments[2], _arguments[3]);
							if (ret_value == RET_VALUE_OK)
							{
								_shell->Out() << "The deive[" << _arguments[1] << "] name was changed to  " << _arguments[3] << endl;
							}
							else 
							{
								_shell->Out() << "Failed to change deive[" << _arguments[1] << "] name." << endl;
							}
						}
					}
				}
			}
		}
	}

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

ShellCommand<ObjectManager>	object_manager_command_device =
{
	.name		= "device",
	.help		= "<command>\n"
				"\tManagement of device.\n"
				"COMMANDS:\n"
				"\tcreate <TYPE> [OPTIONS]\n"
				"OPTIONS:\n"
				"\t--id <ID>\n"
				"\t\tUnique identifier\n"
				"\t--name <NAME>\n"
				"\t\tName\n",
	.short_help	= "Management of device.",
	.function	= ShellCommandDevice
};

