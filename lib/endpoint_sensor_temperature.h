#ifndef	ENDPOINT_SENSOR_TEMPERATURE_H_
#define	ENDPOINT_SENSOR_TEMPERATURE_H_

#include "endpoint_sensor.h"
#include "KompexSQLiteStatement.h"


class	EndpointSensorTemperature : public EndpointSensor
{
public:
	struct	Properties : EndpointSensor::Properties
	{
		Properties();
		Properties(Kompex::SQLiteStatement*	_statement);
		Properties(const Properties* _properties);
		Properties(const JSONNode& _node);
	};

	EndpointSensorTemperature();
	EndpointSensorTemperature(const Properties* _properties);

};

#endif
