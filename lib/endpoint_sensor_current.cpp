#include "endpoint_sensor_current.h"

using namespace std;

EndpointSensorCurrent::Properties::Properties()
: EndpointSensor::Properties(CURRENT_SENSOR) 
{
}

#if 0
EndpointSensorCurrent::Properties::Properties
(
	const	Properties& _properties
)
:	EndpointSensor::Properties(_properties)
{
}

EndpointSensorCurrent::Properties::Properties
(
	const	Properties* _properties
)
:	EndpointSensor::Properties(_properties)
{
}

EndpointSensorCurrent::Properties::Properties
(
	const JSONNode& _node
) 
: EndpointSensor::Properties(_node) 
{
	type = CURRENT_SENSOR;
}

EndpointSensorCurrent::Properties*	EndpointSensorCurrent::Properties::Duplicate()
{
	return	new	Properties(this);
}
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Current Sensor endpoint
//////////////////////////////////////////////////////////////////////////

EndpointSensorCurrent::EndpointSensorCurrent()
:	EndpointSensor(CURRENT_SENSOR)
{
}

EndpointSensorCurrent::EndpointSensorCurrent
(
	const Properties* _properties
)
:	EndpointSensor(_properties)
{
}

