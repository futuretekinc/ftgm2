#include "service_interface_client.h"

ServiceInterfaceClient::ServiceInterfaceClient()
{
}

ServiceInterfaceClient::~ServiceInterfaceClient()
{

}

RetValue	ServiceInterfaceClient::DeviceCreate
(
	Device::Type _type, 
	const Properties& _properties, 
	std::string& _device_id
)
{
	JSONNode	root;
	JSONNode	device_properties;

}

RetValue	ServiceInterfaceClient::DeviceDestroy
(
	const std::string& _device_id
)
{
}

RetValue	ServiceInterfaceClient::DeviceSetProperties
(
	const std::string& _device_id, 
	const Properties& _properties
)
{
}

RetValue	ServiceInterfaceClient::DeviceGetProperties
(
	const std::string& _device_id, 
	Properties& _properties
)
{
}

RetValue	ServiceInterfaceClient::EndpointCreate
(
	Endpoint::Type _type, 
	const Properties& _properties, 
	std::string& _endpoint_id
)
{
}

RetValue	ServiceInterfaceClient::EndpointDestroy
(
	std::string& _endpoint_id
)
{
}

RetValue	ServiceInterfaceClient::EndpointSetProperties
(
	const std::string& _endpoint_id, 
	const Properties& _properties
)
{
}

RetValue	ServiceInterfaceClient::EndpointGetProperties
(
	const std::string& _endpoint_id, 
	Properties& _properties
)
{
}

