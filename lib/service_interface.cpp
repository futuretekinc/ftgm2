#include <libjson/libjson.h>
#include "service_interface.h"
#include "trace.h"

using namespace std;

static
ServiceInterface::TypeInfo	si_type_info[] =
{
	{	ServiceInterface::UNKNOWN,				"unknown"	},

	{	ServiceInterface::DEVICE_CREATE,		"device_create",	ServiceInterface::CreateReqDeviceCreate},
	{	ServiceInterface::DEVICE_DESTROY,		"device_destroy",	ServiceInterface::CreateReqDeviceDestroy},
	{	ServiceInterface::DEVICE_GET_PROPERTIES,"device_get_properties"	},
	{	ServiceInterface::DEVICE_SET_PROPERTIES,"device_set_properties"	},
	{	ServiceInterface::DEVICE_GET_LIST,		"device_get_list"	},

	{	ServiceInterface::ENDPOINT_CREATE,		"endpoint_create"	},
	{	ServiceInterface::ENDPOINT_DESTROY,		"endpoint_destroy"	},
	{	ServiceInterface::ENDPOINT_GET_PROPERTIES,"endpoint_get_properties"	},
	{	ServiceInterface::ENDPOINT_SET_PROPERTIES,"endpoint_set_propwerties"	},
	{	ServiceInterface::ENDPOINT_GET_LIST,	"endpoint_get_list"	}
};

struct	ServiceInterfaceFieldInfo
{
	ServiceInterface::FieldType	type;
	string		name;
}	
field_info[] = 
{
	{	ServiceInterface::FieldType::FIELD_MESSAGE,	"message"	},
	{	ServiceInterface::FieldType::FIELD_DEVICE,	"device"	},
	{	ServiceInterface::FieldType::FIELD_DEVICES,	"devices"	},
	{	ServiceInterface::FieldType::FIELD_ENDPOINT,	"endpoint"	},
	{	ServiceInterface::FieldType::FIELD_ENDPOINTS,	"endpoints"	},
};


ServiceInterface::TypeInfo*	ServiceInterface::GetTypeInfo
(
	Type _type
)
{
	for(int i = 0 ; i < sizeof(si_type_info) / sizeof(ServiceInterface::TypeInfo); i++)
	{
		if (_type == si_type_info[i].type)
		{
			return	&si_type_info[i];	
		}
	}

	return	NULL;
}

ServiceInterface::TypeInfo*	ServiceInterface::GetTypeInfo
(
	const std::string& _type_name
)
{
	for(int i = 0 ; i < sizeof(si_type_info) / sizeof(ServiceInterface::TypeInfo); i++)
	{
		if (_type_name == si_type_info[i].name)
		{
			return	&si_type_info[i];	
		}
	}

	return	NULL;
}

ServiceInterface::Type	ServiceInterface::ToType
(
	const string& _type_string
)
{
	for(int i = 0 ; i < sizeof(si_type_info) / sizeof(ServiceInterface::TypeInfo); i++)
	{
		if (_type_string == si_type_info[i].name)
		{
			return	si_type_info[i].type;	
		}
	}

	return	UNKNOWN;
}

ServiceInterface::FieldType	ServiceInterface::ToFieldType
(
	const std::string& _field_name
)
{
	for(int i = 0 ; i < sizeof(field_info) / sizeof(ServiceInterfaceFieldInfo); i++)
	{
		if (_field_name == field_info[i].name)
		{
			return	field_info[i].type;	
		}
	}

	return	FIEID_UNDEFINED;
}

const std::string&	ServiceInterface::ToString
(
	ServiceInterface::Type _type
)
{
	for(int i = 0 ; i < sizeof(si_type_info) / sizeof(ServiceInterface::TypeInfo); i++)
	{
		if (_type== si_type_info[i].type)
		{
			return	si_type_info[i].name;	
		}
	}

	return	si_type_info[0].name;
}

ServiceInterface*	ServiceInterface::CreateReqDeviceCreate
(
	const std::string& _json
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Device::Properties*	properties = NULL;
	ServiceInterface*	si = NULL;

	INFO(NULL, "Create request device create!");
	if (libjson::is_valid(_json) != true)
	{
		ret_value = RET_VALUE_INVALID_FORMAT;
		ERROR(NULL, ret_value, "Invalid json format!");
		return	NULL;
	}

	JSONNode	root = libjson::parse(_json);

	for(auto it = root.begin(); (ret_value == RET_VALUE_OK) && (it != root.end()); it++)
	{
		FieldType field_type = ToFieldType(it->name());

		switch(field_type)
		{
		case	FIELD_DEVICE:
			{
				properties = Device::Properties::Create(it->as_node());
				if (properties == NULL)
				{
					ret_value = RET_VALUE_INVALID_FIELD;	
					ERROR(NULL, ret_value, "Failed to create device properties!");
				}
			}
			break;

		case	FIELD_MESSAGE:
			break;

		default:
			{
				ret_value = RET_VALUE_INVALID_FIELD;	
				ERROR(NULL, ret_value , "Unkown field[%s-%d]\n", it->name().c_str(), field_type);
			}
		}
	}

	if (properties == NULL)
	{
		ret_value = RET_VALUE_INVALID_FIELD;
		ERROR(NULL, ret_value, "Device information does not exist.");
	}

	if (ret_value == RET_VALUE_OK)
	{
		si = new SIReqDeviceCreate(properties);		
		if (si == NULL)
		{
			delete properties;
			ERROR(NULL, ret_value, "Failed to create message!");
			ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;	
		}
	}

	if (properties != NULL)
	{
		delete properties;
	}

	return	si;
}

ServiceInterface*	ServiceInterface::CreateReqDeviceDestroy
(
	const std::string& _json
)
{
	RetValue	ret_value = RET_VALUE_INVALID_ARGUMENTS;
	ServiceInterface*	si = NULL;
	string		device_id;

	INFO(NULL, "Create request device destroy!");
	if (libjson::is_valid(_json) != true)
	{
		ret_value = RET_VALUE_INVALID_FORMAT;
		ERROR(NULL, ret_value, "Invalid json format!");
		return	NULL;
	}

	JSONNode	root = libjson::parse(_json);

	for(auto it = root.begin(); (ret_value == RET_VALUE_OK) && (it != root.end()); it++)
	{
		FieldType field_type = ToFieldType(it->name());

		switch(field_type)
		{
		case	FIELD_ID:
			{
				device_id = it->as_string();
				ret_value = RET_VALUE_OK;
			}
			break;

		case	FIELD_MESSAGE:
			break;

		default:
			{
				ret_value = RET_VALUE_INVALID_FIELD;	
				ERROR(NULL, ret_value , "Unkown field[%s-%d]\n", it->name().c_str(), field_type);
			}
		}
	}


	if (ret_value == RET_VALUE_OK)
	{
		si = new SIReqDeviceDestroy(device_id);		
		if (si == NULL)
		{
			ERROR(NULL, ret_value, "Failed to create message!");
			ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;	
		}
	}


	return	si;
}

//////////////////////////////////////////////////////////////////////////////
// Service Interface 
//////////////////////////////////////////////////////////////////////////////
ServiceInterface*	ServiceInterface::RequestParser
(
	const std::string& json
)
{
	return	ServiceInterfaceRequest::Parser(json);
}

ServiceInterface*	ServiceInterface::ResponseParser
(
	const std::string& json
)
{
	return	ServiceInterfaceResponse::Parser(json);
}

//////////////////////////////////////////////////////////////////////////////
// Service Interface Request 
//////////////////////////////////////////////////////////////////////////////

ServiceInterface* ServiceInterfaceRequest::Parser
(
	const std::string& _json
)
{
	if (libjson::is_valid(_json) != true)
	{
		ERROR(NULL, RET_VALUE_INVALID_FORMAT, "Invalid json format!");
		return	NULL;
	}

	JSONNode	root = libjson::parse(_json);

	auto it = root.find("message");
	if (it == root.end())
	{
		ERROR(NULL, RET_VALUE_INVALID_FIELD, "Message type filed not found.");
		return	NULL;	
	}

	Type type = ServiceInterface::ToType(it->as_string());
	if (type == UNKNOWN)
	{
		ERROR(NULL, RET_VALUE_INVALID_TYPE, "Message type is invalid!");
		return	NULL;
	}

	ServiceInterface::TypeInfo* type_info = ServiceInterface::GetTypeInfo(type);
	if (type_info == NULL)
	{
		ERROR(NULL, RET_VALUE_INVALID_TYPE, "Message type is not supported.");
		return	NULL;
	}

	return	type_info->Creator(_json);
}

//////////////////////////////////////////////////////////////////////////////
// Service Interface Response
//////////////////////////////////////////////////////////////////////////////
ServiceInterface* ServiceInterfaceResponse::Parser
(
	const std::string& _json
)
{
	if (libjson::is_valid(_json) != true)
	{
		ERROR(NULL, RET_VALUE_INVALID_FORMAT, "Invalid json format!");
		return	NULL;
	}

	JSONNode	root("",_json);

	cout << "test" << endl;	
	cout << root.write_formatted() << endl;

	return	NULL;
}
//////////////////////////////////////////////////////////////////////////////
// Service Interface Device Create
//////////////////////////////////////////////////////////////////////////////

SIReqDeviceCreate::SIReqDeviceCreate
(
	Device::Properties* _properties
)
: ServiceInterfaceRequest(DEVICE_CREATE)
{
	properties = Device::Properties::Create(_properties->type);
	
	properties->Set(_properties);
}

SIReqDeviceCreate::~SIReqDeviceCreate()
{
	delete properties;
}

JSONNode SIReqDeviceCreate::ToJSON()
{
	JSONNode root;	

	root.push_back(JSONNode("message", "device_create"));
	root.push_back(properties->JSON());
	root[1].set_name("device");
	return	root;
}
	
//////////////////////////////////////////////////////////////////////////////
// Service Interface Device Destroy
//////////////////////////////////////////////////////////////////////////////

SIReqDeviceDestroy::SIReqDeviceDestroy
(
	const string&	_id
)
: ServiceInterfaceRequest(DEVICE_DESTROY)
{
	id_ = _id;
}


JSONNode SIReqDeviceDestroy::ToJSON()
{
	JSONNode root;	

	root.push_back(JSONNode("message", "device_destroy"));
	root.push_back(JSONNode("id", id_));

	return	root;
}
	
//////////////////////////////////////////////////////////////////////////////
// Service Interface Device Get Properties
//////////////////////////////////////////////////////////////////////////////

SIReqDeviceGetProperties::SIReqDeviceGetProperties
(
	const string&	_id
)
: ServiceInterfaceRequest(DEVICE_GET_PROPERTIES)
{
	id_ = _id;
}


JSONNode SIReqDeviceGetProperties::ToJSON()
{
	JSONNode root;	

	root.push_back(JSONNode("message", "device_get_properties"));
	root.push_back(JSONNode("id", id_));

	return	root;
}
	
