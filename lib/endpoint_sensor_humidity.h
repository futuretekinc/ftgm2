#ifndef	ENDPOINT_SENSOR_HUMIDITY_H_
#define	ENDPOINT_SENSOR_HUMIDITY_H_

#include "endpoint_sensor.h"

class	EndpointSensorHumidity : public EndpointSensor
{
public:
	struct	Properties : EndpointSensor::Properties
	{
		Properties();
		Properties(const Properties& _properties);
		Properties(const Properties* _properties);
		Properties(const JSONNode& _node);

		Properties*	Duplicate();
	};

	EndpointSensorHumidity();
	EndpointSensorHumidity(const std::string& _id);
	EndpointSensorHumidity(const Properties& _properties);
	EndpointSensorHumidity(const Properties* _properties);
};

#endif
