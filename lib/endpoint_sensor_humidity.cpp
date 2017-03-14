#include "endpoint.h"
#include "endpoint_sensor_humidity.h"

using namespace std;

EndpointSensorHumidity::Properties::Properties()
: EndpointSensor::Properties(TYPE_SENSOR_HUMIDITY) 
{
}

EndpointSensorHumidity::Properties::Properties
(
	const	Properties& _properties
)
:	EndpointSensor::Properties(_properties)
{
}

EndpointSensorHumidity::Properties::Properties
(
	const	Properties* _properties
)
:	EndpointSensor::Properties(_properties)
{
}

EndpointSensorHumidity::Properties::Properties
(
	const JSONNode& _node
) 
: EndpointSensor::Properties(_node) 
{
	type = TYPE_SENSOR_HUMIDITY;
}

EndpointSensorHumidity::Properties*	EndpointSensorHumidity::Properties::Duplicate()
{
	return	new	Properties(this);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Humidity Sensor endpoint
//////////////////////////////////////////////////////////////////////////

EndpointSensorHumidity::EndpointSensorHumidity()
:	EndpointSensor(TYPE_SENSOR_HUMIDITY)
{
}

EndpointSensorHumidity::EndpointSensorHumidity
(
	const string&	_id
)
:	EndpointSensor(TYPE_SENSOR_HUMIDITY, _id)
{
}

EndpointSensorHumidity::EndpointSensorHumidity
(
	const Properties& _properties
)
:	EndpointSensor(_properties)
{
}

EndpointSensorHumidity::EndpointSensorHumidity
(
	const Properties* _properties
)
:	EndpointSensor(_properties)
{
}

