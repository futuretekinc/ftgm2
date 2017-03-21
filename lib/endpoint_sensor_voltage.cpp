#include "endpoint_sensor_voltage.h"

using namespace std;

EndpointSensorVoltage::Properties::Properties()
: EndpointSensor::Properties(VOLTAGE_SENSOR) 
{
}

#if 0
EndpointSensorVoltage::Properties::Properties
(
	const	Properties& _properties
)
:	EndpointSensor::Properties(_properties)
{
}

EndpointSensorVoltage::Properties::Properties
(
	const	Properties* _properties
)
:	EndpointSensor::Properties(_properties)
{
}

EndpointSensorVoltage::Properties::Properties
(
	const JSONNode& _node
) 
: EndpointSensor::Properties(_node) 
{
	type = VOLTAGE_SENSOR;
}

EndpointSensorVoltage::Properties*	EndpointSensorVoltage::Properties::Duplicate()
{
	return	new	Properties(this);
}
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Voltage Sensor endpoint
//////////////////////////////////////////////////////////////////////////

EndpointSensorVoltage::EndpointSensorVoltage()
:	EndpointSensor(VOLTAGE_SENSOR)
{
}

EndpointSensorVoltage::EndpointSensorVoltage
(
	const string& _id
)
:	EndpointSensor(VOLTAGE_SENSOR, _id)
{
}

EndpointSensorVoltage::EndpointSensorVoltage
(
	const Properties& _properties
)
:	EndpointSensor(_properties)
{
}

EndpointSensorVoltage::EndpointSensorVoltage
(
	const Properties* _properties
)
:	EndpointSensor(_properties)
{
}

