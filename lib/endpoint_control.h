#ifndef	ENDPOINT_CONTROL_H_
#define	ENDPOINT_CONTROL_H_

#include "endpoint.h"

class	EndpointControl : public Endpoint
{
public:
	struct	Options
	{
		double	max_value;
		double	min_value;
	};
	
	struct Properties : Endpoint::Properties
	{
		Properties(Type _type = UNKNOWN);

		Options	options;
	};

	EndpointControl(Type _type);
	~EndpointControl();

	double		GetValue();
	virtual
	RetValue	SetValue(double _value);
	double		GetMaxValue();
	virtual
	RetValue	SetMaxValue(double _value);
	double		GetMinValue();
	virtual
	RetValue	SetMinValue(double _value);

private:
	double	value_;
	double	max_value_;
	double	min_value_;
};

#include "endpoint_control_digital_output.h"

#endif
