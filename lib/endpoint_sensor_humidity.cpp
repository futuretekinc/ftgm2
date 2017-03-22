#include "endpoint.h"
#include "endpoint_sensor_humidity.h"

using namespace std;

EndpointSensorHumidity::Properties::Properties()
: EndpointSensor::Properties(HUMIDITY_SENSOR) 
{
}

#if 0
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
#endif

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
	const Properties* _properties
)
:	EndpointSensor(_properties)
{
}

