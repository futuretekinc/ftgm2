#ifndef	ENDPOINT_CURRENT_SENSOR_H_
#define	ENDPOINT_CURRENT_SENSOR_H_

#include "endpoint_sensor.h"
#include "KompexSQLiteStatement.h"

class	EndpointSensorCurrent : public EndpointSensor
{
public:
	struct	Properties : EndpointSensor::Properties
	{
		Properties();
		Properties(const Properties* _properties);
		Properties(Kompex::SQLiteStatement*	_statement);
		Properties(const JSONNode& _node);
	};

	EndpointSensorCurrent();
	EndpointSensorCurrent(const Properties* _properties);
};

#endif
