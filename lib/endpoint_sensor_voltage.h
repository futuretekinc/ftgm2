#ifndef	ENDPOINT_VOLTAGE_SENSOR_H_
#define	ENDPOINT_VOLTAGE_SENSOR_H_

#include "endpoint_sensor.h"

class	EndpointSensorVoltage : public EndpointSensor
{
public:
	struct	Properties : EndpointSensor::Properties
	{
		Properties() ;
		Properties(const Properties& _properties);
		Properties(const Properties* _properties);
		Properties(const JSONNode& _node);

		Properties*	Duplicate();
	};

	EndpointSensorVoltage();
	EndpointSensorVoltage(const std::string& _id);
	EndpointSensorVoltage(const Properties& _properties);
	EndpointSensorVoltage(const Properties* _properties);
};

#endif
