#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <sys/time.h>
#include <ostream>
#include <iomanip>
#include "timer.h"
#include "trace.h"
#include "object_manager.h"
#include "device.h"
#include "device_snmp.h"
#include "device_fte.h"
#include "endpoint.h"

using namespace std;

struct	Device::TypeInfo kDeviceTypeInfoList[] = 
{
	{ Device::TYPE_SNMP, 		"snmp"	},
	{ Device::TYPE_FTE, 		"fte"	},
	{ Device::TYPE_UNKNOWN, 	""		}
};

Device::Properties::Properties(Type _type)
{
	ostringstream	buffer;
	Time			time = Time::GetCurrentTime();

	buffer << "dev-" << time.Microseconds();
 
  	type    =       _type;
  	id      =  	buffer.str();
  	name    =  	buffer.str();
  	enable  =	false;
}

Device::Properties::~Properties()
{
}

Device::Properties*	Device::Properties::Create
(
	Type	_type
)
{
	Properties*	properties = NULL;

	switch(_type)
	{
	case	Device::TYPE_SNMP:
		{
			INFO(NULL, "Device SNMP properties created." );
			properties = new DeviceSNMP::Properties();
		}
		break;

	case	Device::TYPE_FTE:	
		{
			INFO(NULL, "Device FTE properties created." );
			properties = new DeviceFTE::Properties();
		}
		break;

	default:
		break;
	}

	return	properties;
}

Device::Properties*	Device::Properties::Create
(
	const	JSONNode&	_node
)
{
	Properties*	properties = NULL;
	Type	type = TYPE_UNKNOWN;

	if (_node.type() != JSON_NODE)
	{
		cout << "Json node is not node"<< endl;
		return	NULL;
	}

	for(size_t i = 0 ; i < _node.size() ; i++)
	{
		if (strcasecmp(_node[i].name().c_str(), "type") == 0)
		{
			type = StringToType(_node[i].as_string());
			break;
		}
	}

	properties = Create(type);
	if (properties != NULL)
	{
		properties->Set(_node);
	}
	else
	{
		ERROR(NULL, RET_VALUE_INVALID_TYPE,  "Unknown device type[%d]!", type);
	}

	return	properties;
}

RetValue Device::Properties::Set
(
	Properties* _properties
)
{
	
	id = _properties->id;
	name = _properties->name;
	enable = _properties->enable;

	return	RET_VALUE_OK;
}

RetValue Device::Properties::Set
(
	Kompex::SQLiteStatement*	_statement
)
{
	INFO(NULL, "Device::Properties::Set");
	id 		= _statement->GetColumnString("_id");
	name 	= _statement->GetColumnString("_name");
	enable 	= _statement->GetColumnInt("_enable");

	return	RET_VALUE_OK;
}

RetValue Device::Properties::Set
(
	const JSONNode&	_node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (strcasecmp(_node.name().c_str(), "type") == 0)
	{

	}
	else if (strcasecmp(_node.name().c_str(), "name") == 0)
	{
		if (_node.type() == JSON_STRING)
		{
			name = _node.as_string();
		}
		else
		{
			ret_value = RET_VALUE_INVALID_FIELD;
			ERROR(NULL, ret_value, "Failed to set enable!");
		}
	}
	else if (strcasecmp(_node.name().c_str(), "id") == 0)
	{
		if (_node.type() == JSON_STRING)
		{
			id = _node.as_string();
		}
		else
		{
			ret_value = RET_VALUE_INVALID_FIELD;
			ERROR(NULL, ret_value, "Failed to set enable!");
		}
	}
	else if (strcasecmp(_node.name().c_str(), "enable") == 0)
	{
		if (_node.type() == JSON_NUMBER)
		{
			enable = (_node.as_int() != 0);
		}
		else if (_node.type() == JSON_STRING)
		{
			if ((strcasecmp(_node.as_string().c_str(), "true") == 0) || 
					(strcasecmp(_node.as_string().c_str(), "on") == 0) || 
					(strcasecmp(_node.as_string().c_str(), "1") == 0))
			{
				enable = true;	
			}
			else if ((strcasecmp(_node.as_string().c_str(), "false") == 0) || 
					(strcasecmp(_node.as_string().c_str(), "off") == 0) || 
					(strcasecmp(_node.as_string().c_str(), "0") == 0))
			{
				enable = false;
			}
			else
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(NULL, ret_value, "Failed to set enable!");
			}
		}
		else
		{
			ret_value = RET_VALUE_INVALID_FIELD;
			ERROR(NULL, ret_value, "Failed to set enable!");
		}

	}
	else if (strcasecmp(_node.name().c_str(), "endpoints") == 0)
	{
		if(_node.type() == JSON_ARRAY)
		{
			for(int i = 0 ; i < (int)_node.size() ; i++)
			{
				if(_node[i].type() != JSON_STRING)
				{
					ret_value = RET_VALUE_INVALID_FIELD;		
					break;
				}

				endpoint_list.push_back(_node[i].as_string());
			}
		}
		else
		{
			ret_value = RET_VALUE_INVALID_TYPE;
		}

	}
	else if (strcasecmp(_node.name().c_str(), "endpoint") == 0)
	{
		if(_node.type() == JSON_STRING)
		{
			endpoint_list.push_back(_node.as_string());
		}
		else
		{
			ret_value = RET_VALUE_INVALID_FIELD;		
		}
	}
	else if(_node.type() == JSON_NODE)
	{
		for(int i = 0 ; i < (int)_node.size() ; i++)
		{
			Set(_node[i]);	
		}
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FIELD;
		ERROR(NULL, ret_value, "Failed to set device properties[%s]!", _node.name().c_str());
	}

	return	ret_value;
}

RetValue	Device::Properties::SetProperty
(
	const std::string& _name, 
	uint32_t	_value
)
{
	return	RET_VALUE_INVALID_FIELD;
}

RetValue	Device::Properties::SetProperty
(
	const std::string& _name, 
	bool _value
)
{
	if (_name == "enable")
	{
		enable = _value;	
	}
	else
	{
		return	RET_VALUE_INVALID_FIELD;
	}

	return	RET_VALUE_OK;
}

RetValue	Device::Properties::SetProperty
(
	const std::string& _name, 
	const std::string& _value
)
{
	if (_name == "name")
	{
		name = _value;	
	}
	else
	{
		return	RET_VALUE_INVALID_FIELD;
	}

	return	RET_VALUE_OK;
}

uint32	Device::Properties::GetOptions
(
	uint8_t *options, 
	uint32 options_len
)
{
	return	0;
}

uint32	Device::Properties::GetOptions
(
	char *buffer, 
	uint32 buffer_len
)
{
	return	0;
}

JSONNode	Device::Properties::JSON()
{
	JSONNode	root;

	root.push_back(JSONNode("type", Device::TypeToString(type)));
	root.push_back(JSONNode("id", id));
	root.push_back(JSONNode("name", name));
	root.push_back(JSONNode("enable", enable));

	return	root;
}

JSONNode	Device::Properties::JSON(JSONNode& root)
{
	root.push_back(JSONNode("type", Device::TypeToString(type)));
	root.push_back(JSONNode("id", id));
	root.push_back(JSONNode("name", name));
	root.push_back(JSONNode("enable", enable));

	return	root;
}

///////////////////////////////////////////////////////////////////////////////////
// Device Class
///////////////////////////////////////////////////////////////////////////////////
Device::Device
(
	Type _type
)
: MessageProcess()
{
	ostringstream	buffer;

	properties_ = Properties::Create(_type);

	schedule_thread_ 	= NULL;
	schedule_stop_		= true;

	INFO(this, "Device[%s] created.", properties_->id.c_str());
}

Device::~Device()
{
	INFO(this, "Device[%s] destroy.", properties_->id.c_str());

	delete properties_;
}

Device::Type	Device::GetType()
{
	return	properties_->type;	
}

const 
std::string&	Device::GetID()
{	
	return	properties_->id;	
}

const 
std::string&	Device::GetName()
{
	return	properties_->name;	
}

RetValue Device::SetName
(
	const string& _name
)	
{	
	properties_->name = _name;	

	return	RET_VALUE_OK;
}

bool	Device::GetEnable()
{
	return	properties_->enable;
};

RetValue 	Device::SetEnable
(
	bool _enable
)
{
	if (properties_->enable != _enable)
	{
		properties_->enable = _enable;

		if (properties_->enable)
		{
			Start();
		}
		else
		{
			Stop();
		}
	}	

	return	RET_VALUE_OK;
}

RetValue	Device::SetProperty
(
	const std::string& _name, 
	const std::string& _value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (_name == "name")
	{
		ret_value = SetName(_value);
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FIELD;
		ERROR(this, ret_value, "Failed to set %s", _name.c_str());
	}

	return	ret_value;
}

RetValue	Device::SetProperty
(
	const std::string& _name, 
	bool _value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (_name == "enable")
	{
		ret_value = SetEnable(_value);
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FIELD;
		ERROR(this, ret_value, "Failed to set %s", _name.c_str());
	}

	return	ret_value;
}

RetValue	Device::SetProperty
(
	const std::string& _name, 
	uint32_t _value
)
{
	return	RET_VALUE_INVALID_FIELD;
}

RetValue Device::SetProperties
(
	const Properties* _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Device::SetProperties called");
	properties_->name 	= _properties->name;
	properties_->id 	= _properties->id;
	properties_->enable	= _properties->enable;

	return	ret_value;
}

RetValue Device::SetProperties
(
	Kompex::SQLiteStatement*	_statement
)
{
	properties_->id 	= _statement->GetColumnString("_id");
	properties_->name 	= _statement->GetColumnString("_name");
	properties_->enable = _statement->GetColumnInt("_enable");

	return	RET_VALUE_OK;
}

RetValue	Device::SetProperties
(
	const	JSONNode&	node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (node.type() == JSON_NODE)
	{
		for(size_t i = 0 ; i < node.size() ; i++)
		{
			SetProperties(node[i]);
		}
	}
	else 
	{
		if (node.name() == "name")
		{
			if ((node.type() != JSON_STRING) && (node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "");	
			}
			else
			{
				properties_->name = node.as_string();
			}
		}
		else if (node.name() == "id")
		{
			if ((node.type() != JSON_STRING) && (node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this,RET_VALUE_INVALID_FIELD, "");	
			}
			else
			{
				properties_->id = node.as_string();
			}
		}
		else if (node.name() == "enable")
		{
			if ((node.type() != JSON_NUMBER) && (node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this,RET_VALUE_INVALID_FIELD, "");	
			}
			else
			{
				properties_->enable = node.as_int();
			}
		}
	}

	return	ret_value;
}

Device::Properties*	Device::GetProperties()
{
	return	properties_;
}

void	Device::PreProcess()
{
	INFO(this, "Activation of the device[%s] has been requested.", properties_->id.c_str());
	if (properties_->enable)
	{
		if (schedule_stop_)
		{
			schedule_thread_ = new thread(DeviceScheduleProcess, this);
			if (schedule_thread_ == NULL)
			{
				ERROR(this, RET_VALUE_NOT_ENOUGH_MEMORY, "The device[%s] can not create schedule thread!", properties_->id.c_str());
			}
			else
			{
				INFO(this, "The device[%s]'s schedules thread has started.", properties_->id.c_str());

				while(!schedule_thread_->joinable())
				{
					usleep(1000);
				}
			}
		}
		else
		{
			INFO(this, "The device[%s]'s scheduler already started.", properties_->id.c_str());	
		}
	}

	if (schedule_stop_ == false)
	{
		for(auto it = properties_->endpoint_list.begin(); it != properties_->endpoint_list.end() ; it++)
		{
			Endpoint* endpoint;

			endpoint = object_manager_->GetEndpoint(*it);	
			if (endpoint != NULL)
			{
				endpoint->Start();	
			}
			else
			{
				ERROR(this, RET_VALUE_OBJECT_NOT_FOUND, "Failed to get endpoint[%s]", (*it).c_str());
			}
		}
	}
}

void	Device::Process()
{
}

void	Device::PostProcess()
{
	if (schedule_thread_ != NULL)
	{
		schedule_stop_ = true;

		schedule_thread_->join();

		delete schedule_thread_;
		schedule_thread_ = NULL;
	}

}

RetValue	Device::Connect
(
	const string&	_endpoint_id
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Endpoint* endpoint;

	endpoint = object_manager_->GetEndpoint(_endpoint_id);
	if (endpoint == NULL)
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
		ERROR(this, ret_value, "Failed to get endpoint[%s].", _endpoint_id.c_str());
	}
	else
	{
		TimeoutTimer	timer(Time::GetCurrentTime());

		timer.Add(endpoint->GetUpdateInterval());

		ret_value = endpoint_scheduler_.Push(timer, _endpoint_id);
		if (ret_value == RET_VALUE_OK)
		{
			INFO(this, "The endpoint[%s] is connected to device[%s].", _endpoint_id.c_str(), properties_->id.c_str());
		}
		else
		{
			INFO(this, "Failed to connect endpoint[%s] to device[%s].", _endpoint_id.c_str(), properties_->id.c_str());
		}
	}

	return	ret_value;
}

RetValue	Device::Disconnect
(
	const string& _endpoint_id
)
{
	RetValue	ret_value = RET_VALUE_OK;


	ret_value = endpoint_scheduler_.Pop(_endpoint_id, true);
	if (ret_value == RET_VALUE_OK)
	{
		INFO(this, "The endpoint[%s] is disconnected from device[%s].", _endpoint_id.c_str(), properties_->id.c_str());
	}
	else
	{
		INFO(this, "Failed to disconnect endpoint[%s] from device[%s].", _endpoint_id.c_str(), properties_->id.c_str());
	}

	return	ret_value;
}

RetValue	Device::GetEndpointValue
(
	Endpoint* _endpoint
)
{
	return	RET_VALUE_OK;
}

RetValue	Device::SetEndpointValue
(
	Endpoint* _endpoint
)
{
	return	RET_VALUE_OK;
}

void	Device::OnMessage
(
	Message *_message
)
{
	INFO(this, "Message Received[%s]", ToString(_message).c_str() );
	switch(_message->type)
	{
	default:
		MessageProcess::OnMessage(_message);
	}
}

void	Device::OnQuit
(
	Message *_message
)
{
	MessageProcess::OnQuit(_message);
}

Device::Type	Device::StringToType
(
	const std::string& _name
)
{
	struct	Device::TypeInfo *type_info = kDeviceTypeInfoList;

	while(type_info->type != Device::TYPE_UNKNOWN)
	{
		if (type_info->name.compare(_name) == 0)
		{
			return	type_info->type;
		}

		type_info++;
	}

	return	Device::TYPE_UNKNOWN;	
}

const 
std::string& Device::TypeToString
(
	Device::Type	_type
)
{
	struct	Device::TypeInfo *type_info = kDeviceTypeInfoList;

	while(type_info->type != Device::TYPE_UNKNOWN)
	{
		if (type_info->type == _type)
		{
			break;
		}

		type_info++;
	}

	return	type_info->name;
}


void Device::DeviceScheduleProcess
(
	Device *_device
)
{
	LoopTimer	loop_timer(100);

	INFO(_device, "The device[%s] Scheduler started!", _device->GetID().c_str());
	_device->schedule_stop_ = false;


	loop_timer.Start();

	while(!_device->schedule_stop_)
	{
		ScheduleItem<std::string>* item = _device->endpoint_scheduler_.Top();
		if (item != NULL)
		{
			if (item->timer.IsExpired())
			{
				_device->endpoint_scheduler_.Pop();

				Endpoint*	endpoint = _device->object_manager_->GetEndpoint(item->data);
				if (endpoint == NULL)
				{
					INFO(_device,"Endpoint not found!");
					delete item;
				}
				else
				{
					if (endpoint->IsRun())
					{
						RetValue	ret_value;

						ret_value = endpoint->Synchronize();
						if (ret_value != RET_VALUE_OK)		
						{
							ERROR(_device, ret_value, "Failed to synchronize endpoint.");
						}

						item->timer.Add(endpoint->GetUpdateInterval());
						_device->endpoint_scheduler_.Push(item);
					}
				}
			}
		}

		loop_timer.WaitingForExpired();
	}

	INFO(_device, "The device[%s] Scheduler stopped!", _device->GetID().c_str());

}

///////////////////////////////////////////////////////
// Global functions
///////////////////////////////////////////////////////
	Device*		Device::Create
(
 Device::Type _type
)
{
	Device* device ;

	switch(_type)
	{
	case	Device::TYPE_SNMP:
		device = new DeviceSNMP(_type);
		break;

	case	Device::TYPE_FTE:
		device = new DeviceFTE;
		break;

	default:
		ERROR(NULL, RET_VALUE_INVALID_TYPE, "Not supported device[%s]!", TypeToString(_type).c_str());
		device = NULL;
	}

	return	device;
}

Device*		Device::Create
(
	const Device::Properties* _properties
)
{
	Device* device ;

	device = Create(_properties->type);
	if (device != NULL)
	{
		device->SetProperties(_properties);
	}

	return	device;
}

void	Device::Show
(
	ostream& _os 
)
{
	_os << setw(16) << "ID" 		<< " : " << properties_->id << endl;
	_os << setw(16) << "Type"	<< " : " << Device::TypeToString(properties_->type) << endl;
	_os << setw(16) << "Name"	<< " : " << properties_->name << endl;
	_os << setw(16) << "Enable" 	<< " : " << properties_->enable << endl;
}

ostream& operator<<
(
	ostream& _os, 
	Device& _device
)
{
	_device.Show(_os);

	return	_os;
}

