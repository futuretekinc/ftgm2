#include <iostream>
#include <iomanip>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"
#include "string_utils.h"

using namespace std;

RetValue	ShellCommandEndpoint
(
	string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
)
{
	RetValue	ret_value = RET_VALUE_OK;

	ObjectManager*object_manager = _shell->Data();

	if (object_manager == NULL)
	{
		ret_value = RET_VALUE_OBJECT_MANAGER_NOT_INITIALIZED;	
	}
	else
	{
		if (_count == 1)
		{
			object_manager->ShowEndpointList();
		}
		else if (IsCorrectOption(_arguments[1],"create"))
		{
			Endpoint::Type	type = Endpoint::StringToType(_arguments[2]);
			if (type == Endpoint::UNKNOWN)
			{
				_shell->Out() << "Failed to create endpoint[" << _arguments[2] <<"]!" << endl;
			}
			else
			{
				Device*		device;

				device = object_manager->GetDevice(_arguments[3]);
				if (device != NULL)
				{
					Endpoint::Properties	*properties = Endpoint::Properties::Create(type);
					if (properties == NULL)
					{
						ret_value = RET_VALUE_INVALID_ARGUMENTS;	
					}
					else
					{
						properties->device_id = _arguments[3];
						uint32_t	index = 4;
						while(index < _count)
						{
							if (caseInsCompare(_arguments[index].c_str(), "--id"))
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
							else if (caseInsCompare(_arguments[index].c_str(), "--name"))
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
							else if (caseInsCompare(_arguments[index].c_str(), "--index"))
							{
								if(index+1 < _count)
								{
									properties->index = atoi(_arguments[index+1].c_str());
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
							ret_value = object_manager->CreateEndpoint(properties);
							if (ret_value != RET_VALUE_OK)
							{
								_shell->Out() << "Failed to create endpoint[" << _arguments[2] <<"]!" << endl;
							}
							else
							{
								_shell->Out() << "The endpoint[" << _arguments[2] << "] was succefully created." << endl;
							}
						}

						delete properties;
					}
				}
				else
				{
					_shell->Out() << "Unknown device : " << _arguments[3] << endl;	
				}
			}
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
		else if (IsCorrectOption(_arguments[1], "activate"))
		{
			if (_count == 2)
			{
				ret_value = RET_VALUE_INVALID_ARGUMENTS;		
			}
			else
			{
				
				if (IsCorrectOption(_arguments[2], "all"))
				{
					for(uint32 i = 0 ; i < object_manager->GetEndpointCount() ; i++)
					{
						Endpoint*	endpoint = object_manager->GetEndpoint(i);
						if (endpoint != NULL)
						{
							ret_value = endpoint->Activation();	
							if (ret_value == RET_VALUE_OK)
							{
								cout << "The endpoint[" << endpoint->GetID() << "] is activated." << endl;
							}
							else
							{
								cout << "The endpoint[" << endpoint->GetID() << "] is failed to activation." << endl;
							}
						}
						else
						{
							cout << i << "th endpoint not exist!" << endl;
						}
					
					}
				}
				else
				{
					Endpoint *endpoint = object_manager->GetEndpoint(_arguments[2]);
					if (endpoint == NULL)
					{
						cout << "Error : Failed to get endpoint[" << _arguments[2] << "]." << endl;
					}
					else
					{
						ret_value = endpoint->Activation();
						if (ret_value == RET_VALUE_OK)
						{
							cout << "The endpoint[" << _arguments[2] << "] was activated.";
						}
						else
						{
							cout << "The endpoint[" << _arguments[2] << "] can't activate. - [Ret: " << ret_value << "]";
						}
					}
				}
			}
		}
		else if (IsCorrectOption(_arguments[1], "deactivate"))
		{
			if (_count == 2)
			{
				ret_value = RET_VALUE_INVALID_ARGUMENTS;		
			}
			else
			{
				if (IsCorrectOption(_arguments[2], "all"))
				{
					for(uint32 i = 0 ; i < object_manager->GetEndpointCount() ; i++)
					{
						Endpoint*	endpoint = object_manager->GetEndpoint(i);
						if (endpoint != NULL)
						{
							ret_value = endpoint->Deactivation();	
							if (ret_value == RET_VALUE_OK)
							{
								cout << "The endpoint[" << _arguments[2] << "] was deactivated.";
							}
							else
							{
								cout << "The endpoint[" << _arguments[2] << "] can't deactivate. - [Ret: " << ret_value << "]";
							}
						}
						else
						{
							cout << i << "th endpoint not exist!" << endl;
						}
					
					}
				}
				else
				{
					Endpoint *endpoint = object_manager->GetEndpoint(_arguments[2]);
					if (endpoint == NULL)
					{
						cout << "Error : Failed to get endpoint[" << _arguments[2] << "]." << endl;
					}
					else
					{
						ret_value = endpoint->Deactivation();
						if (ret_value == RET_VALUE_OK)
						{
							cout << "The endpoint[" << _arguments[2] << "] was deactivated.";
						}
						else
						{
							cout << "The endpoint[" << _arguments[2] << "] can't deactivate. - [Ret: " << ret_value << "]";
						}
					}
				}
			}
		}
		else 
		{
			Endpoint *endpoint = object_manager->GetEndpoint(_arguments[1]);	
			if (endpoint == NULL)
			{
				cout << "Endpoint not found!" << endl;
				ret_value = RET_VALUE_OBJECT_NOT_FOUND;	
			}
			else
			{
				if (_count == 2)
				{
					cout << setw(16) << "Type : " << Endpoint::TypeToString(endpoint->GetType()) << endl;
					cout << setw(16) << "ID : " << endpoint->GetID() << endl;
					cout << setw(16) << "Name : " << endpoint->GetName() << endl;
					cout << setw(16) << "Value Count : " << endpoint->ValueCount() << endl;
				}
				else if (IsCorrectOption(_arguments[2], "value"))
				{
					uint32_t	count = endpoint->ValueCount();

					if (count != 0)
					{

						TimedValue*	value_list = new TimedValue[count];

						count = endpoint->GetValueList(value_list, count);
						for(uint32_t i = 0 ; i < count ; i++)
						{
							Time	time = value_list[i].GetTime();
							cout <<  time.ToString() << " : " << value_list[i].ToString() << endl;
						}

						delete [] value_list;
					}
				}
				else if (IsCorrectOption(_arguments[2], "enable"))
				{
					ret_value = endpoint->SetEnable(true);	
					if (ret_value == RET_VALUE_OK)
					{
						cout << "The endpoint[" << endpoint->GetID() << "] is enabled." << endl;
					}
					else
					{
						cout << "The endpoint[" << endpoint->GetID() << "] is failed to enable." << endl;
					}
				}
				else if (IsCorrectOption(_arguments[2], "disable"))
				{
					ret_value = endpoint->SetEnable(false);	
					if (ret_value == RET_VALUE_OK)
					{
						cout << "The endpoint[" << endpoint->GetID() << "] is disabled." << endl;
					}
					else
					{
						cout << "The endpoint[" << endpoint->GetID() << "] is failed to disable." << endl;
					}
				}
				else if (IsCorrectOption(_arguments[2], "activate"))
				{
					ret_value = endpoint->Activation();	
					if (ret_value == RET_VALUE_OK)
					{
						cout << "The endpoint[" << endpoint->GetID() << "] is activated." << endl;
					}
					else
					{
						cout << "The endpoint[" << endpoint->GetID() << "] is failed to activation." << endl;
					}
				}
				else if (IsCorrectOption(_arguments[2], "deactivate"))
				{
					ret_value = endpoint->Deactivation();	
					if (ret_value == RET_VALUE_OK)
					{
						cout << "The endpoint[" << endpoint->GetID() << "] is deactivated." << endl;
					}
					else
					{
						cout << "The endpoint[" << endpoint->GetID() << "] is failed to deactivation." << endl;
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
		cout << "Not supported command : ";

		for(uint32_t i = 0 ; i < _count ; i++)
		{
			cout << _arguments[i] << " ";	
		}

		cout << endl;
	}


	return	ret_value;
}

ShellCommand<ObjectManager>	object_manager_command_endpoint =
{
	.name		= "endpoint",
	.help		= "<command>\n"
				"\tManagement of endpoint.\n"
				"COMMANDS:\n"
				"\tcreate <TYPE> <DEVICE_ID> [OPTIONS]\n"
				"OPTIONS:\n"
				"\t--id <ID>\n"
				"\t\tUnique identifier\n"
				"\t--name <NAME>\n"
				"\t\tName\n"
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

