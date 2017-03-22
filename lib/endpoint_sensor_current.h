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
		Properties(Kompex::SQLiteStatement*	_statement);
		Properties(const Properties& _properties);
		Properties(const Properties* _properties);
		Properties(const JSONNode& _node);

		Properties*	Duplicate();
	};

	EndpointSensorCurrent();
	EndpointSensorCurrent(const std::string& _id);
	EndpointSensorCurrent(const Properties& _properties);
	EndpointSensorCurrent(const Properties* _properties);
};

#endif
