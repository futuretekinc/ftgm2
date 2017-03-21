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

EndpointControl::EndpointControl
(
	Type			_type,
	const string&	_id
)
:	Endpoint(_type, _id)
{
	value_		= 0;
	max_value_	= 0;
	min_value_	= 0;
}

#if 0
EndpointControl::EndpointControl
(
	const Properties& _properties
)
:	Endpoint(_properties)
{
	value_	= 0;
	max_value_	= _properties.options.max_value;
	min_value_	= _properties.options.min_value;
}

EndpointControl::EndpointControl
(
	const Properties* _properties
)
:	Endpoint(_properties)
{
	value_	= 0;
	max_value_	= _properties->options.max_value;
	min_value_	= _properties->options.min_value;
}
#endif
EndpointControl::~EndpointControl()
{
}
