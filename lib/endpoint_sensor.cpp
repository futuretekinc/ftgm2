#include "endpoint.h"
#include "trace.h"
#include "KompexSQLiteException.h"

using namespace std;

EndpointSensor::Properties::Properties
(
	Type	_type
)
:	Endpoint::Properties(_type)
{
}

#if 0
EndpointSensor::Properties::Properties
(
	const Properties& _properties
)
:	Endpoint::Properties(_properties)
{
	options.max_value	= _properties.options.max_value;
	options.min_value	= _properties.options.min_value;
}

EndpointSensor::Properties::Properties
(
	const Properties* _properties
)
:	Endpoint::Properties(_properties)
{
	options.max_value	= _properties->options.max_value;
	options.min_value	= _properties->options.min_value;
}

EndpointSensor::Properties::Properties
(
	const JSONNode& _node
) 
{
	Set(_node);
}

#endif

RetValue EndpointSensor::Properties::Set
(
	const Properties* _properties
)
{
	options.max_value	= _properties->options.max_value;
	options.min_value	= _properties->options.min_value;

	return	Endpoint::Properties::Set(_properties);

}

RetValue	EndpointSensor::Properties::Set
(
	const Kompex::SQLiteStatement*	_statement
)
{
	ASSERT(_statement != NULL);
	RetValue ret_value = RET_VALUE_OK;

	try
	{	
		RetValue ret_value;

		ret_value = Endpoint::Properties::Set(_statement);
		if (ret_value == RET_VALUE_OK)
		{
			memcpy(&options, _statement->GetColumnBlob("_options"), sizeof(Options));
		}
	}
	catch (Kompex::SQLiteException &exception)
	{
		cout << "SQLiteException occurred!" << endl;
		ret_value = RET_VALUE_DB_ENTRY_DOES_NOT_EXIST;
	}

	return	ret_value;
}

RetValue EndpointSensor::Properties::Set
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
		if (strcasecmp(_node.name().c_str(), "min_value") == 0)
		{
			if (_node.type() == JSON_NUMBER)
			{
				options.min_value = _node.as_int();
			}
			else
			{
				ret_value = RET_VALUE_INVALID_FIELD;
			}
		}
		else if (strcasecmp(_node.name().c_str(), "max_value") == 0)
		{
			if (_node.type() == JSON_NUMBER)
			{
				options.max_value = _node.as_int();
			}
			else
			{
				ret_value = RET_VALUE_INVALID_FIELD;
			}
		}
		else 
		{
			ret_value = Endpoint::Properties::Set(_node);
		}
	}

	ret_value;
}

RetValue	EndpointSensor::Properties::SetProperty
(
	const std::string& _name, 
	double	_value
)
{
	RetValue	ret_value = RET_VALUE_OK;	

	if (_name == "min_value")
	{
		options.min_value = _value;
	}
	else if (_name == "max_value")
	{
		options.max_value = _value;
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FIELD;	
	}

	return	ret_value;
}

uint32	EndpointSensor::Properties::GetOptionsSize()
{
	return	sizeof(options);
}

uint32	EndpointSensor::Properties::GetOptions
(
	uint8_t *_options, 
	uint32 _options_len
)
{
	if (_options_len > sizeof(options))
	{
		memcpy(_options, &options, sizeof(options));

		return	sizeof(options);
	}

	return	0;
}
//////////////////////////////////////////////////////////////////////////
// Sensor endpoint
//////////////////////////////////////////////////////////////////////////

EndpointSensor::EndpointSensor
(
	Type _type
)
:	Endpoint(_type)
{
	value_		= 0;
	dynamic_cast<EndpointSensor::Properties*>(properties_)->options.max_value	= 0;
	dynamic_cast<EndpointSensor::Properties*>(properties_)->options.min_value	= 0;
}

EndpointSensor::EndpointSensor
(
	const Properties& _properties
)
:	Endpoint(_properties)
{
	value_	= 0;
	dynamic_cast<EndpointSensor::Properties*>(properties_)->options.max_value	= _properties.options.max_value;
	dynamic_cast<EndpointSensor::Properties*>(properties_)->options.min_value	= _properties.options.min_value;
}

EndpointSensor::EndpointSensor
(
	const Properties* _properties
)
:	Endpoint(_properties)
{
	value_	= 0;
	dynamic_cast<EndpointSensor::Properties*>(properties_)->options.max_value	= _properties->options.max_value;
	dynamic_cast<EndpointSensor::Properties*>(properties_)->options.min_value	= _properties->options.min_value;
}

EndpointSensor::EndpointSensor
(
	Type			_type,
	const string&	_id
)
:	Endpoint(_type, _id)
{
	value_	= 0;
	dynamic_cast<EndpointSensor::Properties*>(properties_)->options.max_value	= 0;
	dynamic_cast<EndpointSensor::Properties*>(properties_)->options.min_value	= 0;
}

EndpointSensor::~EndpointSensor()
{
}

RetValue	EndpointSensor::SetProperty
(
	const std::string& _name, 
	double	_value
)
{
	RetValue	ret_value = RET_VALUE_OK;	
	Properties	*properties = dynamic_cast<EndpointSensor::Properties*>(properties_);

	if (_name == "min_value")
	{
		properties->options.min_value = _value;
	}
	else if (_name == "max_value")
	{
		properties->options.max_value = _value;
	}
	else
	{
		ret_value = Endpoint::SetProperty(_name, _value);
	}

	return	ret_value;
}

double	EndpointSensor::GetValue()
{
	return	value_;	
}

double	EndpointSensor::SetValue(double _value)	
{
	return	value_ = _value;	
}

double	EndpointSensor::GetMaxValue()	
{
	return	dynamic_cast<EndpointSensor::Properties *>(properties_)->options.max_value;	
}

double	EndpointSensor::SetMaxValue(double _value)
{
	return	dynamic_cast<EndpointSensor::Properties *>(properties_)->options.max_value = _value;	
}

double	EndpointSensor::GetMinValue()
{
	return	dynamic_cast<EndpointSensor::Properties *>(properties_)->options.min_value;	
}

double	EndpointSensor::SetMinValue(double _value)
{
	return	dynamic_cast<EndpointSensor::Properties *>(properties_)->options.min_value = _value;	
}
