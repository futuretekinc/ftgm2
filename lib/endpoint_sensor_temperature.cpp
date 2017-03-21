#include "endpoint_sensor_temperature.h"
#include "trace.h"
#include "device.h"

using namespace std;

EndpointSensorTemperature::Properties::Properties()
: EndpointSensor::Properties(TEMPERATURE_SENSOR) 
{
}

EndpointSensorTemperature::Properties::Properties
(
	Kompex::SQLiteStatement*	_statement
)
: EndpointSensor::Properties(TEMPERATURE_SENSOR)
{
	Set(_statement);
}

EndpointSensorTemperature::Properties::Properties
(
	const	Properties& _properties
)
: EndpointSensor::Properties(TEMPERATURE_SENSOR)
{
	Set(&_properties);
}

EndpointSensorTemperature::Properties::Properties
(
	const	Properties* _properties
)
: EndpointSensor::Properties(TEMPERATURE_SENSOR)
{
	Set(_properties);
}

EndpointSensorTemperature::Properties::Properties
(
	const JSONNode& _node
) 
: EndpointSensor::Properties(TEMPERATURE_SENSOR)
{
	Set(_node);
}

EndpointSensorTemperature::Properties*	EndpointSensorTemperature::Properties::Duplicate()
{
	return	new	Properties(this);
}
//////////////////////////////////////////////////////////////////////////
// Sensor endpoint
//////////////////////////////////////////////////////////////////////////
// Temperture Sensor endpoint
//////////////////////////////////////////////////////////////////////////

EndpointSensorTemperature::EndpointSensorTemperature ()
:	 EndpointSensor(TEMPERATURE_SENSOR)
{
}

EndpointSensorTemperature::EndpointSensorTemperature
(
	const string& _id
)
:	EndpointSensor(TEMPERATURE_SENSOR, _id)
{
}

EndpointSensorTemperature::EndpointSensorTemperature
(
	const Properties& _properties
)
:	 EndpointSensor(TEMPERATURE_SENSOR)
{
	Set(&_properties);
}

EndpointSensorTemperature::EndpointSensorTemperature
(
	const Properties* _properties
)
:	 EndpointSensor(TEMPERATURE_SENSOR)
{
	Set(_properties);
}

