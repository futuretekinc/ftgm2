#ifndef	ENDPOINT_SENSOR_HUMIDITY_H_
#define	ENDPOINT_SENSOR_HUMIDITY_H_

#include "endpoint_sensor.h"
#include "KompexSQLiteStatement.h"

class	EndpointSensorHumidity : public EndpointSensor
{
public:
	struct	Properties : EndpointSensor::Properties
	{
		Properties();
		Properties(const Properties* _properties);
		Properties(const JSONNode& _node);
	};

	EndpointSensorHumidity();
	EndpointSensorHumidity(const Properties* _properties);
};

#endif
