#include "endpoint.h"
#include "trace.h"

using namespace std;

EndpointSensor::Properties::Properties
(
	const Properties& _properties
)
:	Endpoint::Properties(_properties)
{
	max_value	= _properties.max_value;
	min_value	= _properties.min_value;
}

EndpointSensor::Properties::Properties
(
	const Properties* _properties
)
:	Endpoint::Properties(_properties)
{
	max_value	= _properties->max_value;
	min_value	= _properties->min_value;
}

EndpointSensor::Properties::Properties
(
	const JSONNode& _node
) 
: Endpoint::Properties(_node) 
{
	type = TYPE_SENSOR;
}

EndpointSensor::Properties*	EndpointSensor::Properties::Duplicate()
{
	return	new	Properties(this);
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
	max_value_	= 0;
	min_value_	= 0;
}

EndpointSensor::EndpointSensor
(
	const Properties& _properties
)
:	Endpoint(_properties)
{
	value_	= 0;
	max_value_	= _properties.max_value;
	min_value_	= _properties.min_value;
}

EndpointSensor::EndpointSensor
(
	const Properties* _properties
)
:	Endpoint(_properties)
{
	value_	= 0;
	max_value_	= _properties->max_value;
	min_value_	= _properties->min_value;
}

EndpointSensor::EndpointSensor
(
	Type			_type,
	const string&	_id
)
:	Endpoint(_type, _id)
{
	value_	= 0;
	max_value_	= 0;
	min_value_	= 0;
}

EndpointSensor::~EndpointSensor()
{
}

