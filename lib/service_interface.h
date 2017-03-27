#ifndef	SERVICE_INTERFACE_H_
#define	SERVICE_INTERFACE_H_

#include <string>
#include "device.h"

class	ServiceInterfaceRequest;
class	ServiceInterfaceResponse;
class	SIReqDeviceCreate;

class	ServiceInterface
{
public:
	enum	Type
	{
		UNKNOWN,
		DEVICE_CREATE,
		DEVICE_DESTROY,
		DEVICE_GET_PROPERTIES,
		DEVICE_SET_PROPERTIES,
		DEVICE_GET_LIST,

		ENDPOINT_CREATE,
		ENDPOINT_DESTROY,
		ENDPOINT_GET_PROPERTIES,
		ENDPOINT_SET_PROPERTIES,
		ENDPOINT_GET_LIST,
	};

	struct	TypeInfo
	{
		Type		type;
		std::string	name;
		ServiceInterface*		(*Creator)(const std::string& _json);
	};

	enum	FieldType
	{
		FIEID_UNDEFINED,
		FIELD_MESSAGE,
		FIELD_ID,
		FIELD_NAME,
		FIELD_ENABLE,
		FIELD_COUNT,
		FIELD_DEVICE,
		FIELD_DEVICES,
		FIELD_ENDPOINT,
		FIELD_ENDPOINTS
	};

	virtual ~ServiceInterface()	{};

	static
	TypeInfo*	GetTypeInfo(Type _type);
	static
	TypeInfo*	GetTypeInfo(const std::string& _type_name);

	static
	Type	ToType(const std::string& _type_string);
	static
	FieldType	ToFieldType(const std::string& _field_name);

	static const
	std::string&	ToString(Type _type);

	virtual 
	JSONNode	ToJSON() = 0;

	static
	ServiceInterface*	RequestParser(const std::string& json);

	static
	ServiceInterface*	ResponseParser(const std::string& json);

	static
	ServiceInterface*	CreateReqDeviceCreate(const std::string& _json);
	static
	ServiceInterface*	CreateReqDeviceDestroy(const std::string& _json);
	static
	ServiceInterface*	CreateReqDeviceSetProperties(const std::string& _json);
	static
	ServiceInterface*	CreateReqDeviceGetProperties(const std::string& _json);

protected:
	ServiceInterface(Type	_type) : type_(_type) {};

	Type	type_;
};

class	ServiceInterfaceRequest : public ServiceInterface
{
public:
	static
	ServiceInterface* Parser(const std::string& _payload);

protected:
	ServiceInterfaceRequest(Type _type) : ServiceInterface(_type) {};

	std::string		payload_;
};

class	ServiceInterfaceResponse: public ServiceInterface
{
public:
	static
	ServiceInterface* Parser(const std::string& _payload);

	JSONNode ToJSON();

protected:
	ServiceInterfaceResponse(Type _type) : ServiceInterface(_type) {};
};

//////////////////////////////////////////////////////////////////////////////
// Service Interface Device Create
//////////////////////////////////////////////////////////////////////////////
class	SIReqDeviceCreate : public ServiceInterfaceRequest
{
public:
	SIReqDeviceCreate(Device::Properties* _properties);
	~SIReqDeviceCreate();

	JSONNode ToJSON();

protected:
	Device::Properties* properties;
};

class	SIRespDeviceCreate : public ServiceInterfaceResponse
{
public:
	SIRespDeviceCreate();

};

//////////////////////////////////////////////////////////////////////////////
// Service Interface Device Destroy
//////////////////////////////////////////////////////////////////////////////
class	SIReqDeviceDestroy : public ServiceInterfaceRequest
{
public:
	SIReqDeviceDestroy(const std::string& _id);

	JSONNode ToJSON();
protected:
	std::string	id_;
};

//////////////////////////////////////////////////////////////////////////////
// Service Interface Device Get Properties
//////////////////////////////////////////////////////////////////////////////
class	SIReqDeviceGetProperties : public ServiceInterfaceRequest
{
public:
	SIReqDeviceGetProperties(const std::string& _id);

	JSONNode ToJSON();
protected:
	std::string	id_;
};

//////////////////////////////////////////////////////////////////////////////
// Service Interface Device Set Properties
//////////////////////////////////////////////////////////////////////////////
class	SIReqDeviceSetProperties : public ServiceInterfaceRequest
{
public:
	SIReqDeviceSetProperties(const std::string& _id);
	~SIReqDeviceSetProperties();

	JSONNode ToJSON();

protected:
	std::string	id_;
};


#endif
