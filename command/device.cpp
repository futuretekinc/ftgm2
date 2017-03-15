#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

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
				uint32_t	index;

				Device::Properties	*properties = Device::Properties::Create(type);
				if (properties == NULL)
				{
					ret_value = RET_VALUE_INVALID_ARGUMENTS;	
				}
				else
				{
					while(index < _count)
					{
						if (strcasecmp(_arguments[index].c_str(), "--id") == 0)
						{
							if(index+1 < _count)
							{
								properties->id = _arguments[index+1];	
							}
							else
							{
								ret_value = RET_VALUE_INVALID_ARGUMENTS;
								break;	
							}

							index += 2;
						}
						else if (strcasecmp(_arguments[index].c_str(), "--name") == 0)
						{
							if(index+1 < _count)
							{
								properties->name = _arguments[index+1];	
							}
							else
							{
								ret_value = RET_VALUE_INVALID_ARGUMENTS;
								break;	
							}

							index += 2;
						}
						else
						{
							ret_value = RET_VALUE_INVALID_ARGUMENTS;
							break;	
						}
					}

					if (ret_value == RET_VALUE_OK)
					{
						Device*	device = Device::Create(properties);
						if (device == NULL)
						{
							_shell->Out() << "Failed to create device[" << _arguments[2] <<"]!" << endl;
						}
						else
						{
							_shell->Out() << "The device[" << _arguments[2] << "] was succefully created." << endl;
							ret_value = object_manager->Connect(device);
							if (ret_value != RET_VALUE_OK)
							{
								_shell->Out() << "Failed to attach device[" << _arguments[2] << "] at object manager!" << endl;
								delete device;

							}
						}
					}

					delete properties;
				}
			}
		}
		else if ((_count >= 3) && (_arguments[1] == "delete"))
		{
			uint32_t	index;

			for(index = 2; index < _count ; index++)
			{
				Device*	device = object_manager->GetDevice(_arguments[index]);
				if (device == NULL)
				{
					_shell->Out() << "Failed to get device[" << _arguments[index] << "]." << endl;
				}
				else
				{
					ret_value = object_manager->Disconnect(device);
					if (ret_value == RET_VALUE_OK)
					{
						delete device;
						_shell->Out() << "The device[" << _arguments[index] << "] was detached." << endl;
					}
					else
					{
						_shell->Out() << "Failed to detach the device[" << _arguments[index] << "]." << endl;
					}
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
				if (_count == 2)
				{
				}
				else if (_arguments[2] == "activate")
				{
					ret_value = device->Activation();
					if (ret_value == RET_VALUE_OK)
					{
						_shell->Out() << "The device[" << device->GetID() << "] is activated." << endl;
					}
					else
					{
						_shell->Out() << "The device[" << device->GetID() << "] is failed to activation." << endl;
					}
				}
				else if (_arguments[2] == "deactivate")
				{
					ret_value = device->Deactivation();
					if (ret_value == RET_VALUE_OK)
					{
						_shell->Out() << "The device[" << device->GetID() << "] is deactivated." << endl;
					}
					else
					{
						_shell->Out() << "The device[" << device->GetID() << "] is failed to deactivation." << endl;
					}
				}
				else
				{
					ret_value = RET_VALUE_NOT_SUPPORTED_FUNCTION;	
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
