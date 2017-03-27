#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <mutex>
#include <unistd.h>
#include <libjson/libjson.h>

#include "trace.h"
#include "object_manager.h"
#include "snmp_object.h"
#include "snmp_client.h"
#include "device.h"
#include "device_fte.h"
#include "endpoint.h"
#include "shell.h"
#include "data_manager.h"
#include "service_interface.h"

using namespace std;

string	json_device_create("{\"message\" : \"device_create\",\"device\" : { \"type\" : \"fte\", \"id\" : \"1234\"}}");
							
int main()
{
	Device	device(Device::TYPE_FTE);

	Device::Properties* properties = device.GetProperties();
	
	properties->id = "dev-1234";

	SIReqDeviceCreate device_create(properties);
	cout << "Request Message [" << "Device Create" << "]" << endl;
	cout << device_create.ToJSON().write_formatted() << endl;

	SIReqDeviceGetProperties device_get_properties(properties->id);
	cout << endl << "Request Message [" << "Device Get Properties" << "]" << endl;
	cout << device_get_properties.ToJSON().write_formatted() << endl;

	SIReqDeviceDestroy device_destroy(properties->id);
	cout << endl << "Request Message [" << "Device Destroy" << "]" << endl;
	cout << device_destroy.ToJSON().write_formatted() << endl;

	ServiceInterface* req = ServiceInterface::RequestParser(json_device_create);
	if (req == NULL)
	{
		cout << "Failed to create message" << endl;
	}

	return	0;	
}
