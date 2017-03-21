#include "endpoint.h"
#include "endpoint_sensor_humidity.h"

using namespace std;

EndpointSensorHumidity::Properties::Properties()
: EndpointSensor::Properties(HUMIDITY_SENSOR) 
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
	type = HUMIDITY_SENSOR;
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
:	EndpointSensor(HUMIDITY_SENSOR)
{
}

EndpointSensorHumidity::EndpointSensorHumidity
(
	const string&	_id
)
:	EndpointSensor(HUMIDITY_SENSOR, _id)
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

