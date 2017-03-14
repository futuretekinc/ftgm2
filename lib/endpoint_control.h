#ifndef	ENDPOINT_CONTROL_H_
#define	ENDPOINT_CONTROL_H_

#include "endpoint.h"

class	EndpointControl : public Endpoint
{
public:
	struct	Properties : Endpoint::Properties
	{
		Properties(const Properties& _properties);
		Properties(const Properties* _properties);
		Properties(Type _type = TYPE_CONTROL) : Endpoint::Properties(_type) {};
		Properties(const JSONNode& _node);

		virtual
		Properties*	Duplicate();

		double	max_value;
		double	min_value;
	};

	EndpointControl(Type _type);
	EndpointControl(Type _type, const std::string& _id);
	EndpointControl(const Properties& _properties);
	EndpointControl(const Properties* _properties);
	~EndpointControl();

	double	GetValue()		{	return	value_;	}
	virtual
	double	SetValue(double _value)	{	return	value_ = _value;	}
	double	GetMaxValue()	{	return	max_value_;	}
	double	SetMaxValue(double _value)	{	return	max_value_ = _value;	}
	double	GetMinValue()	{	return	min_value_;	}
	double	SetMinValue(double _value)	{	return	min_value_ = _value;	}
private:
	double	value_;
	double	max_value_;
	double	min_value_;
};

#include "endpoint_control_digital_output.h"

#endif
