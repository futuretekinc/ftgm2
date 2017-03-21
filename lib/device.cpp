#include <iostream>
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
	{ Device::TYPE_FTE, 		"fte"	},
	{ Device::TYPE_UNKNOWN, 	""		}
};

#if 0
Device::Properties::Properties()
{
	ostringstream	buffer;
	Time			time = Time::GetCurrentTime();

	buffer << time.Milliseconds();

	type	=	TYPE_UNKNOWN;
	id		=	"dev-" + buffer.str();
	name	=	id;
	enable	=	false;
}
#endif

Device::Properties::Properties(Type _type)
{
	ostringstream	buffer;
	Time			time = Time::GetCurrentTime();

	buffer << time.Milliseconds();

	type	=	_type;
	id		=	"dev-" + buffer.str();
	name	=	id;
	enable	=	false;
}

#if 0
Device::Properties::Properties
(
	const Properties& _properties
)
{
	type	=	_properties.type;
	id		=	_properties.id;
	name	=	_properties.name;
	enable	=	_properties.enable;

	auto it = _properties.endpoint_list.cbegin();
	while(it != _properties.endpoint_list.end())
	{
		endpoint_list.push_back(*it);

		it++;
	}
}

Device::Properties::Properties
(
	const JSONNode&	_node
)
{
	ostringstream	buffer;
	Time			time = Time::GetCurrentTime();

	buffer << time.Milliseconds();

	INFO(NULL, "The Device properties created.");

	type	= TYPE_UNKNOWN;
	id		= "dev-" + buffer.str();
	name	= id;
	enable	= false;

	Set(_node);
}
#endif

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

	switch(type)
	{
	case	Device::TYPE_FTE:	
		{
			INFO(NULL, "Device FTE properties created." );
			properties = new DeviceFTE::Properties;
		}
		break;

	default:
		break;
	}

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
	Kompex::SQLiteStatement*	_statement
)
{
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

Device::PropertiesList::~PropertiesList()
{
	list<Properties*>::iterator it = this->begin();
	while(it != this->end())
	{
		delete (*it);
		it++;	
	}
}

uint32	Device::Properties::GetOptions
(
	uint8_t *options, 
	uint32 options_len
)
{
	return	0;
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

	activation_			= false;
	schedule_thread_ 	= NULL;

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

		if (IsRun())
		{
			if (properties_->enable == true)
			{
				Activation();
			}
			else if (properties_->enable == false)
			{
				Deactivation();
			}
		}
	}	

	return	RET_VALUE_OK;
}

bool		Device::GetActivation()
{
	return	activation_;	
}

RetValue	Device::SetActivation
(
	bool	_activation
)
{
	if (_activation)
	{
		return	Activation();	
	}
	else
	{
		return	Deactivation();	
	}
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
	else if (_name == "activation")
	{
		ret_value = SetActivation(_value);
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

RetValue	Device::SetProperties
(
	const Properties& _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;

	properties_->name 	= _properties.name;
	properties_->id 	= _properties.id;
	properties_->enable	= _properties.enable;
	activation_ 		= false;
	schedule_thread_ 	= NULL;

	return	ret_value;
}

RetValue Device::SetProperties
(
	const Properties* _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;

	properties_->name 	= _properties->name;
	properties_->id 	= _properties->id;
	properties_->enable	= _properties->enable;
	activation_ 		= false;
	schedule_thread_ 	= NULL;

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
	Activation();
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

RetValue	Device::Activation()
{
	RetValue	ret_value = RET_VALUE_OK;

	if (properties_->enable)
	{
		if (!activation_)
		{
			schedule_thread_ = new thread(DeviceScheduleProcess, this);
			if (schedule_thread_ == NULL)
			{
				ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;
				ERROR(this, ret_value, "The device[%s] can not be activated!", properties_->id.c_str());
			}
			else
			{
				while(!schedule_thread_->joinable())
				{
					usleep(1000);
				}

				INFO(this, "New thread[0x%08x] created." , schedule_thread_->get_id());
				activation_ = true;
			}
		}
	}
	return	ret_value;
}

RetValue	Device::Deactivation()
{
	RetValue	ret_value = RET_VALUE_OK;

	if (activation_)
	{
		schedule_stop_ = true;

		schedule_thread_->join();

		delete schedule_thread_;
		schedule_thread_ = NULL;

		activation_ = false;
	}

	return	ret_value;
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
	case	Message::TYPE_STARTED:
		{
			if (activation_)
			{
				Activation();	
			}
		}
		break;

	case	Message::TYPE_STOP:
		{
			if (activation_)
			{
				Deactivation();	
			}
		}
		break;

	default:
		MessageProcess::OnMessage(_message);
	}
}

void	Device::OnActivation
(
	Message *_message
)
{
	Activation();
}

void	Device::OnDeactivation
(
	Message *_message
)
{
	Deactivation();
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
					if (endpoint->GetActivation())
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
					else
					{
						//INFO(NULL, "The endpoint[%s] is deactivated.", endpoint->GetID().c_str());
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
	case	Device::TYPE_FTE:
		device = new DeviceFTE();
		break;

	default:
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

	switch(_properties->type)
	{
	case	Device::TYPE_FTE:
		device = new DeviceFTE;
		break;

	default:
		device = NULL;
	}

	if (device != NULL)
	{
		device->SetProperties(*(DeviceFTE::Properties *)_properties);
	}

	return	device;
}

ostream& operator<<
(
	ostream& _os, 
	const Device& _device
)
{
	_os << "Name : " << _device.properties_->name << endl;
	_os << "  ID : " << _device.properties_->id << endl;

	return	_os;
}

