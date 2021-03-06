#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <exception>
#include <chrono>
#include <unistd.h>
#include "trace.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"
#include "endpoint.h"
#include "libjson/libjson.h"

using namespace	std;

ObjectManager::ObjectManager()
:	MessageProcess()
{
	tcp_server_ = new TCPServer(this);
}

ObjectManager::~ObjectManager()
{
	if (tcp_server_ != NULL)
	{
		delete tcp_server_;
	}
}

void	ObjectManager::PreProcess()
{
	INFO(this, "Pre-process start.");
	if (data_manager_ != NULL)
	{
		data_manager_->Start(1000);	

		LoadFromDataManager();
	}

	if (tcp_server_ != NULL)
	{
		tcp_server_->Start();	
	}

	INFO(this, "Pre-process done.");
}

void	ObjectManager::PostProcess()
{
	if (tcp_server_ != NULL)
	{
		tcp_server_->Stop();	
	}

	if (data_manager_ != NULL)
	{
		data_manager_->Stop();	
	}
}
/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
RetValue	ObjectManager::Load
(
	const string file_name
)
{
	RetValue	ret_value = RET_VALUE_OK;

	try
	{
		ifstream ifs(file_name.c_str());

		if (!ifs.is_open())
		{
			ret_value = RET_VALUE_INVALID_CONFIG_FILE;
			ERROR(this, ret_value, "Failed to load configuration!");

			return	ret_value;
		}

		filebuf* pbuf = ifs.rdbuf();

		size_t size = pbuf->pubseekoff (0,ifs.end, ifs.in);
		pbuf->pubseekpos (0,ifs.in);

		char* buffer=new char[size + 1];
		memset(buffer, 0, size+1);

		pbuf->sgetn (buffer, size);
		ifs.close();

		INFO(this, "Load configuration[%s]", file_name.c_str());
			INFO(this, buffer);
		TRACE_PUSH();
		if (libjson::is_valid(buffer) == true)
		{
			JSONNode	root = libjson::parse(buffer);
			for(size_t i = 0 ; (ret_value == RET_VALUE_OK) && (i < root.size()) ; i++)
			{
				if (root[i].name() == "devices")
				{
					ret_value = LoadDevices(root[i]);	
				}
				else if (root[i].name() == "device")
				{
					ret_value = LoadDevice(root[i]);	
				}
				else if (root[i].name() == "endpoints")
				{
					ret_value = LoadEndpoints(root[i]);	
				}
				else if (root[i].name() == "endpoint")
				{
					ret_value = LoadEndpoint(root[i]);	
				}
			}
		}
		else
		{
			ret_value = RET_VALUE_INVALID_CONFIG_FILE;
			INFO(this, buffer);
		}

		if (ret_value != RET_VALUE_OK)
		{
			ERROR(this, ret_value, "Failed to load configuration!");	
		}
		TRACE_POP();

		delete	[] buffer;

		return	ret_value;
	}
	catch(exception& e)
	{
		ret_value = RET_VALUE_EXCEPTION;
		ERROR(this, ret_value, "***"/*e.what()*/);

		return	ret_value;
	}
}

////////////////////////////////////////////////////////////////
//	for Device
////////////////////////////////////////////////////////////////
RetValue	ObjectManager::CreateDevice
(
	Device::Properties *_properties
)
{
	RetValue	ret_value;
	Device*		device;

	if (data_manager_ != NULL)
	{ 
		if (data_manager_->IsDeviceExist(_properties->id))
		{
			INFO(this, "The device[%s] is already registered.", _properties->id.c_str());
			return	RET_VALUE_OBJECT_EXISTS;
		}
	}
	else
	{
		INFO(this, "Data manager is not connected!");
	
	}

	device = Device::Create(_properties);
	if (device == NULL)
	{
		ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;	
		ERROR(this, ret_value, "Failed to create device!");
		return	ret_value;	
	}

	if (data_manager_ != NULL)
	{
		ret_value = data_manager_->AddDevice(_properties);	
		if (ret_value != RET_VALUE_OK)
		{
			ERROR(this, ret_value, "Failed to add device to data manager!");
			delete device;
			return	ret_value;
		}
	}

	ret_value = Connect(device);
	if (ret_value != RET_VALUE_OK)
	{
		INFO(this, "Failed to connect deivce!" );
		data_manager_->DeleteDevice(_properties->id);	
		delete device;
		return	ret_value;
	}

	INFO(this, "The device[%s] was successfully created!", _properties->id.c_str());

	return	ret_value;
}

RetValue	ObjectManager::DestroyDevice
(
	const	string&	_id
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Device*		device = NULL;

	auto it = device_map_.find(_id);
	if (it == device_map_.end())
	{
		return	RET_VALUE_OBJECT_NOT_FOUND;
	}

	device = it->second;

	ret_value = data_manager_->DeleteDevice(_id);
	if (ret_value != RET_VALUE_OK)
	{
		ERROR(this, ret_value, "Failed to delete device from data manager!");	
	}

	device_map_.erase(it);

	delete device;

	return	RET_VALUE_OK;
}

RetValue	ObjectManager::Connect
(
	Device *_device
)
{
	RetValue	ret_value = RET_VALUE_OK;

	auto it = device_map_.find(_device->GetID());
	if (it == device_map_.end())
	{
		INFO(this, "The device[%s] was connected.", _device->GetID().c_str());
		device_map_[_device->GetID()] = _device;

		_device->object_manager_ = this;
	}

	return	ret_value;
}

RetValue	ObjectManager::Disconnect
(
	Device*	_device
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (device_map_.erase(_device->GetID()) != 0)
	{
		_device->ReleaseParent();
	}
	else
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
	}

	return	ret_value;
}

uint32		ObjectManager::GetDeviceCount()
{
	return	device_map_.size();
}

Device*		ObjectManager::GetDevice
(
	uint32 _index
)
{
	if (_index < device_map_.size())
	{
		auto it = device_map_.begin();
		for(; it != device_map_.end() ; it++)
		{
			if (_index == 0)
			{
				return	it->second;	
			}
		}		
	}

	return	NULL;
}

Device*		ObjectManager::GetDevice
(
	const string& _id
)
{
	auto it = device_map_.begin();
	for(; it != device_map_.end() ; it++)
	{
		if (it->first == _id)
		{
			return	it->second;	
		}
	}		

	return	NULL;
}

RetValue	ObjectManager::LoadDevices
(
	const	JSONNode& _json_node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Load Devices");
	TRACE_PUSH();
	if(_json_node.type() == JSON_ARRAY)
	{
		for(int i = 0 ; i < (int)_json_node.size() ; i++)
		{
			ret_value = LoadDevice(_json_node[i]);	
			if (ret_value != RET_VALUE_OK)
			{
				break;	
			}
		}
	}
	TRACE_POP();

	return	ret_value;
}

RetValue	ObjectManager::LoadDevice
(
	const	JSONNode& _json_node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Load Device");
	TRACE_PUSH();

	if (_json_node.type() == JSON_NODE)
	{
		Device* device = NULL;

		JSONNode::const_iterator it=_json_node.find("type");
		if (it == _json_node.end())
		{
			ret_value = RET_VALUE_INVALID_TYPE;
			ERROR(this, RET_VALUE_INVALID_TYPE, "type not found!");
		}
		else
		{
			Device::Properties*	device_properties;
			list<Endpoint::Properties*>	epp_list;
			
			device_properties = Device::Properties::Create(_json_node);
			if (device_properties != NULL)
			{
				it = _json_node.find("endpoints");
				if (it != _json_node.end())
				{
					ret_value = Endpoint::Properties::Create(*it, epp_list);
					if (ret_value == RET_VALUE_OK)
					{
						for(auto epp_it = epp_list.begin();epp_it != epp_list.end(); epp_it++)
						{
							(*epp_it)->device_id = device_properties->id;
						}
					}
					else
					{
						delete device_properties;

						device_properties = NULL;
					}
				}
			}

			if (device_properties != NULL)
			{
				device = Device::Create(device_properties);
				if (device != NULL)
				{
					ret_value = Connect(device);
					if (ret_value == RET_VALUE_OK)
					{
						load_device_list_.push_back(device->GetID());	

						for(auto epp_it = epp_list.begin(); epp_it != epp_list.end() ; epp_it++)
						{
							Endpoint *endpoint;

							endpoint = Endpoint::Create(*epp_it);
							if (endpoint != NULL)
							{
								ret_value = Connect(endpoint);
								if (ret_value == RET_VALUE_OK)
								{
									load_endpoint_list_.push_back(endpoint->GetID());	
								}
							}
						}
					}
					else
					{
						ERROR(this, ret_value, "Failed to attach device to manager!");
						delete device;
					}
				}
				else
				{
					ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;
					ERROR(this, ret_value, "Failed to create device!");	
				}

				delete device_properties;
				for(auto epp_it = epp_list.begin(); epp_it != epp_list.end() ; epp_it++)
				{
					delete *epp_it;
				}
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_INVALID_TYPE;
		ERROR(this, ret_value, "Object is not node!");
	}
	TRACE_POP();

	return	ret_value;
}

RetValue	ObjectManager::SetDeviceProperty
(
	const string& _id,
	const string& _field,
	const string& _value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	Device*		device = GetDevice(_id);
	if (device == NULL)
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
		ERROR(this, ret_value, "Failed to set device name!");
		return	ret_value;
	}

	ret_value = device->SetProperty(_field, _value);
	if (ret_value == RET_VALUE_OK)
	{
		ret_value = data_manager_->SetDeviceProperties(device->GetProperties());
	}

	if (ret_value != RET_VALUE_OK)
	{
		ERROR(this, ret_value, "Failed to set device property[%s:%s]!", _field.c_str(), _value.c_str());	
	}

	return	ret_value;
}

RetValue	ObjectManager::SetDeviceProperty
(
	const string& _id,
	const string& _field,
	bool		_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	Device*		device = GetDevice(_id);
	if (device == NULL)
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
		ERROR(this, ret_value, "Failed to set device name!");
		return	ret_value;
	}

	ret_value = device->SetProperty(_field, _value);
	if (ret_value == RET_VALUE_OK)
	{
		ret_value = data_manager_->SetDeviceProperties(device->GetProperties());
	}

	if (ret_value != RET_VALUE_OK)
	{
		ERROR(this, ret_value, "Failed to set device property[%s:%d]!", _field.c_str(), _value);	
	}

	return	ret_value;
}

RetValue	ObjectManager::SetDeviceProperty
(
	const string& 	_id,
	const string& 	_field,
	uint32_t		_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	Device*		device = GetDevice(_id);
	if (device == NULL)
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
		ERROR(this, ret_value, "Failed to set device name!");
		return	ret_value;
	}

	ret_value = device->SetProperty(_field, _value);
	if (ret_value == RET_VALUE_OK)
	{
		ret_value = data_manager_->SetDeviceProperties(device->GetProperties());
	}

	if (ret_value != RET_VALUE_OK)
	{
		ERROR(this, ret_value, "Failed to set device property[%s:%u]!", _field.c_str(), _value);	
	}

	return	ret_value;
}

RetValue	ObjectManager::SetDeviceActivation
(
	const string& _id,
	bool _activation
)
{
	RetValue	ret_value = RET_VALUE_OK;

	Device*		device = GetDevice(_id);
	if (device == NULL)
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
		ERROR(this, ret_value, "Failed to set device name!");
		return	ret_value;
	}

	if (_activation)
	{
		return	device->Start();
	}
	else
	{
		return	device->Stop();
	}
}

void	ObjectManager::ShowDeviceList()
{
	size_t	name_width 	= 16;
	size_t	id_width	= 16;
	size_t	type_width	= 8;
	size_t	enable_width	= 8;
	size_t	activation_width	= 11;

	auto it = device_map_.begin();
	for(; it != device_map_.end() ; it++)
	{
		if (name_width < it->second->GetName().length())
		{
			name_width = it->second->GetName().length();
		}

		if (id_width < it->second->GetID().length())
		{
			id_width = it->second->GetID().length();
		}

		const string	type = Device::TypeToString(it->second->GetType());
		if (type_width < type.length())
		{
			type_width = type.length();
		}
	}

	cout << setw(name_width) << "Name" << " ";
	cout << setw(id_width) << "ID" << " " ;
	cout << setw(type_width) << "Type" << " " ;
	cout << setw(enable_width) << "Enable" << " ";
	cout << setw(activation_width) << "Activation" << endl;
	it = device_map_.begin();
	for(; it != device_map_.end() ; it++)
	{
		cout << setw(name_width) << it->second->GetName();
		cout << " " << setw(id_width) << it->second->GetID();
		cout << " " << setw(type_width) << Device::TypeToString(it->second->GetType());
		if (it->second->GetEnable())
		{
			cout << " " << setw(enable_width) << "on";
		}
		else
		{
			cout << " " << setw(enable_width) << "off";
		}

		if (it->second->IsRun())
		{
			cout << " " << setw(activation_width) << "on";
		}
		else
		{
			cout << " " << setw(activation_width) << "off";
		}
		cout << endl;
	}

}

RetValue	ObjectManager::SaveDevice
(
	Device* _device 
)
{
	return	data_manager_->SetDeviceProperties(_device->GetProperties());
}

////////////////////////////////////////////////////////////////
//	for Endpoint
////////////////////////////////////////////////////////////////
RetValue	ObjectManager::CreateEndpoint
(
	Endpoint::Properties *_properties
)
{
	RetValue	ret_value;
	Device*		device;
	Endpoint*	endpoint;

	if (data_manager_ != NULL)
	{ 
		if (data_manager_->IsEndpointExist(_properties->id))
		{
			ret_value = RET_VALUE_OBJECT_EXISTS;
			ERROR(this, ret_value, "Endpoint[%s] already exist.", _properties->id.c_str());
			return	ret_value;
		}

		if (!data_manager_->IsDeviceExist(_properties->device_id))
		{
			ret_value = RET_VALUE_DEVICE_DOES_NOT_EXIST;
			ERROR(this, ret_value, "Device[%s] does not exist.", _properties->device_id.c_str());
			return	ret_value;
		}
	}

	device = GetDevice(_properties->device_id);
	if (device == NULL)
	{
		ret_value= RET_VALUE_DEVICE_DOES_NOT_EXIST;
		ERROR(this, ret_value, "Device[%s] does not exist.", _properties->device_id.c_str());
		return	RET_VALUE_DEVICE_DOES_NOT_EXIST;
	}

	endpoint = Endpoint::Create(_properties);
	if (endpoint == NULL)
	{
		ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;	
		ERROR(this, ret_value, "Failed to creat endpoint.");
		return	ret_value;	
	}

	if (data_manager_ != NULL)
	{
		ret_value = data_manager_->AddEndpoint(_properties);	
		if (ret_value != RET_VALUE_OK)
		{
			ERROR(this, ret_value, "Failed to add endpoint to data manager!");
			delete endpoint;
			return	ret_value;
		}
	}

	ret_value = Connect(endpoint);
	if (ret_value != RET_VALUE_OK)
	{
		INFO(this, "Failed to connect endpoint!" );
		data_manager_->DeleteEndpoint(_properties->id);	
		delete endpoint;
		return	ret_value;
	}

	INFO(this, "The endpoin[%s] was successfully created!", _properties->id.c_str());

	return	ret_value;
}

RetValue	ObjectManager::DestroyEndpoint
(
	const	string&	_id
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Endpoint*	endpoint = NULL;

	auto it = endpoint_map_.find(_id);
	if (it == endpoint_map_.end())
	{
		return	RET_VALUE_OBJECT_NOT_FOUND;
	}

	endpoint = it->second;

	ret_value = data_manager_->DeleteEndpoint(_id);
	if (ret_value != RET_VALUE_OK)
	{
		ERROR(this, ret_value, "Failed to delete endpoint from data manager!");	
	}

	endpoint_map_.erase(it);

	delete endpoint;

	return	RET_VALUE_OK;
}

RetValue	ObjectManager::Connect
(
	Endpoint *_endpoint
)
{
	RetValue	ret_value = RET_VALUE_OK;

	auto it = endpoint_map_.find(_endpoint->GetID());
	if (it == endpoint_map_.end())
	{
		INFO(this, "The endpoint[%s] was connected.", _endpoint->GetID().c_str());
		endpoint_map_[_endpoint->GetID()] = _endpoint;

		_endpoint->object_manager_ = this;
	}

	return	ret_value;
}

RetValue	ObjectManager::Disconnect
(
	Endpoint*	_endpoint
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (endpoint_map_.erase(_endpoint->GetID()) != 0)
	{
		_endpoint->ReleaseParent();
	}
	else
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
	}

	return	ret_value;
}

uint32		ObjectManager::GetEndpointCount()
{
	return	endpoint_map_.size();
}

Endpoint*		ObjectManager::GetEndpoint
(
	uint32 _index
)
{
	auto it = endpoint_map_.begin();
	for(; it != endpoint_map_.end() ; _index--, it++)
	{
		if (_index == 0)
		{
			return	it->second;
		}
	}		

	return	NULL;
}

Endpoint*	ObjectManager::GetEndpoint
(
	const string& _id
)
{
	auto it = endpoint_map_.find(_id);
	if (it != endpoint_map_.end())
	{
		return	it->second;
	}		

	return	NULL;
}

RetValue	ObjectManager::SetEndpointProperty
(
	const string& _id,
	const string& _field,
	const string& _value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	Endpoint*		endpoint = GetEndpoint(_id);
	if (endpoint == NULL)
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
		ERROR(this, ret_value, "Failed to set endpoint name!");
		return	ret_value;
	}

	ret_value = endpoint->SetProperty(_field, _value);
	if (ret_value == RET_VALUE_OK)
	{
		ret_value = data_manager_->SetEndpointProperties(endpoint->GetProperties());
	}

	if (ret_value != RET_VALUE_OK)
	{
		ERROR(this, ret_value, "Failed to set endpoint property[%s:%s]!", _field.c_str(), _value.c_str());	
	}

	return	ret_value;
}

RetValue	ObjectManager::SetEndpointProperty
(
	const string& _id,
	const string& _field,
	bool		_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	Endpoint*		endpoint = GetEndpoint(_id);
	if (endpoint == NULL)
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
		ERROR(this, ret_value, "Failed to set endpoint name!");
		return	ret_value;
	}

	ret_value = endpoint->SetProperty(_field, _value);
	if (ret_value == RET_VALUE_OK)
	{
		ret_value = data_manager_->SetEndpointProperties(endpoint->GetProperties());
	}

	if (ret_value != RET_VALUE_OK)
	{
		ERROR(this, ret_value, "Failed to set endpoint property[%s:%d]!", _field.c_str(), _value);	
	}

	return	ret_value;
}

RetValue	ObjectManager::SetEndpointProperty
(
	const string& 	_id,
	const string& 	_field,
	uint32_t		_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	Endpoint*		endpoint = GetEndpoint(_id);
	if (endpoint == NULL)
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
		ERROR(this, ret_value, "Failed to set endpoint name!");
		return	ret_value;
	}

	ret_value = endpoint->SetProperty(_field, _value);
	if (ret_value == RET_VALUE_OK)
	{
		ret_value = data_manager_->SetEndpointProperties(endpoint->GetProperties());
	}

	if (ret_value != RET_VALUE_OK)
	{
		ERROR(this, ret_value, "Failed to set endpoint property[%s:%u]!", _field.c_str(), _value);	
	}

	return	ret_value;
}

RetValue	ObjectManager::LoadEndpoints
(
	const	JSONNode& _json_node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Start loading endpoint arrays.");
	TRACE_PUSH();

	if(_json_node.type() == JSON_ARRAY)
	{
		for(int i = 0 ; i < (int)_json_node.size() ; i++)
		{
			ret_value = LoadEndpoint(_json_node[i]);	
			if (ret_value != RET_VALUE_OK)
			{
				break;
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_INVALID_TYPE;
	}

	if (ret_value == RET_VALUE_OK)
	{
		INFO(this, "Endpoint array load finished.");
	}
	else
	{
		ERROR(this, ret_value, "An error occurred while loading the endpoint array!");
	}

	TRACE_POP();

	return	ret_value;
}


RetValue	ObjectManager::LoadEndpoint
(
	const	JSONNode& _json_node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Start loading endpoint.");
	TRACE_PUSH();
	if (_json_node.type() == JSON_NODE)
	{
		Endpoint* endpoint = NULL;

		JSONNode::const_iterator it=_json_node.find("type");
		if (it != _json_node.end())
		{
			Endpoint::Type	type = (Endpoint::Type)it->as_int();
			Endpoint::Properties*	properties = Endpoint::Properties::Create(type);
			if (properties != NULL)
			{
				ret_value = properties->Set(_json_node);
				if (ret_value == RET_VALUE_OK)
				{
					ret_value = CreateEndpoint(properties);
					if (ret_value == RET_VALUE_OK)
					{
						load_endpoint_list_.push_back(properties->id);	
					}
					else
					{
						ERROR(this, ret_value, "Failed to create endpoint!");	
					}
				}
				else
				{
					ERROR(this, ret_value, "Failed to set properties!");	
				}

				delete properties;
			}
		}
		else
		{
			ret_value = RET_VALUE_INVALID_TYPE;
		}
	}
	else
	{
		ret_value = RET_VALUE_INVALID_TYPE;
	}

	if (ret_value == RET_VALUE_OK)
	{
		INFO(this, "The endpoint load finished!");
	}
	else
	{
		ERROR(this, ret_value, "Failed to load endpoint!");
	}
	TRACE_POP();

	return	ret_value;
}

RetValue	ObjectManager::SaveEndpoint
(
	Endpoint* _endpoint
)
{
	return	data_manager_->SetEndpointProperties(_endpoint->GetProperties());
}

///////////////////////////////////////////////////////
// for DataManager
///////////////////////////////////////////////////////
RetValue	ObjectManager::Connect
(
	DataManager*	_data_manager
)
{
	if (data_manager_ == _data_manager)
	{
		return	RET_VALUE_OK;
	}

	data_manager_ = _data_manager;

	return	RET_VALUE_OK;
}

RetValue	ObjectManager::LoadFromDataManager()
{
	uint32_t	device_count = data_manager_->GetDeviceCount();
	if (device_count == 0)
	{
		return	RET_VALUE_OK;
	}

	std::list<Device::Properties*>	device_list;

	RetValue ret_value = data_manager_->GetDeviceProperties(0, device_count, device_list);
	if (ret_value != RET_VALUE_OK)
	{
		return	ret_value;	
	}

	std::list<Device::Properties*>::iterator device_it = device_list.begin();
	while(device_it != device_list.end())
	{
		Device*	device = Device::Create(*device_it);
		if (device != NULL)
		{
			INFO(this, "The device has been created.");
			ret_value = Connect(device);
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to connect device to manager!");

				delete device;
			}
		}

		device_it++;
	}

	INFO(this, "Device loading completed");
	uint32_t	endpoint_count = data_manager_->GetEndpointCount();
	if (endpoint_count == 0)
	{
		return	RET_VALUE_OK;
	}

	std::list<Endpoint::Properties*>	endpoint_list;

	ret_value = data_manager_->GetEndpointProperties(0, endpoint_count, endpoint_list);
	if (ret_value != RET_VALUE_OK)
	{
		return	ret_value;	
	}

	std::list<Endpoint::Properties*>::iterator endpoint_it = endpoint_list.begin();
	while(endpoint_it != endpoint_list.end())
	{
		Endpoint*	endpoint = Endpoint::Create(*endpoint_it);
		if (endpoint != NULL)
		{
			ret_value = Connect(endpoint);
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to connect endpoint to manager!");

				delete endpoint;
			}
		}

		endpoint_it++;
	}
	INFO(this, "Endpoint loading completed");


	INFO(this, "Object loading completed");
	return	RET_VALUE_OK;
}

RetValue	ObjectManager::Disconnect
(
	DataManager*	_data_manager
)
{
	if (data_manager_ == _data_manager)
	{
		data_manager_ = NULL;
	}

	return	RET_VALUE_OK;
}

DataManager* ObjectManager::GetDataManager()
{
	return	data_manager_;
}

TCPServer*	ObjectManager::GetTCPServer()
{
	return	tcp_server_;
}

void	ObjectManager::OnMessage
(
	Message* _base_message
)
{
	INFO(this, "Message[%s] recieved.", ToString(_base_message).c_str());
	switch(_base_message->type)
	{
	case	Message::TYPE_STARTED:
		{
			MessageStart* message = (MessageStart *)_base_message;

			if (message->id == string(""))
			{
				auto it = device_map_.begin();
				for(; it != device_map_.end(); it++)
				{
					it->second->Start();
				}

				auto endpoint_it = endpoint_map_.begin();
				for(; endpoint_it != endpoint_map_.end(); endpoint_it++)
				{
					INFO(this, "Endpoint[%s] : Device[%s]", endpoint_it->second->GetID().c_str(), endpoint_it->second->GetDeviceID().c_str());

					endpoint_it->second->Start();
				}
			}
			else
			{
				auto it = device_map_.find(message->id);
				if (it != device_map_.end())
				{
					it->second->Start();
					INFO(this, "Device[%s] started.", it->second->GetID().c_str());

					auto endpoint_it = endpoint_map_.begin();
					for(; endpoint_it != endpoint_map_.end(); endpoint_it++)
					{
						if (endpoint_it->second->GetDeviceID() == message->id)
						{
							endpoint_it->second->Start();
						}
					}
				}
			}
		}
		break;

	case	Message::TYPE_STOP:
		{
			MessageStop* message = (MessageStop *)_base_message;

			if (message->id == string(""))
			{
				auto it = device_map_.begin();
				for(; it != device_map_.end(); it++)
				{
					it->second->Stop();
				}

				auto endpoint_it = endpoint_map_.begin();
				for(; endpoint_it != endpoint_map_.end(); endpoint_it++)
				{
					endpoint_it->second->Stop();
				}
			}
			else
			{
				auto it = device_map_.find(message->id);
				if (it != device_map_.end())
				{
					INFO(this, "Device[%s] stopped.", it->second->GetID().c_str());
					it->second->Stop();

					auto endpoint_it = endpoint_map_.begin();
					for(; endpoint_it != endpoint_map_.end(); endpoint_it++)
					{
						if (endpoint_it->second->GetDeviceID() == message->id)
						{
							endpoint_it->second->Stop();
						}
					}
				}
			}
		}
		break;

	default:
		{
			MessageProcess::OnMessage(_base_message);
		}
	}
}

RetValue	ObjectManager::GetLoadedDeviceList
(
	list<string>& _device_list
)
{
	_device_list = load_device_list_;

	return	RET_VALUE_OK;
}

RetValue	ObjectManager::GetLoadedEndpointList
(
	list<string>& _endpoint_list
)
{
	_endpoint_list = load_endpoint_list_;

	return	RET_VALUE_OK;
}

RetValue	ObjectManager::SaveLoadedDevice
(
	string& _device_id
)
{
	RetValue	ret_value = RET_VALUE_OBJECT_NOT_FOUND;

	for(auto it = load_device_list_.begin(); it != load_device_list_.end(); it++)
	{
		if (*it == _device_id)
		{
			Device *device = GetDevice(_device_id);
			if (device != NULL)
			{
				ret_value = data_manager_->AddDevice(device->GetProperties());	
				if (ret_value != RET_VALUE_OK)
				{
					ERROR(this, ret_value, "Failed to add device[%s]!", it->c_str());		
				}
			}

			break;
		}
	}

	return	ret_value;
}

RetValue	ObjectManager::SaveLoadedDevices()
{
	RetValue	ret_value = RET_VALUE_OBJECT_NOT_FOUND;

	for( auto it = load_device_list_.begin(); it != load_device_list_.end(); it++)
	{
		Device *device = GetDevice(*it);
		if (device != NULL)
		{
			ret_value = data_manager_->AddDevice(device->GetProperties());	
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to add device[%s]!", it->c_str());		
				break;
			}
		}
	}

	return	ret_value;
}

RetValue	ObjectManager::SaveLoadedEndpoint
(
	std::string& _endpoint_id
)
{
	RetValue	ret_value = RET_VALUE_OBJECT_NOT_FOUND;

	for( auto it = load_endpoint_list_.begin(); it != load_endpoint_list_.end(); it++)
	{
		if (*it == _endpoint_id)
		{
			Endpoint *endpoint = GetEndpoint(_endpoint_id);
			if (endpoint != NULL)
			{
				ret_value = data_manager_->AddEndpoint(endpoint->GetProperties());	
				if (ret_value != RET_VALUE_OK)
				{
					ERROR(this, ret_value, "Failed to add endpoint[%s]!", it->c_str());		
				}
			}
			break;
		}
	}

	return	ret_value;
}

RetValue	ObjectManager::SaveLoadedEndpoints()
{
	RetValue	ret_value = RET_VALUE_OBJECT_NOT_FOUND;

	for(auto it = load_endpoint_list_.begin(); it != load_endpoint_list_.end(); it++)
	{
		Endpoint *endpoint = GetEndpoint(*it);
		if (endpoint != NULL)
		{
			ret_value = data_manager_->AddEndpoint(endpoint->GetProperties());	
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to add endpoint[%s]!", it->c_str());
				break;
			}
		}
	}

	return	ret_value;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void	ObjectManager::OnQuit
(
	Message*	_message
)
{
	
	auto device_it = device_map_.begin();
	for(; device_it != device_map_.end() ; device_it++)
	{
		device_it->second->Stop();
	}

	MessageProcess::OnQuit(_message);
}

void	ObjectManager::ShowEndpointList()
{
	size_t	name_width 		= 16;
	size_t	id_width		= 16;
	size_t	type_width		= 8;
	size_t	index_width 	= 8;
	size_t	device_id_width = 16;
	size_t	enable_width 	= 8;
	size_t	activation_width= 10;
	size_t	value_count_width=32;

	for(size_t i = 0 ; i < GetEndpointCount() ; i++)
	{
		Endpoint* endpoint = GetEndpoint(i);

		if (name_width < endpoint->GetName().length())
		{
			name_width = endpoint->GetName().length();
		}

		if (id_width < endpoint->GetID().length())
		{
			id_width = endpoint->GetID().length();
		}

		const string	type = Endpoint::TypeToString(endpoint->GetType());
		if (type_width < type.length())
		{
			type_width = type.length();
		}
	}

	cout << setw(name_width) << "Name";
	cout << " " << setw(id_width) << "ID";
	cout << " " << setw(type_width) << "Type";
	cout << " " << setw(index_width) << "Index";
	cout << " " << setw(enable_width) << "Enable";
	cout << " " << setw(activation_width) << "Activation";
	cout << " " << setw(device_id_width) << "Device" ;
	cout << " " << setw(value_count_width) << "Value" << endl;

	for(size_t i = 0 ; i < GetEndpointCount() ; i++)
	{
		Endpoint* endpoint = GetEndpoint(i);
		ostringstream	buffer;

		cout << setw(name_width) << endpoint->GetName();
		cout << " " << setw(id_width) << endpoint->GetID();
		if (Endpoint::TypeToString(endpoint->GetType()).length() != 0)
		{
			cout << " " << setw(type_width) << Endpoint::TypeToString(endpoint->GetType());
		}
		else
		{
			cout << " " << setw(type_width) << endpoint->GetType();
		}

		cout << " " << setw(index_width) << endpoint->GetIndex();

		if (endpoint->GetEnabled())
		{
			cout << " " << setw(enable_width) << "on";
		}
		else
		{
			cout << " " << setw(enable_width) << "off";
		}

		if (endpoint->IsRun())
		{
			cout << " " << setw(activation_width) << "on";
		}
		else
		{
			cout << " " << setw(activation_width) << "off";
		}


		cout << " " << setw(device_id_width) << endpoint->GetDeviceID();

		Value value = endpoint->GetValue();
		buffer << value.ToString() << "[" << value.GetTime().ToString() << "]";
		cout << " " << setw(value_count_width) << buffer.str();
		
		cout << " " << endpoint->ValueCount();
		cout << endl;
		
	}

}

std::ostream& operator<< 
(
	std::ostream& _os,
	const ObjectManager& _om
)
{
	return	_os;
}
