#include "endpoint_sensor_voltage.h"

using namespace std;

EndpointSensorVoltage::Properties::Properties()
: EndpointSensor::Properties(TYPE_SENSOR_VOLTAGE) 
{
}

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
	type = TYPE_SENSOR_VOLTAGE;
}

EndpointSensorVoltage::Properties*	EndpointSensorVoltage::Properties::Duplicate()
{
	return	new	Properties(this);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Voltage Sensor endpoint
//////////////////////////////////////////////////////////////////////////

EndpointSensorVoltage::EndpointSensorVoltage()
:	EndpointSensor(TYPE_SENSOR_VOLTAGE)
{
}

EndpointSensorVoltage::EndpointSensorVoltage
(
	const string& _id
)
:	EndpointSensor(TYPE_SENSOR_VOLTAGE, _id)
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

