#include <iostream>
#include <iomanip>
#include "object_manager.h"
#include "device.h"
#include "endpoint.h"
#include "trace.h"
#include "KompexSQLiteException.h"

using namespace std;


struct	EndpointTypeInfo
{
	Endpoint::Type	type;
	const string name;
}	kEndpointTypeInfoList[] = 
{
	{ Endpoint::TEMPERATURE_SENSOR, 		"temperature"	},
	{ Endpoint::HUMIDITY_SENSOR, 			"humidity"	},
	{ Endpoint::VOLTAGE_SENSOR, 			"voltage"	},
	{ Endpoint::CURRENT_SENSOR, 			"Current"	},
	{ Endpoint::DO_CONTROL,	"digital_output"		},
	{ Endpoint::DO_CONTROL,	"do"		},
	{ Endpoint::UNKNOWN, 	""		}
};

Endpoint::Properties::Properties
(
	Type _type
) 
: type(_type)
{
	ostringstream	buffer;
	Time			time = Time::GetCurrentTime();

	buffer << time.Microseconds();

	id		= "ep-" + buffer.str();
	index 	= 0;
	name 	= "";
	enable 	= false;
	device_id="";
	update_interval = 1000;
	value_count = 100;
}

Endpoint::Properties::~Properties()
{
}

Endpoint::Properties*	Endpoint::Properties::Create
(
	Type _type
)
{
	switch(_type)
	{
	case	TEMPERATURE_SENSOR:	return	new	EndpointSensorTemperature::Properties;
	case	HUMIDITY_SENSOR:	return	new EndpointSensorHumidity::Properties;
	case	VOLTAGE_SENSOR: 	return	new EndpointSensorVoltage::Properties;
	case	CURRENT_SENSOR: 	return	new EndpointSensorCurrent::Properties;
	case	DO_CONTROL:			return	new EndpointControlDigitalOutput::Properties;
	default	:
		return	NULL;
	}
}

Endpoint::Properties*	Endpoint::Properties::Create
(
	const Properties* _properties
)
{
	Properties*	new_properties = Create(_properties->type);

	if (new_properties != NULL)
	{
		new_properties->index = _properties->index;
		new_properties->id = _properties->id;
		new_properties->name = _properties->name;
		new_properties->device_id = _properties->device_id;
		new_properties->enable = _properties->enable;
		new_properties->update_interval = _properties->update_interval;
		new_properties->value_count = _properties->value_count;
	}

	return	new_properties;
}

Endpoint::Properties*	Endpoint::Properties::Create
(
	const JSONNode& _node
)
{
	Endpoint::Type	type = UNKNOWN;

	if (_node.type() != JSON_NODE)
	{
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

	Endpoint::Properties*	properties = Create(type);
	if (properties != NULL)
	{
		properties->Set(_node);
	}

	return	properties;
}

RetValue Endpoint::Properties::Set
(
	const Properties* _properties
)
{
	index	= _properties->index;
	id 		= _properties->id;
	name 	= _properties->name;
	enable 	= _properties->enable;
	device_id=_properties->device_id;
	update_interval = _properties->update_interval;
	value_count 	= _properties->value_count;

	return	RET_VALUE_OK;
}

RetValue	Endpoint::Properties::Create
(
	const JSONNode& _node, 
	list<Properties*>& _properties_list
)
{
	RetValue ret_value = RET_VALUE_OK;

	if(_node.type() == JSON_ARRAY)
	{
		for(size_t i = 0 ; i < _node.size() ; i++)
		{
			Properties*	properties;

			properties = Create(_node[i]);	
			if (properties != NULL)
			{
				_properties_list.push_back(properties);
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FORMAT;
	}

	
	return	ret_value;
}

RetValue Endpoint::Properties::Set
(
	const	JSONNode&	_node
)
{
	RetValue ret_value = RET_VALUE_OK;

	if (_node.type() == JSON_NODE)
	{
		for(size_t i = 0 ; i < _node.size() ; i++)
		{
			ret_value = Set(_node[i]);
		}
	}
	else
	{
		if (strcasecmp(_node.name().c_str(), "index") == 0)
		{
			if (_node.type() == JSON_NUMBER)
			{
				index = _node.as_int();
			}
			else
			{
				ret_value = RET_VALUE_INVALID_FIELD;
			}
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
			}
		}
		else if (strcasecmp(_node.name().c_str(), "device_id") == 0)
		{
			if (_node.type() == JSON_STRING)
			{
				device_id = _node.as_string();
			}
			else
			{
				ret_value = RET_VALUE_INVALID_FIELD;
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
						(strcasecmp(_node.as_string().c_str(), "yes") == 0) || 
						(strcasecmp(_node.as_string().c_str(), "on") == 0) || 
						(strcasecmp(_node.as_string().c_str(), "1") == 0))
				{
					enable = true;	
				}
				else if ((strcasecmp(_node.as_string().c_str(), "false") == 0) || 
						(strcasecmp(_node.as_string().c_str(), "no") == 0) || 
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
		else if (strcasecmp(_node.name().c_str(), "update_interval") == 0)
		{
			if (_node.type() == JSON_NUMBER)
			{
				update_interval = _node.as_int();
			}
			else
			{
				ret_value = RET_VALUE_INVALID_FIELD;
			}
		}
		else if (strcasecmp(_node.name().c_str(), "value_count") == 0)
		{
			if (_node.type() == JSON_NUMBER)
			{
				value_count = _node.as_int();
			}
			else
			{
				ret_value = RET_VALUE_INVALID_FIELD;
			}
		}
	}

	return	ret_value;
}

RetValue	Endpoint::Properties::Set
(
	const Kompex::SQLiteStatement*	_statement
)
{
	ASSERT(_statement != NULL);

	RetValue	ret_value = RET_VALUE_OK;

	try
	{	
		index 			= _statement->GetColumnInt("_index");
		enable 			= _statement->GetColumnInt("_enable");
		id  			= _statement->GetColumnString("_id");
		name 			= _statement->GetColumnString("_name");
		device_id 		= _statement->GetColumnString("_device_id");
		update_interval	= _statement->GetColumnInt("_update_interval");
		value_count 	= _statement->GetColumnInt("_value_count");
	}
	catch (Kompex::SQLiteException &exception)
	{
		ret_value = RET_VALUE_DB_ENTRY_DOES_NOT_EXIST;
		ERROR(NULL, ret_value, "Database entry does not exist.");
		return	ret_value;
	}

	return	ret_value;
}

RetValue	Endpoint::Properties::SetProperty
(
	const std::string& _name, 
	uint32_t	_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (_name == "update_interval")
	{
		update_interval = _value;	
	}
	else if (_name == "value_count")
	{
		value_count = _value;	
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FIELD;
	}

	return	ret_value;
}

RetValue	Endpoint::Properties::SetProperty
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

RetValue	Endpoint::Properties::SetProperty
(
	const std::string& _name, 
	const std::string& _value
)
{
	if (_name == "name")
	{
		name = _value;	
	}
	else if (_name == "device_id")
	{
		device_id = _value;	
	}
	else
	{
		return	RET_VALUE_INVALID_FIELD;
	}

	return	RET_VALUE_OK;
}

RetValue	Endpoint::Properties::SetProperty
(
	const std::string& _name, 
	double	_value
)
{
	return	RET_VALUE_INVALID_FIELD;
}

uint32	Endpoint::Properties::GetOptionsSize()
{	
	return	0;	
};

uint32	Endpoint::Properties::GetOptions
(
	uint8_t *options, 
	uint32_t options_len
)
{
	return	0;
}

////////////////////////////////////////////////////
// Class Endpoint
////////////////////////////////////////////////////
Endpoint::Endpoint
(
	Type	_type
)
:	activation_(false)
{
	properties_ = Properties::Create(_type);

	INFO(this, "The endpoint[%s] was created.", properties_->id.c_str());
}

Endpoint::Endpoint
(	
	const Properties& _properties
)
:	activation_(false)
{
	properties_ = Properties::Create(_properties.type);

	properties_->Set(&_properties);
	INFO(this, "The endpoint[%s] was created.", properties_->id.c_str());
}

Endpoint::Endpoint
(	
	const Properties* _properties
)
:	activation_(false)
{
	properties_ = Properties::Create(_properties->type);

	properties_->Set(_properties);
	INFO(this, "The endpoint[%s] was created.", properties_->id.c_str());
}

Endpoint::Endpoint
(
	Type	_type,
	string	_id
)
:	activation_(false)
{
	properties_ = Properties::Create(_type);
	properties_->id = _id;
	properties_->name = _id;
	INFO(this, "The endpoint[%s] was created.", properties_->id.c_str());
}

Endpoint::~Endpoint()
{
	delete properties_;
}

const 
std::string&	Endpoint::GetName()
{	
	return	properties_->name;	
}

RetValue Endpoint::SetName
(
	const std::string& _name
)	
{	
	properties_->name = _name; 

	return	RET_VALUE_OK;
}

uint32		Endpoint::GetIndex()
{	
	return	properties_->index;	
}

RetValue	Endpoint::SetIndex
(
	uint32_t _index
)
{
	properties_->index = _index;

	return	RET_VALUE_OK;
}

const
std::string&	Endpoint::GetDeviceID()
{	
	return	properties_->device_id;	
}

RetValue	Endpoint::SetDeviceID
(
	const std::string& _device_id
)
{
	properties_->device_id = _device_id;

	return	RET_VALUE_OK;
}

bool		Endpoint::GetEnabled()
{	
	return	properties_->enable;
}

RetValue	Endpoint::SetEnable
(
	bool _enable
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (properties_->enable != _enable)
	{
		properties_->enable = _enable;

		ret_value = object_manager_->SaveEndpoint(this);
		if (ret_value != RET_VALUE_OK)
		{
			ERROR(this, ret_value, "Failed to set properties!");	
		}
		else
		{
			if (properties_->enable == true)
			{
				Start();
			}
			else if (properties_->enable == false)
			{
				Stop();
			}

			if (properties_->enable)
			{
				INFO(this, "The endpoint[%s] was enabled.", properties_->id.c_str());
			}
			else
			{
				INFO(this, "The endpoint[%s] was disabled.", properties_->id.c_str());
			}
		}
	}	

	return	ret_value;
}

uint32		Endpoint::GetUpdateInterval()
{	
	return	properties_->update_interval;
}

RetValue	Endpoint::SetUpdateInterval
(
	uint32	_interval
)
{
	properties_->update_interval = _interval;

	return	RET_VALUE_OK;
}

uint32		Endpoint::GetMaxValueCount()
{	
	return	properties_->value_count;	
}

RetValue	Endpoint::SetMaxValueCount
(
	uint32 _count
)
{
	properties_->value_count = _count;

	while(value_list_.size() > properties_->value_count)
	{
		value_list_.pop_front();
	}

	return	RET_VALUE_OK;
}

RetValue	Endpoint::SetProperty
(
	const std::string& _name, 
	const std::string& _value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (_name == "name")
	{
		properties_->name = _value;	
	}
	else if (_name == "device_id")
	{
		properties_->device_id = _value;	
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FIELD;
		ERROR(this, ret_value, "Failed to set %s", _name.c_str());
	}

	return	ret_value;
}

RetValue	Endpoint::SetProperty
(
	const std::string& _name, 
	bool _value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (_name == "enable")
	{
		properties_->enable = _value;	
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FIELD;
		ERROR(this, ret_value, "Failed to set %s", _name.c_str());
	}

	return	ret_value;
}

RetValue	Endpoint::SetProperty
(
	const std::string& _name, 
	uint32_t _value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (_name == "update_interval")
	{
		properties_->update_interval = _value;	
	}
	else if (_name == "value_count")
	{
		properties_->value_count = _value;	
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FIELD;
		ERROR(this, ret_value, "Failed to set %s", _name.c_str());
	}

	return	ret_value;
}

RetValue	Endpoint::SetProperty
(
	const std::string& _name, 
	double	_value
)
{
	return	RET_VALUE_INVALID_FIELD;
}

RetValue	Endpoint::Start()
{
	RetValue 	ret_value = RET_VALUE_OK;

	INFO(this, "Activation of the endpoint[%s] has been requested.", properties_->id.c_str());
	if (properties_->enable)
	{
		if (!activation_)
		{
			if (object_manager_ == NULL)
			{
				ret_value = RET_VALUE_OBJECT_IS_NOT_INITIALIZED;
			}
			else
			{
				Device*	device = object_manager_->GetDevice(properties_->device_id);
				if (device == NULL)
				{
					ret_value = RET_VALUE_NOT_ATTACHED_TO_DEVICE;
					ERROR(this, ret_value, "Failed to get device[%s].", properties_->device_id.c_str());
				}
				else
				{
					if (device->IsRun())
					{
						ret_value = device->Connect(properties_->id);
						if (ret_value == RET_VALUE_OK)
						{
							activation_ = true;
							INFO(this, "The endpoint[%s] was activated.", properties_->id.c_str());
						}
					}
					else
					{
						ret_value = RET_VALUE_DEVICE_NOT_ACTIVATED;
					}
				}
			}
		}
	}

	return	ret_value;	
}

RetValue	Endpoint::Stop()
{
	RetValue 	ret_value = RET_VALUE_OK;

	INFO(this, "Deactivation of the endpoint[%s] has been requested.", properties_->id.c_str());
	if (activation_)
	{
		if (object_manager_ == NULL)
		{
			ret_value = RET_VALUE_OBJECT_IS_NOT_INITIALIZED;
		}
		else
		{
			Device*	device = object_manager_->GetDevice(properties_->device_id);
			if (device == NULL)
			{
				ret_value = RET_VALUE_NOT_ATTACHED_TO_DEVICE;
				ERROR(this, ret_value, "Failed to get device[%s].", properties_->device_id.c_str());
			}
			else
			{
				ret_value = device->Disconnect(properties_->id);
				if ((ret_value == RET_VALUE_OK) || (ret_value == RET_VALUE_OBJECT_NOT_FOUND))
				{
					activation_ = false;
					INFO(this, "The endpoint[%s] was deactivated.", properties_->id.c_str());
				}
			}
		}
	}

	return	ret_value;
}

RetValue	Endpoint::Synchronize()
{
	Device*	device;

	INFO(this, "The endpoint[%s] sync!", properties_->id.c_str());
	device = object_manager_->GetDevice(properties_->device_id);
	if (device == NULL)
	{
		return	RET_VALUE_OBJECT_NOT_FOUND;
	}

	return	device->GetEndpointValue(this);
}


Endpoint::Properties*	Endpoint::GetProperties()
{
	return	properties_;
}

uint32	Endpoint::ValueCount()
{
	return	value_list_.size();
}

RetValue	Endpoint::AddValue
(
	const TimedValue& _value
)
{
	Lock();

	INFO(this, "Add new value!");
	if (value_list_.size() >= properties_->value_count)
	{
		value_list_.pop_front();
	}

	value_list_.push_back(_value);

	Unlock();

	return	RET_VALUE_OK;
}

RetValue	Endpoint::DeleteValue
(
	uint32	_count
)
{
	Lock();

	for(; (_count > 0) && (value_list_.size() > 0) ; _count--)
	{
		value_list_.pop_front();	
	}

	Unlock();

	return	RET_VALUE_OK;
}

TimedValue	Endpoint::GetValue()
{
	TimedValue	value;

	Lock();

	list<TimedValue>::reverse_iterator	it = value_list_.rbegin();
	if (it != value_list_.rend())
	{
		value = (*it);
	}

	Unlock();

	return	value;
}

TimedValue	Endpoint::GetValue
(
	uint32 _index
)
{
	TimedValue	value;

	Lock();

	list<TimedValue>::iterator	it = value_list_.begin();
	if (value_list_.size() > _index)
	{
		for(; (_index > 0) && (value_list_.size() > 0) ; _index--, it++)
		{
		}
	
		value = (*it);
	}

	Unlock();

	return	value;
}
		
uint32	Endpoint::GetValueList
(
	TimedValue*	_value_list,
	uint32	_max_count
)
{
	uint32	count = 0;
	Lock();
	
	list<TimedValue>::iterator	it = value_list_.begin();
	for(; (count < _max_count) && (it != value_list_.end()) ; count++, it++)
	{
		_value_list[count] = (*it);
	}
	
	Unlock();

	return	count;
}

const string&	Endpoint::TypeToString
(
	Type	_type
)
{
	struct	EndpointTypeInfo *type_info = kEndpointTypeInfoList;

	while(type_info->type != Endpoint::UNKNOWN)
	{
		if (type_info->type == _type)
		{
			break;
		}

		type_info++;
	}

	return	type_info->name;
}

Endpoint::Type	Endpoint::StringToType
(
	const string& _name
)
{
	struct	EndpointTypeInfo *type_info = kEndpointTypeInfoList;

	while(type_info->type != Endpoint::UNKNOWN)
	{
		if (type_info->name.compare(_name) == 0)
		{
			return	type_info->type;
		}

		type_info++;
	}

	return	Endpoint::UNKNOWN;
}

RetValue	Endpoint::Set
(
	const Properties* _properties
)
{
	return	properties_->Set(_properties);
}

RetValue	Endpoint::Set
(
	const	JSONNode&	_node
)
{
	RetValue ret_value = RET_VALUE_OK;

	if (_node.type() == JSON_NODE)
	{
		for(size_t i = 0 ; i < _node.size() ; i++)
		{
			ret_value = Set(_node[i]);
			if (ret_value != RET_VALUE_OK)
			{
				break;
			}
		}
	}
	else 
	{
		if (_node.name() == "name")
		{
			if ((_node.type() != JSON_STRING) && (_node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "Failed to set property[%s].", _node.name().c_str());	
			}
			else
			{
				properties_->name = _node.as_string();
			}
		}
		else if (_node.name() == "id")
		{
			if ((_node.type() != JSON_STRING) && (_node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "Failed to set property[%s].", _node.name().c_str());	
			}
			else
			{
				properties_->id = _node.as_string();
			}
		}
		else if (_node.name() == "deviceproperties_->id")
		{
			if ((_node.type() != JSON_STRING) && (_node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "Failed to set property[%s].", _node.name().c_str());	
			}
			else
			{
				properties_->id = _node.as_string();
			}
		}
		else if (_node.name() == "enable")
		{
			if ((_node.type() != JSON_NUMBER) && (_node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "Failed to set property[%s].", _node.name().c_str());	
			}
			else
			{
				properties_->enable = _node.as_int();
			}
		}
		else if (_node.name() == "update_interval")
		{
			if ((_node.type() != JSON_NUMBER) && (_node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "Failed to set property[%s].", _node.name().c_str());	
			}
			else
			{
				properties_->update_interval = _node.as_int();
			}
		}
		else if (_node.name() == "value_count")
		{
			if ((_node.type() != JSON_NUMBER) && (_node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "Failed to set property[%s].", _node.name().c_str());	
			}
			else
			{
				properties_->value_count = _node.as_int();
			}
		}
	}

	return	ret_value;
}

RetValue	Endpoint::Set
(
	const Kompex::SQLiteStatement*	_statement
)
{
	ASSERT(_statement != NULL);

	try
	{	
		properties_->index 			= _statement->GetColumnInt("_index");
		properties_->id 			= _statement->GetColumnInt("properties_->id");
		properties_->name 			= _statement->GetColumnInt("_name");
		properties_->id 			= _statement->GetColumnInt("_deviceproperties_->id");
		properties_->enable 		= _statement->GetColumnInt("_enable");
		properties_->update_interval= _statement->GetColumnInt("_update_interval");
		properties_->value_count 	= _statement->GetColumnInt("_value_count");
	}
	catch (Kompex::SQLiteException &exception)
	{
		return	RET_VALUE_DB_ENTRY_DOES_NOT_EXIST;
	}

	return	RET_VALUE_OK;
}

void	Endpoint::ReleaseParent()
{
	if (parent_ != NULL)
	{
		parent_ = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// Global function
//////////////////////////////////////////////////////////////////////////
Endpoint* 	Endpoint::Create
(
	Endpoint::Type _type
)
{
	switch(_type)
	{
	case	Endpoint::TEMPERATURE_SENSOR:	return	new	EndpointSensorTemperature;
	case	Endpoint::HUMIDITY_SENSOR:		return	new EndpointSensorHumidity;
	case	Endpoint::VOLTAGE_SENSOR: 		return	new EndpointSensorVoltage;
	case	Endpoint::CURRENT_SENSOR: 		return	new EndpointSensorCurrent;
	case	Endpoint::DO_CONTROL:			return	new EndpointControlDigitalOutput;
	default	:
		return	NULL;
	}
}

Endpoint* 	Endpoint::Create
(
	const Endpoint::Properties* properties
)
{
	INFO(NULL, "Create Endpoint : id - %s, name - %s", properties->id.c_str(), properties->name.c_str());
	Endpoint* endpoint = Endpoint::Create(properties->type);

	if (endpoint != NULL)
	{
		endpoint->Set(properties);
	}

	return	endpoint;
}

ostream& operator<<
(
	ostream& _os, 
	const Endpoint& _ep
)
{
	_os << "Name : " << _ep.properties_->name << endl;
	_os << "  ID : " << _ep.properties_->id << endl;

	return	_os;
}
