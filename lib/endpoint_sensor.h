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
//		Properties(const Properties& _properties);
//		Properties(const Properties* _properties);
//		Properties(const JSONNode& _node);

		Properties*	Duplicate();

		RetValue	Set(const JSONNode& _node);
		RetValue	Set(const Properties* _properties);
		RetValue	Set(const Kompex::SQLiteStatement*	_statement);

		RetValue	SetProperty(const std::string& _name, double	_value);

		uint32		GetOptionsSize();
		uint32		GetOptions(uint8_t *options, uint32 options_len);

		Options	options;
	};

				EndpointSensor(Type _type);
				EndpointSensor(const Properties& _properties);
				EndpointSensor(const Properties* _properties);
				EndpointSensor(Type	_type,const std::string&	_id);
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

#endif
