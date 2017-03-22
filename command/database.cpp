#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

using namespace std;

RetValue	ShellCommandDatabase
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
)
{
	RetValue	ret_value;
	ObjectManager*	object_manager = _shell->Data();
	DataManager*	data_manager = object_manager->GetDataManager();

	if (object_manager == NULL)
	{
		ret_value = RET_VALUE_OBJECT_MANAGER_NOT_INITIALIZED;	
	}
	else 
	{
		if (_count == 1)
		{
		}
		else if (strcasecmp(_arguments[1].c_str(), "device") == 0)
		{
			if (_count == 2)
			{
				uint32_t	count = 0;

				count = data_manager->GetDeviceCount();
				list<Device::Properties*>	_list;

				_shell->Out() << "Device Count : " << _count << endl;

				ret_value = data_manager->GetDeviceProperties(0, _count, _list);
				if (ret_value == RET_VALUE_OK)
				{
					list<Device::Properties*>::iterator	it = _list.begin();
					while(it != _list.end())
					{
						delete *it;
						it++;	
					}
				}
			}
			else if (strcasecmp(_arguments[2].c_str(), "init") == 0)
			{
				ret_value = data_manager->CreateDeviceTable();	
				if (ret_value != RET_VALUE_OK)
				{
					_shell->Out() << "Failed to create device table." << endl;
				}
			}
		}
		else if (strcasecmp(_arguments[1].c_str(), "endpoint") == 0)
		{
			if (_count == 2)
			{
				uint32_t	count = 0;

				count = data_manager->GetEndpointCount();

				list<Endpoint::Properties*>	_list;

				_shell->Out() << "Endpoint Count : " << _count << endl;

				ret_value = data_manager->GetEndpointProperties(0, _count, _list);
				if (ret_value == RET_VALUE_OK)
				{
					_shell->Out() << setw(4) << setfill(' ');
					_shell->Out() << " " << setw(16) << "Type";
					_shell->Out() << " " << setw(16) << "ID";
					_shell->Out() << " " << setw(4) << "Index";
					_shell->Out() << " " << setw(16) << "Name";
					_shell->Out() << " " << setw(8)  << "Enable";
					_shell->Out() << " " << setw(8) << "Interval";
					_shell->Out() << " " << setw(8) << "Value" << endl;

					uint32_t	index = 0;
					list<Endpoint::Properties*>::iterator	it = _list.begin();
					while(it != _list.end())
					{
						_shell->Out() << setw(4) << ++index;
						_shell->Out() << " " << setw(16) << Endpoint::TypeToString((*it)->type);
						_shell->Out() << " " << setw(16) << (*it)->id;
						_shell->Out() << " " << setw(4) << (*it)->index;
						_shell->Out() << " " << setw(16) << (*it)->name;
						_shell->Out() << " " << setw(8)  << ((*it)->enable?"enabled":"disabled");
						_shell->Out() << " " << setw(8) << (*it)->update_interval;
						_shell->Out() << " " << setw(8) << (*it)->value_count << endl;

						it++;	
					}
				}
			}
			else if (strcasecmp(_arguments[2].c_str(), "init") == 0)
			{
				ret_value = data_manager->CreateEndpointTable();	
				if (ret_value != RET_VALUE_OK)
				{
					_shell->Out() << "Failed to create endpoint table." << endl;
				}
			}
		}
		else if (strcasecmp(_arguments[1].c_str(), "value") == 0)
		{
		
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
