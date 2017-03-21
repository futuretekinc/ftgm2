#ifndef	OBJECT_MANAGER_H_
#define	OBJECT_MANAGER_H_

#include "defined.h"
#include <map>
#include <iostream>
#include <string>
#include <thread>
#include "object.h"
#include "device.h"
#include "endpoint.h"
#include "message_queue.h"
#include "message_process.h"

class	JSONNode;
class	DataManager;

class	ObjectManager : public MessageProcess
{
	friend	class	DataManager;
	friend	class	Device;
	friend 	class	Endpoint;
public:
	ObjectManager();
	~ObjectManager();

	RetValue	Destroy(const std::string& _device_id);

	RetValue	CreateDevice(Device::Properties *_properties);
	RetValue	DestroyDevice(const std::string& _id);

	uint32		GetDeviceCount();
	Device*		GetDevice(uint32 _index);
	Device*		GetDevice(const std::string& _id);

	virtual
	RetValue	SetDeviceProperty(const std::string& _id, const std::string& _field, const std::string& _value);
	virtual
	RetValue	SetDeviceProperty(const std::string& _id, const std::string& _field, bool _value);
	virtual
	RetValue	SetDeviceProperty(const std::string& _id, const std::string& _field, uint32_t _value);
	RetValue	SetDeviceActivation(const std::string& _id, bool _activation);

	void		ShowDeviceList();

	RetValue	CreateEndpoint(Endpoint::Properties *_properties);
	RetValue	DestroyEndpoint(const std::string& _id);

	uint32		GetEndpointCount();
	Endpoint*	GetEndpoint(uint32 _index);
	Endpoint*	GetEndpoint(const std::string& _id);

	virtual
	RetValue	SetEndpointProperty(const std::string& _id, const std::string& _field, const std::string& _value);
	virtual
	RetValue	SetEndpointProperty(const std::string& _id, const std::string& _field, bool _value);
	virtual
	RetValue	SetEndpointProperty(const std::string& _id, const std::string& _field, uint32_t _value);
	RetValue	SetEndpointActivation(const std::string& _id, bool _activation);

	void		ShowEndpointList();

	RetValue	Connect(DataManager* _data_manager);
	RetValue	Disconnect(DataManager* _data_manager);
	DataManager* GetDataManager();

	RetValue	Load(std::string _file_name);
	RetValue	LoadFromDataManager();

	void		OnMessage(Message* _message);
	void		OnQuit(Message* _message);


	friend std::ostream& operator<< (std::ostream& _os, const ObjectManager& _om);

protected:
	void		PreProcess();
	void		PostProcess();

	RetValue	Connect(Device *_device);
	RetValue	Disconnect(Device *_device);
	RetValue	LoadDevice(const JSONNode& _json_node);
	RetValue	SaveDevice(Device* _device);

	RetValue	Connect(Endpoint *_endpoint);
	RetValue	Disconnect(Endpoint *_endpoint);
	RetValue	LoadEndpoint(const JSONNode& _json_node);
	RetValue	SaveEndpoint(Endpoint* _endpoint);

	std::map<const std::string, Device *>		device_map_;
	std::map<const std::string, Endpoint *>		endpoint_map_;
	DataManager*								data_manager_;
};

#endif
