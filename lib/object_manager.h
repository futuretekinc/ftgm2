#ifndef	OBJECT_MANAGER_H_
#define	OBJECT_MANAGER_H_

#include "defined.h"
#include <map>
#include <list>
#include <iostream>
#include <string>
#include <thread>
#include "object.h"
#include "device.h"
#include "endpoint.h"
#include "message_queue.h"
#include "message_process.h"
#include "tcp_server.h"

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

	RetValue	Load(std::string _file_name);
	RetValue	LoadFromDataManager();

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
	TCPServer*	GetTCPServer();

	RetValue	GetLoadedDeviceList(std::list<std::string>& _device_list);
	RetValue	GetLoadedEndpointList(std::list<std::string>& _endpoint_list);
	RetValue	SaveLoadedDevice(std::string& _device_id);
	RetValue	SaveLoadedDevices();
	RetValue	SaveLoadedEndpoint(std::string& _endpoint_id);
	RetValue	SaveLoadedEndpoints();

	void		OnMessage(Message* _message);
	void		OnQuit(Message* _message);


	friend std::ostream& operator<< (std::ostream& _os, const ObjectManager& _om);

protected:
	void		PreProcess();
	void		PostProcess();

	RetValue	Connect(Device *_device);
	RetValue	Disconnect(Device *_device);
	RetValue	LoadDevices(const JSONNode& _json_node);
	RetValue	LoadDevice(const JSONNode& _json_node);
	RetValue	SaveDevice(Device* _device);

	RetValue	Connect(Endpoint *_endpoint);
	RetValue	Disconnect(Endpoint *_endpoint);
	RetValue	LoadEndpoints(const JSONNode& _json_node);
	RetValue	LoadEndpoint(const JSONNode& _json_node);
	RetValue	SaveEndpoint(Endpoint* _endpoint);

	std::list<std::string>				load_device_list_;
	std::list<std::string>				load_endpoint_list_;

	std::map<const std::string, Device *>		device_map_;
	std::map<const std::string, Endpoint *>		endpoint_map_;
	DataManager*								data_manager_;
	TCPServer*									tcp_server_;
};

#endif
