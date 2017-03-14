#include "endpoint_control_digital_output.h"

using namespace std;

EndpointControlDigitalOutput::Properties::Properties()
: EndpointControl::Properties(TYPE_CONTROL_DIGITAL_OUTPUT)
{
}

EndpointControlDigitalOutput::Properties::Properties
(
	const JSONNode& _node
) 
: EndpointControl::Properties(_node) 
{
	type = TYPE_CONTROL_DIGITAL_OUTPUT;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Digital Output Control endpoint
//////////////////////////////////////////////////////////////////////////

EndpointControlDigitalOutput::EndpointControlDigitalOutput()
:	EndpointControl(Endpoint::TYPE_CONTROL_DIGITAL_OUTPUT)
{
}

EndpointControlDigitalOutput::EndpointControlDigitalOutput
(
	const string& _id
)
:	EndpointControl(Endpoint::TYPE_CONTROL_DIGITAL_OUTPUT, _id)
{
}

EndpointControlDigitalOutput::EndpointControlDigitalOutput
(
	const Properties& _properties
)
:	EndpointControl(_properties)
{
}


EndpointControlDigitalOutput::EndpointControlDigitalOutput
(
	const Properties* _properties
)
:	EndpointControl(_properties)
{
}


