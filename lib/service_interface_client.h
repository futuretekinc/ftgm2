#ifndef	SERVICE_INTERFACE_CLIENT_H_
#define	SERVICE_INTERFACE_CLIENT_H_

#include "device.h"

class	ServiceInterfaceClient
{
public:
	ServiceInterfaceClient();
	~ServiceInterfaceClient();

	RetValue	DeviceCreate(Device::Type _type, const Properties& _properties, std::string& _device_id);
	RetValue	DeviceDestroy(const std::string& _device_id);
	RetValue	DeviceSetProperties(const std::string& _device_id, const Properties& _properties);
	RetValue	DeviceGetProperties(const std::string& _device_id, Properties& _properties);
	RetValue	DeviceGetList(std::list<std::string> _device_id_list, uint32_t _index = 0, uint32_t _count = 0);

	RetValue	EndpointCreate(Endpoint::Type _type, const Properties& _properties, std::string& _endpoint_id);
	RetValue	EndpointDestroy(std::string& _endpoint_id);
	RetValue	EndpointSetProperties(const std::string& _endpoint_id, const Properties& _properties);
	RetValue	EndpointGetProperties(const std::string& _endpoint_id, Properties& _properties);
	RetValue	EndpointGetList(std::list<std::string> _device_id_list, uint32_t _index = 0, uint32_t _count = 0);

protected:
};

#endif
