#ifndef	ENDPOINT_CONTROL_DIGITAL_OUTPUT_H_
#define	ENDPOINT_CONTROL_DIGITAL_OUTPUT_H_

#include "endpoint_control.h"

// Digital Output Endpoint
class EndpointControlDigitalOutput : public EndpointControl
{
public:
	struct	Properties : EndpointControl::Properties
	{
		Properties();
		Properties(const JSONNode& _node);
	};

	EndpointControlDigitalOutput();
	EndpointControlDigitalOutput(const std::string& _id);
	EndpointControlDigitalOutput(const Properties& _properties);
	EndpointControlDigitalOutput(const Properties* _properties);
};

#endif
