#ifndef	ENDPOINT_SENSOR_TEMPERATURE_H_
#define	ENDPOINT_SENSOR_TEMPERATURE_H_

#include "endpoint_sensor.h"


class	EndpointSensorTemperature : public EndpointSensor
{
public:
	struct	Properties : EndpointSensor::Properties
	{
		Properties() ;
		Properties(const Properties& _properties);
		Properties(const Properties* _properties);
		Properties(const JSONNode& _node);

		Properties* Duplicate();
	};

				EndpointSensorTemperature();
				EndpointSensorTemperature(const std::string& _id);
				EndpointSensorTemperature(const Properties& _properties);
				EndpointSensorTemperature(const Properties* _properties);

	virtual
	RetValue	Synchronize();
};

#endif
