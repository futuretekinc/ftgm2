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

class	DataManager;
class	Device;
class	Endpoint;
class	JSONNode;

class	ObjectManager : public MessageProcess
{
public:
	ObjectManager();
	~ObjectManager();

	const 
	std::string	ClassName();

	RetValue	Connect(Device *_device);
	RetValue	Disconnect(Device *_device);
	uint32		DeviceCount();
	Device*		GetDevice(uint32 _index);
	Device*		GetDevice(const std::string& _id);
	void		ShowDeviceList();

	RetValue	Connect(Endpoint *_endpoint);
	RetValue	Disconnect(Endpoint *_endpoint);
	uint32		EndpointCount();
	Endpoint*	GetEndpoint(uint32 _index);
	Endpoint*	GetEndpoint(const std::string& _id);
	void		ShowEndpointList();

	RetValue	Connect(DataManager* _data_manager);
	RetValue	Disconnect(DataManager* _data_manager);
	DataManager* GetDataManager();

	RetValue	Load(std::string _file_name);

	void		OnMessage(Message* _message);
	void		OnQuit(Message* _message);


	friend std::ostream& operator<< (std::ostream& _os, const ObjectManager& _om);

protected:
	void		PreProcess();
	void		PostProcess();

	RetValue	LoadDevice(const JSONNode& _json_node);
	RetValue	LoadEndpoint(const JSONNode& _json_node);

	std::map<const std::string, Device *>		device_map_;
	std::map<const std::string, Endpoint *> 	endpoint_map_;
	DataManager*								data_manager_;
};

#endif
