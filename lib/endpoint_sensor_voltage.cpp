#include "endpoint_sensor_voltage.h"

using namespace std;

EndpointSensorVoltage::Properties::Properties()
: EndpointSensor::Properties(VOLTAGE_SENSOR) 
{
}

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
	const Properties* _properties
)
:	EndpointSensor(_properties)
{
}

