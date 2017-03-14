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
{
}

ObjectManager::~ObjectManager()
{
	map<const string, Device *>::iterator device_it = device_map_.begin();
	for(; device_it != device_map_.end() ; device_it++)
	{
		delete device_it->second;
	}

	map<const string, Endpoint *>::iterator endpoint_it = endpoint_map_.begin();
	for(; endpoint_it != endpoint_map_.end() ; endpoint_it++)
	{
		delete endpoint_it->second;
	}
}

const std::string	ObjectManager::ClassName()
{
	return	typeid(*this).name();
}

void	ObjectManager::PreProcess()
{
	if (data_manager_ != NULL)
	{
		data_manager_->Start(1000);	
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

		pbuf->sgetn (buffer, size);
		ifs.close();

		if (libjson::is_valid(buffer) == true)
		{
			JSONNode	root = libjson::parse(buffer);

			for(size_t i = 0 ; i < root.size() ; i++)
			{
				if (root[i].name() == "device")
				{
					LoadDevice(root[i]);	
				}
			}
		}

		delete	[] buffer;

		return	RET_VALUE_OK;
	}
	catch(exception& e)
	{
		ret_value = RET_VALUE_EXCEPTION;
		ERROR(this, ret_value, e.what());

		return	ret_value;
	}
}

////////////////////////////////////////////////////////////////
//	for Device
////////////////////////////////////////////////////////////////
RetValue	ObjectManager::Connect
(
	Device *_device
)
{
	RetValue	ret_value = RET_VALUE_OK;

	map<const string, Device *>::iterator it = device_map_.find(_device->GetID());
	if (it == device_map_.end())
	{
		TRACE(this, "The device[%s] was connected", _device->GetID().c_str());
		device_map_[_device->GetID()] = _device;

		_device->SetObjectManager(this);

		for(uint32 i = 0 ; i < _device->GetEndpointCount() ; i++)
		{
			Endpoint*	endpoint = _device->GetEndpoint(i);
			if (endpoint != NULL)
			{
				ret_value = Connect(endpoint);
				if (ret_value != RET_VALUE_OK)
				{
					ERROR(this, ret_value, "Failed to attach endpoint to manager!");
				}
			}
		}
	}

	return	ret_value;
}

RetValue	ObjectManager::Disconnect
(
	Device*	_device
)
{
	RetValue	ret_value = RET_VALUE_OK;

	map<const string, Device *>::iterator it = device_map_.find(_device->GetID());
	if (it != device_map_.end())
	{
		device_map_.erase(_device->GetID());

		_device->ReleaseObjectManager();

		for(uint32 i = 0 ; i < _device->GetEndpointCount() ; i++)
		{
			Endpoint*	endpoint = _device->GetEndpoint(i);
			if (endpoint != NULL)
			{
				ret_value = Disconnect(endpoint);
				if (ret_value != RET_VALUE_OK)
				{
					ERROR(this, ret_value, "Failed to detach endpoint to manager!");
				}
			}
		}
	}

	return	ret_value;
}

uint32		ObjectManager::DeviceCount()
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
		map<const string, Device *>::iterator it = device_map_.begin();
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
	map<const string, Device *>::iterator it = device_map_.begin();
	for(; it != device_map_.end() ; it++)
	{
		if (it->first == _id)
		{
			return	it->second;	
		}
	}		

	return	NULL;
}

RetValue	ObjectManager::LoadDevice
(
	const	JSONNode& _json_node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	TRACE_ENTRY(this);
	if(_json_node.type() == JSON_ARRAY)
	{
		for(int i = 0 ; i < (int)_json_node.size() ; i++)
		{
			LoadDevice(_json_node[i]);	
		}
	}
	else if (_json_node.type() == JSON_NODE)
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
			Device::Properties*	properties = Device::Properties::Create(_json_node);
			if (properties != NULL)
			{
				properties->Show();

				device = Device::Create(properties);
				if (device != NULL)
				{
					ret_value = Connect(device);
					if (ret_value != RET_VALUE_OK)
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
			}

		}
	}
	else
	{
		ret_value = RET_VALUE_INVALID_TYPE;
		ERROR(this, ret_value, "Object is not node!");
	}

	TRACE_EXIT(this);

	return	ret_value;
}

void	ObjectManager::ShowDeviceList()
{
	size_t	name_width 	= 16;
	size_t	id_width	= 16;
	size_t	type_width	= 8;
	size_t	enable_width	= 8;
	size_t	activation_width	= 11;

	map<const string, Device *>::iterator it = device_map_.begin();
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

	cout << setw(name_width) << "Name" << " " << setw(id_width) << "ID" << " " << setw(type_width) << "Type" << " " << setw(enable_width) << "Enable" << " " << setw(activation_width) << "Activation" << endl;
	it = device_map_.begin();
	for(; it != device_map_.end() ; it++)
	{
		cout << setw(name_width) << it->second->GetName();
		cout << " " << setw(id_width) << it->second->GetID();
		cout << " " << setw(type_width) << Device::TypeToString(it->second->GetType());
		if (it->second->IsEnabled())
		{
			cout << " " << setw(enable_width) << "on";
		}
		else
		{
			cout << " " << setw(enable_width) << "off";
		}

		if (it->second->IsActivated())
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

////////////////////////////////////////////////////////////////
//	for Endpoint
////////////////////////////////////////////////////////////////
RetValue	ObjectManager::Connect
(
	Endpoint*	_endpoint
)
{
	RetValue	ret_value = RET_VALUE_OK;

	map<const string, Endpoint *>::iterator it = endpoint_map_.find(_endpoint->GetID());
	if (it == endpoint_map_.end())
	{
		TRACE(this, "The endpoint[%s] was connected", _endpoint->GetID().c_str());
		endpoint_map_[_endpoint->GetID()] = _endpoint;
		_endpoint->SetObjectManager(this);
	}

	return	ret_value;
}

RetValue	ObjectManager::Disconnect
(
	Endpoint*	_endpoint
)
{
	RetValue	ret_value = RET_VALUE_OK;

	map<const string, Endpoint *>::iterator it = endpoint_map_.find(_endpoint->GetID());
	if (it != endpoint_map_.end())
	{
		endpoint_map_.erase(_endpoint->GetID());
		_endpoint->ReleaseObjectManager();
	}

	return	ret_value;
}

uint32		ObjectManager::EndpointCount()
{
	return	endpoint_map_.size();
}

Endpoint*		ObjectManager::GetEndpoint
(
	uint32 _index
)
{
	if (_index < endpoint_map_.size())
	{
		map<const string, Endpoint *>::iterator it = endpoint_map_.begin();
		for(; it != endpoint_map_.end() ; it++, _index--)
		{
			if (_index == 0)
			{
				return	it->second;	
			}
		}		
	}

	return	NULL;
}

Endpoint*		ObjectManager::GetEndpoint
(
	const string& _id
)
{
	map<const string, Endpoint *>::iterator it = endpoint_map_.begin();
	for(; it != endpoint_map_.end() ; it++)
	{
		if (it->first == _id)
		{
			return	it->second;	
		}
	}		

	return	NULL;
}

RetValue	ObjectManager::LoadEndpoint
(
	const	JSONNode& _json_node
)
{
	RetValue ret_value = RET_VALUE_OK;

	if(_json_node.type() == JSON_ARRAY)
	{
		for(int i = 0 ; i < (int)_json_node.size() ; i++)
		{
			LoadEndpoint(_json_node[i]);	
		}
	}
	else if (_json_node.type() == JSON_NODE)
	{
		Endpoint* endpoint = NULL;

		JSONNode::const_iterator it=_json_node.find("type");
		if (it == _json_node.end())
		{
			ret_value = RET_VALUE_INVALID_TYPE;
			ERROR(this, ret_value, "type not found!");
		}
		else
		{
			for(size_t i = 0 ; i < _json_node.size() ; i++)
			{
				const JSONNode& child_node = _json_node[i];

				if (child_node.name() == "type")
				{
					Endpoint::Type	type = Endpoint::StringToType(child_node.as_string());
					if (type != Endpoint::TYPE_UNKNOWN)
					{
						endpoint = Endpoint::Create(type);
						break;
					}
				}

			}

			if (endpoint == NULL)
			{
				ret_value = RET_VALUE_INVALID_TYPE;	
				ERROR(this, ret_value, "");
			}
			else
			{
				endpoint->SetProperties(_json_node);
				endpoint_map_[endpoint->GetID()] = endpoint;
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_INVALID_TYPE;
		ERROR(this, ret_value, "Object is not node!");
	}

	return	ret_value;
}

///////////////////////////////////////////////////////
// for DataManager
///////////////////////////////////////////////////////
RetValue	ObjectManager::Connect
(
	DataManager*	_data_manager
)
{
	if (data_manager_ != _data_manager)
	{
		data_manager_ = _data_manager;
	}

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

void	ObjectManager::OnMessage
(
	Message* _message
)
{
	TRACE(this, "Message[%s] Received", ToString(_message).c_str());
	switch(_message->type)
	{
	case	Message::TYPE_STARTED:
		{
			if (_message->id == string(""))
			{
				map<const string, Device *>::iterator	it = device_map_.begin();
				for(; it != device_map_.end(); it++)
				{
					it->second->Start();
					TRACE(this, "Device[%s] started", it->second->GetID().c_str());
				}
			}
			else
			{
				map<const string, Device *>::iterator	it = device_map_.find(((MessageStart *)_message)->id);
				if (it != device_map_.end())
				{
					it->second->Start();
					TRACE(this, "Device[%s] started", it->second->GetID().c_str());
				}
			}
		}
		break;

	case	Message::TYPE_STOP:
		{
			if (_message->id == string(""))
			{
				map<const string, Device *>::iterator	it = device_map_.begin();
				for(; it != device_map_.end(); it++)
				{
					TRACE(this, "Device[%s] stop", it->second->GetID().c_str());
					it->second->Stop();
				}
			}
			else
			{
				map<const string, Device *>::iterator	it = device_map_.find(((MessageStart *)_message)->id);
				if (it != device_map_.end())
				{
					TRACE(this, "Device[%s] stop", it->second->GetID().c_str());
					it->second->Stop();
				}
			}
		}
		break;

	default:
		{
			MessageProcess::OnMessage(_message);
		}
	}
}

void	ObjectManager::OnQuit
(
	Message*	_message
)
{
	
	map<const string, Device *>::iterator device_it = device_map_.begin();
	for(; device_it != device_map_.end() ; device_it++)
	{
		device_it->second->Stop();
	}

	MessageProcess::OnQuit(_message);
}

void	ObjectManager::ShowEndpointList()
{
	size_t	name_width 	= 16;
	size_t	id_width	= 16;
	size_t	type_width	= 8;
	size_t	index_width = 8;
	size_t	device_id_width = 16;
	size_t	enable_width = 8;
	size_t	activation_width= 10;
	size_t	value_count_width=32;

	map<const string, Endpoint *>::iterator it = endpoint_map_.begin();
	for(; it != endpoint_map_.end() ; it++)
	{
		if (name_width < it->second->GetName().length())
		{
			name_width = it->second->GetName().length();
		}

		if (id_width < it->second->GetID().length())
		{
			id_width = it->second->GetID().length();
		}

		const string	type = Endpoint::TypeToString(it->second->GetType());
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

	it = endpoint_map_.begin();
	for(; it != endpoint_map_.end() ; it++)
	{
		ostringstream	buffer;

		cout << setw(name_width) << it->second->GetName();
		cout << " " << setw(id_width) << it->second->GetID();
		if (Endpoint::TypeToString(it->second->GetType()).length() != 0)
		{
			cout << " " << setw(type_width) << Endpoint::TypeToString(it->second->GetType());
		}
		else
		{
			cout << " " << setw(type_width) << it->second->GetType();
		}

		cout << " " << setw(index_width) << it->second->GetIndex();

		if (it->second->IsEnabled())
		{
			cout << " " << setw(enable_width) << "on";
		}
		else
		{
			cout << " " << setw(enable_width) << "off";
		}

		if (it->second->IsActivated())
		{
			cout << " " << setw(activation_width) << "on";
		}
		else
		{
			cout << " " << setw(activation_width) << "off";
		}


		if (it->second->GetDevice() != NULL)
		{
			cout << " " << setw(device_id_width) << it->second->GetDevice()->GetID();
		}
		else
		{
			cout << " " << setw(device_id_width) << setfill(' ') << "";
		}

		TimedValue value = it->second->GetValue();
		buffer << value.ToString() << "[" << value.GetTime().ToString() << "]";
		cout << " " << setw(value_count_width) << buffer.str();
		
		cout << " " << it->second->ValueCount();
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
