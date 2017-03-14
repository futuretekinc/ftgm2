#include "endpoint.h"

using namespace std;

EndpointControl::Properties::Properties
(
	const Properties& _properties
)
: Endpoint::Properties(_properties)
{
}

EndpointControl::Properties::Properties
(
	const Properties* _properties
)
: Endpoint::Properties(_properties)
{
}

EndpointControl::Properties::Properties
(
	const JSONNode& _node
) 
: Endpoint::Properties(_node) 
{
	type = TYPE_CONTROL;
}


EndpointControl::Properties*	EndpointControl::Properties::Duplicate()
{
	return	new	Properties(this);
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

EndpointControl::EndpointControl
(
	const Properties& _properties
)
:	Endpoint(_properties)
{
	value_	= 0;
	max_value_	= _properties.max_value;
	min_value_	= _properties.min_value;
}

EndpointControl::EndpointControl
(
	const Properties* _properties
)
:	Endpoint(_properties)
{
	value_	= 0;
	max_value_	= _properties->max_value;
	min_value_	= _properties->min_value;
}

EndpointControl::~EndpointControl()
{
}
