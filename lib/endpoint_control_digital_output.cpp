#include "endpoint_control_digital_output.h"

using namespace std;

EndpointControlDigitalOutput::Properties::Properties()
: EndpointControl::Properties(DO_CONTROL)
{
}

#if 0
EndpointControlDigitalOutput::Properties::Properties
(
	const JSONNode& _node
) 
: EndpointControl::Properties(_node) 
{
	type = DO_CONTROL;
}
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Digital Output Control endpoint
//////////////////////////////////////////////////////////////////////////

EndpointControlDigitalOutput::EndpointControlDigitalOutput()
:	EndpointControl(Endpoint::DO_CONTROL)
{
}

EndpointControlDigitalOutput::EndpointControlDigitalOutput
(
	const string& _id
)
:	EndpointControl(Endpoint::DO_CONTROL, _id)
{
}



