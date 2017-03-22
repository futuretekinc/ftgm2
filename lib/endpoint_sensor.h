#ifndef	ENDPOINT_SENSOR_H_
#define	ENDPOINT_SENSOR_H_

#include "endpoint.h"


class	EndpointSensor : public Endpoint
{
public:
	struct	Options
	{
		double	max_value;
		double	min_value;
	};
	
	struct Properties : Endpoint::Properties
	{
		Properties(Type _type);

		RetValue	Set(const JSONNode& _node);
		RetValue	Set(const Endpoint::Properties* _properties);
		RetValue	Set(const Kompex::SQLiteStatement*	_statement);

		RetValue	SetProperty(const std::string& _name, double	_value);

		uint32		GetOptionsSize();
		uint32		GetOptions(uint8_t *options, uint32 options_len);

		Options	options;
	};

				EndpointSensor(Type _type);
				EndpointSensor(const Endpoint::Properties* _properties);
				~EndpointSensor();

	RetValue	SetProperty(const std::string& _name, double	_value);

	double		GetValue();
	double		SetValue(double _value);
	double		GetMaxValue();
	double		SetMaxValue(double _value);
	double		GetMinValue();
	double		SetMinValue(double _value);


private:
	double	value_;
};

#include "endpoint_sensor_temperature.h"
#include "endpoint_sensor_humidity.h"
#include "endpoint_sensor_voltage.h"
#include "endpoint_sensor_current.h"

#endif
