#ifndef	ENDPOINT_VOLTAGE_SENSOR_H_
#define	ENDPOINT_VOLTAGE_SENSOR_H_

#include "endpoint_sensor.h"
#include "KompexSQLiteStatement.h"

class	EndpointSensorVoltage : public EndpointSensor
{
public:
	struct	Properties : EndpointSensor::Properties
	{
		Properties();
		Properties(const Properties* _properties);
		Properties(Kompex::SQLiteStatement*	_statement);
		Properties(const JSONNode& _node);
	};

	EndpointSensorVoltage();
	EndpointSensorVoltage(const Properties* _properties);
};

#endif
