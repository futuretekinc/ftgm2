#include <iostream>
#include "object_manager.h"
#include "device.h"
#include "endpoint.h"
#include "trace.h"

using namespace std;

struct	EndpointTypeInfo
{
	Endpoint::Type	type;
	const string name;
}	kEndpointTypeInfoList[] = 
{
	{ Endpoint::TYPE_SENSOR_TEMPERATURE, 		"temperature"	},
	{ Endpoint::TYPE_SENSOR_HUMIDITY, 			"humidity"	},
	{ Endpoint::TYPE_SENSOR_VOLTAGE, 			"voltage"	},
	{ Endpoint::TYPE_CONTROL_DIGITAL_OUTPUT,	"digital_output"		},
	{ Endpoint::TYPE_CONTROL_DIGITAL_OUTPUT,	"do"		},
	{ Endpoint::TYPE_UNKNOWN, 	""		}
};

Endpoint::Properties::Properties
(
	const Properties& _properties
)
{
	type	= _properties.type;
	index	= _properties.index;
	id 		= _properties.id;
	name 	= _properties.name;
	enable 	= _properties.enable;
	update_interval = _properties.update_interval;
	value_count 	= _properties.value_count;
}

Endpoint::Properties::Properties
(
	const Properties* _properties
)
{
	type	= _properties->type;
	index	= _properties->index;
	id 		= _properties->id;
	name 	= _properties->name;
	enable 	= _properties->enable;
	update_interval = _properties->update_interval;
	value_count 	= _properties->value_count;
}

Endpoint::Properties::Properties
(
	Type _type,
	uint32_t _index
) 
: type(_type), index(_index) 
{
	id = "";
	name = "";
	enable = false;
	update_interval = 1000;
	value_count = 100;
}

Endpoint::Properties::Properties
(
	const JSONNode& _node
)
{
	type 	= TYPE_UNKNOWN;
	index	= 0;
	id 		= "";
	name 	= "";
	enable	= false;
	update_interval = 1000;
	value_count = 100;

	Set(_node);
}

Endpoint::Properties::~Properties()
{
}

Endpoint::Properties*	Endpoint::Properties::Duplicate()
{
	return	new Properties(this);
}

Endpoint::Properties*	Endpoint::Properties::Create
(
	const JSONNode& _node
)
{
	Endpoint::Type	type = TYPE_UNKNOWN;

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

	switch(type)
	{
	case	TYPE_SENSOR_TEMPERATURE:		return	new	EndpointSensorTemperature::Properties(_node);
	case	TYPE_SENSOR_HUMIDITY:			return	new EndpointSensorHumidity::Properties(_node);
	case	TYPE_SENSOR_VOLTAGE: 			return	new EndpointSensorVoltage::Properties(_node);
	case	TYPE_CONTROL_DIGITAL_OUTPUT:	return	new EndpointControlDigitalOutput::Properties(_node);
	default	:
		return	NULL;
	}
}

Endpoint::Properties*	Endpoint::Properties::Create
(
	Type	_type
)
{
	switch(_type)
	{
	case	TYPE_SENSOR_TEMPERATURE:		return	new	EndpointSensorTemperature::Properties();
	case	TYPE_SENSOR_HUMIDITY:			return	new EndpointSensorHumidity::Properties();
	case	TYPE_SENSOR_VOLTAGE: 			return	new EndpointSensorVoltage::Properties();
	case	TYPE_CONTROL_DIGITAL_OUTPUT:	return	new EndpointControlDigitalOutput::Properties();
	default	:
		return	NULL;
	}
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


Endpoint::PropertiesList::~PropertiesList()
{
	list<Properties*>::iterator it = this->begin();
	while(it != this->end())
	{
		delete (*it);
		it++;	
	}
}

////////////////////////////////////////////////////
// Class Endpoint
////////////////////////////////////////////////////
Endpoint::Endpoint
(
	Type	_type
)
:	properties_(_type, 0)
{
	activation_ = false;

	TRACE(this, "The endpoint[%s] was created.", properties_.id.c_str());
}

Endpoint::Endpoint
(	
	const Properties& _properties
)
:	properties_(_properties)
{
	activation_		= false;

	TRACE(this, "The endpoint[%s] was created.", properties_.id.c_str());
}

Endpoint::Endpoint
(	
	const Properties* _properties
)
:	properties_(*_properties)
{
	activation_		= false;

	TRACE(this, "The endpoint(%x)[%s] was created.", this, properties_.id.c_str());
}

Endpoint::Endpoint
(
	Type	_type,
	string	_id
)
:	properties_(_type, 0)
{
	properties_.id 	= _id;
	properties_.name= _id;

	activation_	= false;

	TRACE(this, "The endpoint[%s] was created.", properties_.id.c_str());
}

Endpoint::~Endpoint()
{
	if (parent_ != NULL)
	{
		((Device *)parent_)->Disconnect(this);
	}
}

Endpoint::Type	Endpoint::GetType()
{
	return	properties_.type;	
}

uint32_t		Endpoint::GetIndex()
{
	return	properties_.index;	
}

const
std::string&	Endpoint::SetName
(
	const std::string& _name
)	
{	
	return	properties_.name = _name; 
}

bool		Endpoint::IsEnabled()
{	
	return	properties_.enable;	
}

uint32		Endpoint::UpdateInterval()	
{	
	return	properties_.update_interval;
}

bool		Endpoint::IsActivated()
{
	return	activation_;	
}

uint32_t	Endpoint::MaxValueCount()
{	
	return	properties_.value_count;	
}

const
std::string&	Endpoint::GetID()
{
	return	properties_.id;
}

void	Endpoint::UpdateInterval
(
	uint32_t	_interval
)
{
	properties_.update_interval = _interval;
}

RetValue	Endpoint::SetEnable
(
	bool _enable
)
{
	if (properties_.enable != _enable)
	{
		properties_.enable = _enable;

		Device *device = (Device *)parent_;

		if (device != NULL)
		{
			if (device->IsRun())
			{
				if (properties_.enable == true)
				{
					Activation();
				}
				else if (properties_.enable == false)
				{
					Deactivation();
				}
			}
			else 
			{
				Deactivation();
			}
		}
	}	

	return	RET_VALUE_OK;
}

RetValue	Endpoint::Activation()
{
	RetValue 	ret_value = RET_VALUE_OK;

	if (properties_.enable)
	{
		if (!activation_)
		{
			if (properties_.enable)
			{
				activation_ = true;
			}

			if (parent_ != NULL)
			{
				ret_value = ((Device *)parent_)->Activation(this);
			}
			else
			{
				ret_value = RET_VALUE_NOT_ATTACHED_TO_DEVICE;
				ERROR(this, ret_value, "The endpoint[%s] is not attached to device!", properties_.id.c_str());
			}

		}
	}

	return	ret_value;	
}

RetValue	Endpoint::Deactivation()
{
	RetValue 	ret_value = RET_VALUE_OK;

	if (activation_)
	{
		activation_ = false;
		if (parent_ != NULL)
		{
			((Device *)parent_)->Deactivation(this);
		}
		else
		{
			ret_value = RET_VALUE_NOT_ATTACHED_TO_DEVICE;
			ERROR(this, ret_value, "The endpoint[%s] is not attached to device!", properties_.id.c_str());
		}

	}

	return	ret_value;
}

RetValue	Endpoint::Synchronize()
{
	TRACE(this, "The endpoint[%s] was synchronized.");
	return	RET_VALUE_OK;
}

void		Endpoint::MaxValueCount
(
	uint32_t _count
)
{
	properties_.value_count = _count;

	while(value_list_.size() > properties_.value_count)
	{
		value_list_.pop_front();
	}
}

uint32_t	Endpoint::ValueCount()
{
	return	value_list_.size();
}

RetValue	Endpoint::AddValue
(
	const TimedValue& _value
)
{
	Lock();

	if (value_list_.size() >= properties_.value_count)
	{
		value_list_.pop_front();
	}

	value_list_.push_back(_value);

	Unlock();

	return	RET_VALUE_OK;
}

RetValue	Endpoint::DeleteValue
(
	uint32_t	_count
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
	uint32_t _index
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
		
uint32_t	Endpoint::GetValueList
(
	TimedValue*	_value_list,
	uint32_t	_max_count
)
{
	uint32_t	count = 0;
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

	while(type_info->type != Endpoint::TYPE_UNKNOWN)
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

	while(type_info->type != Endpoint::TYPE_UNKNOWN)
	{
		if (type_info->name.compare(_name) == 0)
		{
			return	type_info->type;
		}

		type_info++;
	}

	return	Endpoint::TYPE_UNKNOWN;
}

RetValue	Endpoint::SetProperties
(
	const	JSONNode&	_endpoint
)
{
	RetValue ret_value = RET_VALUE_OK;

	if (_endpoint.type() == JSON_NODE)
	{
		for(size_t i = 0 ; i < _endpoint.size() ; i++)
		{
			SetProperties(_endpoint[i]);
		}
	}
	else 
	{
		if (_endpoint.name() == "name")
		{
			if ((_endpoint.type() != JSON_STRING) && (_endpoint.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "");	
			}
			else
			{
				properties_.name = _endpoint.as_string();
			}
		}
		else if (_endpoint.name() == "id")
		{
			if ((_endpoint.type() != JSON_STRING) && (_endpoint.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "");	
			}
			else
			{
				properties_.id = _endpoint.as_string();
			}
		}
	}

	return	ret_value;
}

void	Endpoint::ReleaseParent()
{
	if (parent_ != NULL)
	{
		Device*	device = (Device *)parent_;
		parent_ = NULL;

		device->Disconnect(this);	
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
	case	Endpoint::TYPE_SENSOR_TEMPERATURE:		return	new	EndpointSensorTemperature;
	case	Endpoint::TYPE_SENSOR_HUMIDITY:			return	new EndpointSensorHumidity;
	case	Endpoint::TYPE_SENSOR_VOLTAGE: 			return	new EndpointSensorVoltage;
	case	Endpoint::TYPE_CONTROL_DIGITAL_OUTPUT:	return	new EndpointControlDigitalOutput;
	default	:
		return	NULL;
	}
}

Endpoint* 	Endpoint::Create
(
	const Endpoint::Properties* properties
)
{
	switch(properties->type)
	{
	case	Endpoint::TYPE_SENSOR_TEMPERATURE:		return	new	EndpointSensorTemperature((EndpointSensorTemperature::Properties*)properties);
	case	Endpoint::TYPE_SENSOR_HUMIDITY:			return	new EndpointSensorHumidity((EndpointSensorHumidity::Properties*)properties);
	case	Endpoint::TYPE_SENSOR_VOLTAGE: 			return	new EndpointSensorVoltage((EndpointSensorVoltage::Properties*)properties);
	case	Endpoint::TYPE_CONTROL_DIGITAL_OUTPUT:	return	new EndpointControlDigitalOutput((EndpointControlDigitalOutput::Properties*)properties);
	default	:
		return	NULL;
	}
}

ostream& operator<<
(
	ostream& _os, 
	const Endpoint& _ep
)
{
	_os << "Name : " << _ep.properties_.name << endl;
	_os << "  ID : " << _ep.properties_.id << endl;

	return	_os;
}
