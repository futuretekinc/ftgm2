#include "endpoint_sensor_temperature.h"
#include "trace.h"
#include "device.h"

using namespace std;

EndpointSensorTemperature::Properties::Properties()
: EndpointSensor::Properties(TYPE_SENSOR_TEMPERATURE)
{
}

EndpointSensorTemperature::Properties::Properties
(
	const	Properties& _properties
)
:	EndpointSensor::Properties(_properties)
{
}

EndpointSensorTemperature::Properties::Properties
(
	const	Properties* _properties
)
:	EndpointSensor::Properties(_properties)
{
}

EndpointSensorTemperature::Properties::Properties
(
	const JSONNode& _node
) 
: EndpointSensor::Properties(_node) 
{
	type = TYPE_SENSOR_TEMPERATURE;
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

EndpointSensorTemperature::EndpointSensorTemperature()
:	EndpointSensor(TYPE_SENSOR_TEMPERATURE)
{
}

EndpointSensorTemperature::EndpointSensorTemperature
(
	const string& _id
)
:	EndpointSensor(TYPE_SENSOR_TEMPERATURE, _id)
{
}

EndpointSensorTemperature::EndpointSensorTemperature
(
	const Properties& _properties
)
:	EndpointSensor(_properties)
{
}

EndpointSensorTemperature::EndpointSensorTemperature
(
	const Properties* _properties
)
:	EndpointSensor(_properties)
{
}

RetValue	EndpointSensorTemperature::Synchronize()
{
	RetValue	ret_value = RET_VALUE_OK;

	if (device_ == NULL)
	{
		ret_value = RET_VALUE_NOT_ATTACHED_TO_DEVICE;
		ERROR(this, ret_value, "The endpoint[%s] is not attached to device.", properties_.id.c_str());

	}
	else
	{
		device_->GetValue(this);

		TRACE(this, "The endpoint[%s] was synchronized.", properties_.id.c_str());
	}
	return	ret_value;
}
