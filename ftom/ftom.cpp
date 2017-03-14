#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <mutex>
#include <unistd.h>
#include "trace.h"
#include "object_manager.h"
#include "snmp_object.h"
#include "snmp_client.h"
#include "device.h"
#include "device_fte.h"
#include "endpoint.h"
#include "shell.h"
#include "shell_command.h"
#include "data_manager.h"

using namespace std;

oid	temp1[] = { 1, 3, 6, 1, 4, 1, 42251, 1, 3, 256, 2, 1, 6, 1};
SNMPClient::Properties	client_properties = 
{
	.name = "FTE"
};

SNMPSession::Properties	session_properties = 
{
	.peer = "10.0.1.39",
	.community="public"
};

extern	ShellCommand<ObjectManager, DataManager>	test_shell_commands[];
extern	int	test_shell_command_count;

int main()
{
	ObjectManager*	om = new ObjectManager;
	DataManager*	dm = new DataManager;

#if 0
	int	i, j;
	for(i = 0 ; i < 10 ; i++)
	{
		DeviceFTE::Properties	deviceProperties;

		deviceProperties.name = "Device" + to_string(i);
		deviceProperties.id = "device" + to_string(i);
		deviceProperties.url = string("127.0.0.1");
		deviceProperties.community = string("public");

		Device *device = Device::Create(&deviceProperties);
		if (device != NULL)
		{
			RetValue	ret_value;

			ret_value = om->Attach(device);
			if (ret_value == RET_VALUE_OK)
			{
				TRACE(NULL, "The device[%s] is created!", device->GetID().c_str());

				for(j = 0 ; j < 10 ; j++)
				{
					EndpointSensorTemperature::Properties	endpointProperties;

					endpointProperties.name = "Temp" + to_string(j);
					endpointProperties.id = "device" + to_string(i) + "-temp" + to_string(j);
					endpointProperties.max_value = 100;
					endpointProperties.min_value = -100;

					EndpointSensorTemperature* endpoint = new EndpointSensorTemperature(endpointProperties);
					if (endpoint == NULL)
					{
						ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;
						ERROR(NULL, ret_value, "The endpoint[%s] creation failed.", endpointProperties.id.c_str());
					}
					else
					{
						ret_value = om->Attach(endpoint);
						if (ret_value == RET_VALUE_OK)
						{
							TRACE(NULL, "The endpoint[%s] is created!", endpoint->GetID().c_str());

							ret_value = device->Attach(endpoint);	
							if (ret_value != RET_VALUE_OK)
							{
								ERROR(NULL, ret_value, "The endpoint[%s] is failed to attach at device[%s]!", endpoint->GetID().c_str(), device->GetID().c_str());
							}
							else
							{
								TRACE(NULL, "The endpoint[%s] is attached at device[%s]!", endpoint->GetID().c_str(), device->GetID().c_str());
							}
						}
						else
						{
							ERROR(NULL, ret_value, "The endpoint[%s] is failed to attach at object manager!", endpoint->GetID().c_str());
							
							delete endpoint;
						}
					}
				}
			}
			else
			{
				ERROR(NULL, ret_value, "The device[%s] is failed to attach at object manager!", device->GetID().c_str());
							
				delete device;
			}
		}

	}
#endif

	TRACE_ON();
	ERROR_ON();

	dm->Start(1000);

	om->Load("ftom.conf");
	om->Connect(dm);

	om->Start();
	
	if (!dm->IsTableExist("devices"))
	{
		dm->CreateDeviceTable();
	}

	Device*	device = om->GetDevice(string("1234-1"));
	if (device != NULL)
	{
		if (!dm->IsDeviceExist(device))
		{
			dm->AddDevice(device);
		}
	}

	TRACE(NULL, "Check Endpoint Table");
	if (!dm->IsTableExist("endpoints"))
	{
		dm->CreateEndpointTable();
	}
	Endpoint*	endpoint = om->GetEndpoint(string("ep-1"));
	if (endpoint != NULL)
	{
		if (!dm->IsEndpointExist(endpoint))
		{
			dm->AddEndpoint(endpoint);
		}
	}

	endpoint = om->GetEndpoint(string("ep-2"));
	if (endpoint != NULL)
	{
		if (!dm->IsEndpointExist(endpoint))
		{
			dm->AddEndpoint(endpoint);
		}
	}

	endpoint = om->GetEndpoint(string("ep-3"));
	if (endpoint != NULL)
	{
		if (!dm->IsEndpointExist(endpoint))
		{
			dm->AddEndpoint(endpoint);
		}
	}
#if 0
	SNMPClient*		client = new SNMPClient(client_properties);
	client->Start();

	SNMPSession*	session  new SNMPSession(session_properties);
	if (session != NULL)
	{
		RetValue	ret_value;

		ret_value = session->Open();
		if (ret_value != RET_VALUE_OK)
		{
			ERROR(NULL, ret_value, "The session open failed!");	
		}
	}
#endif
	Shell<ObjectManager, DataManager>	shell(object_manager_shell_commands, object_manager_shell_command_count, om, dm);

	shell.Start(true);

	return	0;	
}
