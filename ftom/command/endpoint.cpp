#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"
#include "string_utils.h"

using namespace std;

RetValue	ShellCommandEndpoint
(
	string* _arguments, 
	uint32_t	_count,
	Shell* _shell
)
{
	ASSERT(_arguments != NULL);
	ASSERT(_shell != NULL);

	RetValue	ret_value = RET_VALUE_OK;

	ObjectManager*object_manager = (ObjectManager*)_shell->Data();
	ASSERT(object_manager != NULL);

	if (_count == 1)
	{
		object_manager->ShowEndpointList();
		goto finished;
	}

	if (IsCorrectOption(_arguments[1], "create"))
	{
		if (_count < 5)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;
			goto finished;
		}

		Endpoint::Type	type = Endpoint::StringToType(_arguments[2]);
		if (type == Endpoint::UNKNOWN)
		{
			ret_value = RET_VALUE_INVALID_TYPE;
			_shell->Out() << "Error : Unknown endpoint type[" << _arguments[2] <<"]!" << endl;
			goto finished;
		}

		Endpoint::Properties	*properties = Endpoint::Properties::Create(type);
		if (properties == NULL)
		{
			ret_value = RET_VALUE_INVALID_TYPE;
			_shell->Out() << "Error : Not supported endpoint[" << _arguments[2] << "]!" << endl;	
			goto finished;
		}

		for(uint32_t i = 3 ; i+1 < _count ; i+=2)
		{
			if (IsCorrectOption(_arguments[i], "--device_id"))
			{
				properties->device_id = _arguments[i+1];
			}
			else if (IsCorrectOption(_arguments[i], "--id"))
			{
				properties->id = _arguments[i+1];
			}
			else if (IsCorrectOption(_arguments[i], "--name"))
			{
				properties->name = _arguments[i+1];
			}
			else if (IsCorrectOption(_arguments[i], "--index"))
			{
				properties->index = atoi(_arguments[i+1].c_str());
			}
			else
			{
				ret_value = RET_VALUE_INVALID_ARGUMENTS;
				break;
			}
		}

		if (ret_value != RET_VALUE_OK)
		{
			delete properties;	
			goto finished;
		}

		ret_value = object_manager->CreateEndpoint(properties);
		if (ret_value != RET_VALUE_OK)
		{
			_shell->Out() << "Failed to create endpoint[" << _arguments[2] <<"]!" << endl;
		}
		else
		{
			_shell->Out() << "The endpoint[" << _arguments[2] << "] was succefully created." << endl;
		}

		delete properties;
	}
	else if (IsCorrectOption(_arguments[1], "delete"))
	{
		uint32_t	index;

		for(index = 2; index < _count ; index++)
		{
			ret_value = object_manager->DestroyEndpoint(_arguments[index]);
			if (ret_value == RET_VALUE_OK)
			{
				_shell->Out() << "The endpoint[" << _arguments[index] << "] was detached." << endl;
			}
			else
			{
				_shell->Out() << "Failed to detach the endpoint[" << _arguments[index] << "]." << endl;
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


		if (IsCorrectOption(_arguments[2], "all"))
		{
			for(uint32 i = 0 ; i < object_manager->GetEndpointCount() ; i++)
			{
				Endpoint*	endpoint = object_manager->GetEndpoint(i);
				if (endpoint != NULL)
				{
					ret_value = endpoint->Start();	
					if (ret_value == RET_VALUE_OK)
					{
						_shell->Out() << "The endpoint[" << endpoint->GetID() << "] was started." << endl;
					}
					else
					{
						_shell->Out() << "The endpoint[" << endpoint->GetID() << "] is failed to start." << endl;
					}
				}
				else
				{
					_shell->Out() << i << "th endpoint not exist!" << endl;
				}

			}
		}
		else
		{
			for(uint32_t i = 2 ; i < _count ; i++)
			{
				Endpoint *endpoint = object_manager->GetEndpoint(_arguments[i]);
				if (endpoint == NULL)
				{
					_shell->Out() << "Error : Failed to get endpoint[" << _arguments[i] << "]." << endl;
				}
				else
				{
					ret_value = endpoint->Start();
					if (ret_value == RET_VALUE_OK)
					{
						_shell->Out() << "The endpoint[" << _arguments[i] << "] was started.";
					}
					else
					{
						_shell->Out() << "The endpoint[" << _arguments[i] << "] is failed to start. - [Ret: " << ret_value << "]";
					}
				}
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

		if (IsCorrectOption(_arguments[2], "all"))
		{
			for(uint32 i = 0 ; i < object_manager->GetEndpointCount() ; i++)
			{
				Endpoint*	endpoint = object_manager->GetEndpoint(i);
				if (endpoint != NULL)
				{
					ret_value = endpoint->Stop();	
					if (ret_value == RET_VALUE_OK)
					{
						_shell->Out() << "The endpoint[" << _arguments[2] << "] was deactivated.";
					}
					else
					{
						_shell->Out() << "The endpoint[" << _arguments[2] << "] can't deactivate. - [Ret: " << ret_value << "]";
					}
				}
				else
				{
					_shell->Out() << i << "th endpoint not exist!" << endl;
				}

			}
		}
		else
		{
			for(uint32_t i = 2 ; i < _count ; i++)
			{
				Endpoint *endpoint = object_manager->GetEndpoint(_arguments[i]);
				if (endpoint == NULL)
				{
					_shell->Out() << "Error : Failed to get endpoint[" << _arguments[i] << "]." << endl;
				}
				else
				{
					ret_value = endpoint->Stop();
					if (ret_value == RET_VALUE_OK)
					{
						_shell->Out() << "The endpoint[" << _arguments[i] << "] was deactivated.";
					}
					else
					{
						_shell->Out() << "The endpoint[" << _arguments[i] << "] can't deactivate. - [Ret: " << ret_value << "]";
					}
				}
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "show"))
	{
		if ((_count < 3) || IsCorrectOption(_arguments[2], "all"))
		{
			for(uint32_t i = 0 ; i < object_manager->GetEndpointCount() ; i++)
			{
				Endpoint *endpoint = object_manager->GetEndpoint(i);	
				if (endpoint == NULL)
				{
					_shell->Out() << "Error : Endpoint[" << _arguments[i] << "] not found!" << endl;
				}
				else
				{
					endpoint->Show(_shell->Out());
				}
			}
		}
		else
		{
			for(uint32_t i = 2 ; i < _count ; i++)
			{
				Endpoint *endpoint = object_manager->GetEndpoint(_arguments[i]);	
				if (endpoint == NULL)
				{
					_shell->Out() << "Error : Endpoint[" << _arguments[i] << "] not found!" << endl;
				}
				else
				{
					endpoint->Show(_shell->Out());
				}
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "value"))
	{
		if (_count < 3)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;		
			goto finished;
		}

		Endpoint*	endpoint = object_manager->GetEndpoint(_arguments[2]);
		if (endpoint == NULL)
		{
			_shell->Out() << "Error : Endpoint[" << _arguments[2] << "] not found!" << endl;
			ret_value = RET_VALUE_OBJECT_NOT_FOUND;
			goto finished;	
		}

		uint32_t	count = endpoint->ValueCount();
		_shell->Out() << "[ Endpoint Values ]" << endl;
		_shell->Out() << setw(16) << "Count" << " : " << count << endl << endl;
		if (count != 0)
		{
			int	i = 0;;
			list<Value>	value_list;

			for(auto it = value_list.begin() ; it != value_list.end() ; it++)
			{
				Time	time = it->GetTime();
				_shell->Out() <<  setw(4) << ++i << " " << time.ToString() << " : " << it->ToString() << endl;
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
			ret_value = object_manager->SetEndpointProperty(_arguments[i], _arguments[1], true);
			if (ret_value == RET_VALUE_OK)
			{
				_shell->Out() << "The endpoint[" << _arguments[i] << "] was enabled." << endl;
			}
			else
			{
				_shell->Out() << "The endpoint[" << _arguments[i] << "] is failed to enable." << endl;
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
			ret_value = object_manager->SetEndpointProperty(_arguments[i], _arguments[1], false);
			if (ret_value == RET_VALUE_OK)
			{
				_shell->Out() << "The endpoint[" << _arguments[i] << "] was disabled." << endl;
			}
			else
			{
				_shell->Out() << "The endpoint[" << _arguments[i] << "] is failed to disable." << endl;
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
			Endpoint*	endpoint = object_manager->GetEndpoint(_arguments[i]);
			if (endpoint == NULL)
			{
				_shell->Out() << "Error : Endpoint[" << _arguments[2] << "] not found!" << endl;
				ret_value = RET_VALUE_OBJECT_NOT_FOUND;
				goto finished;	
			}

			ret_value = endpoint->Start();
			if (ret_value == RET_VALUE_OK)
			{
				_shell->Out() << "The endpoint[" << endpoint->GetID() << "] was started." << endl;
			}
			else
			{
				_shell->Out() << "The endpoint[" << endpoint->GetID() << "] is failed to start." << endl;
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
			Endpoint*	endpoint = object_manager->GetEndpoint(_arguments[i]);
			if (endpoint == NULL)
			{
				_shell->Out() << "Error : Endpoint[" << _arguments[2] << "] not found!" << endl;
				ret_value = RET_VALUE_OBJECT_NOT_FOUND;
				goto finished;	
			}

			ret_value = endpoint->Stop();
			if (ret_value == RET_VALUE_OK)
			{
				_shell->Out() << "The endpoint[" << endpoint->GetID() << "] was stopped." << endl;
			}
			else
			{
				_shell->Out() << "The endpoint[" << endpoint->GetID() << "] is failed to stop." << endl;
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "set"))
	{
		if (_count < 3)
		{
			ret_value = RET_VALUE_INVALID_ARGUMENTS;		
			goto finished;
		}

		Endpoint*	endpoint = object_manager->GetEndpoint(_arguments[2]);
		if (endpoint == NULL)
		{
			_shell->Out() << "Error : Endpoint[" << _arguments[2] << "] not found!" << endl;
			ret_value = RET_VALUE_OBJECT_NOT_FOUND;
			goto finished;	
		}


		for(uint32_t i = 3 ; i+1 < _count ; i+=2)
		{
			if (IsCorrectOption(_arguments[i], "--id") || IsCorrectOption(_arguments[i], "--name") || IsCorrectOption(_arguments[i], "--device_id"))
			{
				ret_value = object_manager->SetEndpointProperty(_arguments[2], _arguments[i].substr(2), _arguments[i+1]);
				if (ret_value == RET_VALUE_OK)
				{
					_shell->Out() << "The deive[" << _arguments[2] << "] " << _arguments[i].substr(2) << " was changed to " << _arguments[i+1] << endl;
				}
				else 
				{
					_shell->Out() << "Failed to change " << _arguments[i].substr(2) << " of deive[" << _arguments[2] << "]" << endl;
				}
			}
			else if (IsCorrectOption(_arguments[i], "--index") || IsCorrectOption(_arguments[i], "--update_interval") || IsCorrectOption(_arguments[i], "--value_count"))
			{
				uint32_t	value = strtoul(_arguments[i+1].c_str(), 0, 10);

				ret_value = object_manager->SetEndpointProperty(_arguments[2], _arguments[i].substr(2), value);
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
					_shell->Out() << "The endpoint[" << _arguments[2] << "] was " << ((enable)?"enabled":"disabled") << endl;
				}
				else 
				{
					_shell->Out() << "The endpoint[" << _arguments[2] << "] is failed to " << _arguments[i].substr(2) << endl;
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

Shell::Command	object_manager_command_endpoint =
{
	.name		= "endpoint",
	.help		= "<command>\n"
				"\tManagement of endpoint.\n"
				"COMMANDS:\n"
				"\tcreate <TYPE> [OPTIONS]\n"
				"\tcreate <TYPE> [OPTIONS]\n"
				"OPTIONS:\n"
				"\t--id <ID>\n"
				"\t\tUnique identifier\n"
				"\t--name <NAME>\n"
				"\t\tName\n"
				"\t--index <INDEX>\n"
				"\t\tThe index of the device's endpoint\n"
				"\t--device_id <DEVICE_ID>\n"
				"\t\tDevice id\n"
				"\t--update_interval <MS>\n"
				"\t\tData collection cycle time.\n"
				"\t--value_count <COUNT>\n"
				"\t\tMaximum number of saved data.\n"
				"PARAMETERS:\n"
				"\tTYPE\tEndpoint type.\n"
				"\t\ttemperature\n"
				"\t\thumidity\n"
				"\t\tvoltage\n"
				"\tDEVICE_ID\tThe ID of the device to which the endpoint belongs.\n"
				"\tID\tUnique identifier of the endpoint.\n"
				"\tNAME\tName of the endpoint.\n",
	.short_help	= "Management of endpoint.",
	.function	= ShellCommandEndpoint
};

