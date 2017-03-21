#ifndef	ENDPOINT_CONTROL_DIGITAL_OUTPUT_H_
#define	ENDPOINT_CONTROL_DIGITAL_OUTPUT_H_

#include "endpoint_control.h"
#include "KompexSQLiteStatement.h"

// Digital Output Endpoint
class EndpointControlDigitalOutput : public EndpointControl
{
public:
	struct	Properties : EndpointControl::Properties
	{
		Properties();
	};

	EndpointControlDigitalOutput();
	EndpointControlDigitalOutput(const std::string& _id);
};

#endif
