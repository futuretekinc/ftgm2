#include "endpoint.h"

using namespace std;

EndpointControl::Properties::Properties
(
	Type _type
) 
: Endpoint::Properties(_type) 
{
}

//////////////////////////////////////////////////////////////////////////
// Control endpoint
//////////////////////////////////////////////////////////////////////////

EndpointControl::EndpointControl
(
	Type _type
)
:	Endpoint(_type)
{
	value_		= 0;
	max_value_	= 0;
	min_value_	= 0;
}

EndpointControl::~EndpointControl()
{
}
double	EndpointControl::GetValue()
{	
	return	value_;	
}

RetValue	EndpointControl::SetValue(double _value)
{	
	return	value_ = _value;
}

double	EndpointControl::GetMaxValue()
{	
	return	max_value_;	
}

RetValue	EndpointControl::SetMaxValue(double _value)	
{	
	max_value_ = _value;	
	
	return	RET_VALUE_OK;
}

double	EndpointControl::GetMinValue()	
{	
	return	min_value_;	
}

RetValue	EndpointControl::SetMinValue(double _value)	
{	
	min_value_ = _value;	

	return	RET_VALUE_OK;
}
