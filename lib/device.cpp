#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <sys/time.h>
#include <ostream>
#include <iomanip>
#include "timer.h"
#include "trace.h"
#include "object_manager.h"
#include "device.h"
#include "device_snmp.h"
#include "device_fte.h"
#include "endpoint.h"

using namespace std;

struct	Device::TypeInfo kDeviceTypeInfoList[] = 
{
	{ Device::TYPE_FTE, 		"fte"	},
	{ Device::TYPE_UNKNOWN, 	""		}
};

Device::Properties::Properties()
{
	ostringstream	buffer;
	Time			time = Time::GetCurrentTime();

	buffer << time.Milliseconds();

	type	=	TYPE_UNKNOWN;
	id		=	"dev-" + buffer.str();
	name	=	id;
	enable	=	false;
}
Device::Properties::Properties(Type _type)
{
	ostringstream	buffer;
	Time			time = Time::GetCurrentTime();

	buffer << time.Milliseconds();

	type	=	_type;
	id		=	"dev-" + buffer.str();
	name	=	id;
	enable	=	false;
}

Device::Properties::Properties
(
	const Properties& _properties
)
{
	type	=	_properties.type;
	id		=	_properties.id;
	name	=	_properties.name;
	enable	=	_properties.enable;

	auto it = _properties.endpoint_list.cbegin();
	while(it != _properties.endpoint_list.end())
	{
		Endpoint::Properties *endpoint_properties = (*it)->Duplicate();
		if (endpoint_properties != NULL)
		{
			endpoint_list.push_back(endpoint_properties);
		}

		it++;
	}
}

Device::Properties::Properties
(
	const JSONNode&	_node
)
{
	ostringstream	buffer;
	Time			time = Time::GetCurrentTime();

	buffer << time.Milliseconds();

	TRACE(NULL, "The Device properties created.");

	type	= TYPE_UNKNOWN;
	id		= "dev-" + buffer.str();
	name	= id;
	enable	= false;

	Set(_node);
}

Device::Properties::~Properties()
{
	list<Endpoint::Properties*>::iterator	it = endpoint_list.begin();
	while(it != endpoint_list.end())
	{
		delete (*it);	

		it++;
	}
}

Device::Properties*	Device::Properties::Create
(
	Type	_type
)
{
	Properties*	properties = NULL;

	switch(_type)
	{
	case	Device::TYPE_FTE:	
		{
			TRACE(NULL, "Device FTE properties created.");
			properties = new DeviceFTE::Properties;
		}
		break;

	default:
		break;
	}

	return	properties;
}

Device::Properties*	Device::Properties::Create
(
	const	JSONNode&	_node
)
{
	Properties*	properties = NULL;
	Type	type = TYPE_UNKNOWN;

	if (_node.type() != JSON_NODE)
	{
		cout << "Json node is not node"<< endl;
		return	NULL;
	}

	for(size_t i = 0 ; i < _node.size() ; i++)
	{
		if (strcasecmp(_node[i].name().c_str(), "type") == 0)
		{
			type = StringToType(_node[i].as_string());
			break;
		}
	}

	switch(type)
	{
	case	Device::TYPE_FTE:	
		{
			TRACE(NULL, "Device FTE properties created.");
			properties = new DeviceFTE::Properties;
		}
		break;

	default:
		break;
	}

	if (properties != NULL)
	{
		properties->Set(_node);
	}
	else
	{
		cout << "Unknown device type!" << endl;
	}

	return	properties;
}

RetValue Device::Properties::Set
(
	const JSONNode&	_node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (_node.type() == JSON_NODE)
	{
		for(size_t i = 0 ; i < _node.size() ; i++)
		{
			ret_value = Set(_node[i]);
			if (ret_value != RET_VALUE_OK)
			{
				break;	
			}
		}
	}
	else if (_node.type() != JSON_NULL)
	{
		if (strcasecmp(_node.name().c_str(), "name") == 0)
		{
			if (_node.type() == JSON_STRING)
			{
				name = _node.as_string();
			}
			else
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(NULL, ret_value, "Failed to set enable!");
			}
		}
		else if (strcasecmp(_node.name().c_str(), "id") == 0)
		{
			if (_node.type() == JSON_STRING)
			{
				id = _node.as_string();
			}
			else
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(NULL, ret_value, "Failed to set enable!");
			}
		}
		else if (strcasecmp(_node.name().c_str(), "enable") == 0)
		{
			if (_node.type() == JSON_NUMBER)
			{
				enable = (_node.as_int() != 0);
			}
			else if (_node.type() == JSON_STRING)
			{
				if ((strcasecmp(_node.as_string().c_str(), "true") == 0) || 
						(strcasecmp(_node.as_string().c_str(), "on") == 0) || 
						(strcasecmp(_node.as_string().c_str(), "1") == 0))
				{
					enable = true;	
				}
				else if ((strcasecmp(_node.as_string().c_str(), "false") == 0) || 
						(strcasecmp(_node.as_string().c_str(), "off") == 0) || 
						(strcasecmp(_node.as_string().c_str(), "0") == 0))
				{
					enable = false;
				}
				else
				{
					ret_value = RET_VALUE_INVALID_FIELD;
					ERROR(NULL, ret_value, "Failed to set enable!");
				}
			}
			else
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(NULL, ret_value, "Failed to set enable!");
			}

		}
		else if (strcasecmp(_node.name().c_str(), "endpoints") == 0)
		{
			AddEndpoints(_node);	
		}
		else if (strcasecmp(_node.name().c_str(), "endpoint") == 0)
		{
			AddEndpoint(_node);	
		}
	}
	else
	{
		ret_value = RET_VALUE_INVALID_FIELD;
	}

	return	ret_value;
}

RetValue	Device::Properties::AddEndpoints
(
	const JSONNode& _node
)
{
	RetValue ret_value = RET_VALUE_OK;

	if(_node.type() == JSON_ARRAY)
	{
		for(int i = 0 ; i < (int)_node.size() ; i++)
		{
			AddEndpoint(_node[i]);	
		}
	}
	else
	{
		ret_value = RET_VALUE_INVALID_TYPE;
	}

	return	ret_value;
}

RetValue	Device::Properties::AddEndpoint
(
	const	JSONNode&	_node
)
{
	Endpoint::Properties*	endpoint_properties = Endpoint::Properties::Create(_node);

	endpoint_list.push_back(endpoint_properties);

	return	RET_VALUE_OK;
}

void	Device::Properties::Show()
{

	cout << setw(16) << "Type : " << Device::TypeToString(type) << endl;
	cout << setw(16) << "ID : "   << id << endl;
	cout << setw(16) << "Name : " << name << endl;
	cout << setw(16) << "Enable : " << (enable?"true":"false") << endl;
	cout << setw(16) << "Endpoint Count : " << endpoint_list.size() << endl;
	std::list<Endpoint::Properties*>::iterator it = endpoint_list.begin();
	while(it != endpoint_list.end())
	{
		cout << setw(16) << "" << (*it)->id << endl;
		it++;	
	}
}

Device::PropertiesList::~PropertiesList()
{
	list<Properties*>::iterator it = this->begin();
	while(it != this->end())
	{
		delete (*it);
		it++;	
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Device Class
///////////////////////////////////////////////////////////////////////////////////
Device::Device()
: MessageProcess()
{
	struct timeval	time;
	ostringstream	out;

	gettimeofday(&time, NULL);

	out << "device" << time.tv_sec << time.tv_usec;

	properties_.type	= TYPE_UNKNOWN;
	properties_.name 	= out.str();
	properties_.id 		= out.str();
	properties_.enable	= false;
	activation_			= false;
	schedule_thread_ 	= NULL;

	TRACE(this, "Device[%s] created.", properties_.id.c_str());
}

Device::Device
(
	const Properties& _properties
)
: MessageProcess(), properties_(_properties)
{
	activation_ 		= false;
	schedule_thread_ 	= NULL;

	auto it = _properties.endpoint_list.cbegin();
	while(it != _properties.endpoint_list.end())
	{
		Endpoint	*endpoint = Endpoint::Create(*it);
		if (endpoint != NULL)
		{
			RetValue	ret_value;

			ret_value = Connect(endpoint);	
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to attach to endpoint");
				delete endpoint;	
			}
		}

		it++;
	}

	TRACE(this, "Device[%s] created.", properties_.id.c_str());
}

Device::Device
(
	const Properties* _properties
)
: MessageProcess(), properties_(*_properties)
{
	activation_ 		= false;
	schedule_thread_ 	= NULL;

	auto it = properties_.endpoint_list.cbegin();
	while(it != properties_.endpoint_list.end())
	{
		Endpoint	*endpoint = Endpoint::Create(*it);
		if (endpoint != NULL)
		{
			RetValue	ret_value;

			ret_value = Connect(endpoint);	
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to attach to endpoint");
				delete endpoint;	
			}
		}

		it++;
	}

	TRACE(this, "Device[%s] created.", properties_.id.c_str());
}

Device::Device
(
	const std::string &_id
)
: MessageProcess()
{
	properties_.type	= TYPE_UNKNOWN;
	properties_.name 	= _id;
	properties_.id 		= _id;	
	properties_.enable	= false;
	activation_			= false;
	schedule_thread_ 	= NULL;

	TRACE(this, "Device[%s] created.", properties_.id.c_str());
}

Device::~Device()
{
	map<const string, Endpoint *>::iterator it = endpoint_list_.begin();
	while(it != endpoint_list_.end())
	{
		Endpoint*	endpoint = it->second;
	
		endpoint->ReleaseDevice();
		delete endpoint;

		it++;
	}

	if (object_manager_ != NULL)
	{
		object_manager_->Disconnect(this);	
	}
	TRACE(this, "Device[%s] destroy.", properties_.id.c_str());
}

void	Device::SetEnable
(
	bool _enable
)
{
	if (properties_.enable != _enable)
	{
		properties_.enable = _enable;

		if (IsRun())
		{
			if (properties_.enable == true)
			{
				Activation();
			}
			else if (properties_.enable == false)
			{
				Deactivation();
			}
		}
	}	
}

void	Device::SetName
(
	const string& _name
)	
{	
	properties_.name = _name;	
}

RetValue	Device::SetObjectManager
(
	ObjectManager* _object_manager
)
{
	object_manager_ = _object_manager;

	return	RET_VALUE_OK;
}

RetValue	Device::ReleaseObjectManager()
{
	object_manager_ = NULL;

	return	RET_VALUE_OK;
}

Endpoint*	Device::CreateEndpoint
(
	Endpoint::Type _type
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Endpoint*	endpoint = Endpoint::Create(_type);

	if (endpoint != NULL)
	{
		ret_value = Connect(endpoint);
		if (ret_value != RET_VALUE_OK)
		{
			delete endpoint;	
			endpoint = NULL;
		}
	}

	return	endpoint;
}

Endpoint*	Device::CreateEndpoint
(
	const Endpoint::Properties& _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Endpoint*	endpoint = Endpoint::Create(&_properties);

	if (endpoint != NULL)
	{
		ret_value = Connect(endpoint);
		if (ret_value != RET_VALUE_OK)
		{
			delete endpoint;	
			endpoint = NULL;
		}
	}

	return	endpoint;
}

Endpoint*	Device::CreateEndpoint
(
	const Endpoint::Properties* _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Endpoint*	endpoint = Endpoint::Create(_properties);

	if (endpoint != NULL)
	{
		ret_value = Connect(endpoint);
		if (ret_value != RET_VALUE_OK)
		{
			delete endpoint;	
			endpoint = NULL;
		}
	}

	return	endpoint;
}

RetValue	Device::DeleteEndpoint
(
	Endpoint* _endpoint
)
{
	RetValue	ret_value;
	
	ret_value = Disconnect(_endpoint);
	if (ret_value == RET_VALUE_OK)
	{
		delete _endpoint;	
	}

	return	ret_value;
}

RetValue	Device::DeleteEndpoint
(
	const std::string& _id
)
{	
	RetValue	ret_value;

	Endpoint*	endpoint = GetEndpoint(_id);
	if (endpoint == NULL)
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;	
	}
	else
	{
		ret_value = Disconnect(endpoint);
		if (ret_value == RET_VALUE_OK)
		{
			delete endpoint;	
		}
	}

	return	ret_value;

}

RetValue	Device::Connect
(
	Endpoint *_ep
)
{
	RetValue	ret_value;

	map<const string, Endpoint *>::iterator it = endpoint_list_.find(_ep->GetID());

	if (it != endpoint_list_.end())
	{
		ret_value = RET_VALUE_OK;
	}
	else
	{
		endpoint_list_[_ep->GetID()] = _ep;

		ret_value = _ep->SetDevice(this);
		if (ret_value != RET_VALUE_OK)
		{
			ERROR(this, ret_value, "The endpoint[%s] is failed to attach at device[%s]", _ep->GetID().c_str(), properties_.id.c_str());	
		}
	}

	return	ret_value;
}

RetValue	Device::Disconnect
(
	Endpoint *_ep
)
{
	RetValue	ret_value;

	if (_ep == NULL)
	{
		for(map<const string, Endpoint *>::iterator it = endpoint_list_.begin(); it != endpoint_list_.end() ; it++)
		{
			it->second->ReleaseDevice();
			endpoint_list_.erase(it);
		}
	}
	else
	{
		map<const string, Endpoint *>::iterator it = endpoint_list_.find(_ep->GetID());
		if (it != endpoint_list_.end())
		{
			endpoint_list_.erase(it);

			ret_value = _ep->ReleaseDevice();
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "The endpoint[%s] can not be detached.", _ep->GetID().c_str());
			}
		}
	}

	return	ret_value;
}

RetValue	Device::SetProperties
(
	const	JSONNode&	node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (node.type() == JSON_NODE)
	{
		for(size_t i = 0 ; i < node.size() ; i++)
		{
			SetProperties(node[i]);
		}
	}
	else 
	{
		if (node.name() == "name")
		{
			if ((node.type() != JSON_STRING) && (node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "");	
			}
			else
			{
				properties_.name = node.as_string();
			}
		}
		else if (node.name() == "id")
		{
			if ((node.type() != JSON_STRING) && (node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this,RET_VALUE_INVALID_FIELD, "");	
			}
			else
			{
				properties_.id = node.as_string();
			}
		}
	}

	return	ret_value;
}

void	Device::PreProcess()
{
	Activation();
}

void	Device::Process()
{
}

void	Device::PostProcess()
{
	if (schedule_thread_ != NULL)
	{
		delete schedule_thread_;
		schedule_thread_ = NULL;
	}

}

uint32		Device::GetEndpointCount()
{
	return	endpoint_list_.size();
}

Endpoint*	Device::GetEndpoint
(
	int	_index
)
{
	map<const string, Endpoint *>::iterator it = endpoint_list_.begin();
	for(;_index > 0 && it != endpoint_list_.end() ; it++, _index--)
	{
	}

	if (it != endpoint_list_.end())
	{
		return	it->second;	
	}
	
	return	NULL;
}

Endpoint*	Device::GetEndpoint
(
	const std::string _id
)
{
	map<const string, Endpoint *>::iterator it = endpoint_list_.find(_id);
	if (it != endpoint_list_.end())
	{
		return	it->second;	
	}

	return	NULL;
}

RetValue	Device::GetValue
(
	Endpoint* _endpoint
)
{
	return	RET_VALUE_OK;
}

RetValue	Device::SetValue
(
	Endpoint* _endpoint
)
{
	return	RET_VALUE_OK;
}

RetValue	Device::Activation()
{
	RetValue	ret_value = RET_VALUE_OK;

	if (properties_.enable)
	{
		if (!activation_)
		{
			schedule_thread_ = new thread(DeviceScheduleProcess, this);
			if (schedule_thread_ == NULL)
			{
				ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;
				ERROR(this, ret_value, "The device[%s] can not be activated!", properties_.id.c_str());
			}
			else
			{
				while(!schedule_thread_->joinable())
				{
					usleep(1000);
				}

				TRACE(this, "New thread[%08x] created", schedule_thread_->get_id());
				activation_ = true;
			}
		}
	}
	return	ret_value;
}

RetValue	Device::Deactivation()
{
	RetValue	ret_value = RET_VALUE_OK;

	if (activation_)
	{
		schedule_stop_ = true;

		schedule_thread_->join();

		delete schedule_thread_;
		schedule_thread_ = NULL;

		activation_ = false;
	}

	return	ret_value;
}

RetValue	Device::Activation
(
	const std::string& _id
)
{
	Endpoint *endpoint = GetEndpoint(_id);

	if (endpoint == NULL)
	{
		return	RET_VALUE_OBJECT_NOT_FOUND;
	}

	return	Activation(endpoint);
}

RetValue	Device::Deactivation
(
	const std::string& _id
)
{
	Endpoint *endpoint = GetEndpoint(_id);

	if (endpoint == NULL)
	{
		return	RET_VALUE_OBJECT_NOT_FOUND;
	}

	return	Deactivation(endpoint);
}

RetValue	Device::Activation
(
	Endpoint* _endpoint
)
{
	TRACE_ENTRY(this);
	TimeoutTimer	timer(Time::GetCurrentTime());

	timer.Add(_endpoint->UpdateInterval());

	TRACE(this, "The endpoint[%s] is activated!", _endpoint->GetID().c_str());
	return	endpoint_scheduler_.Push(timer, _endpoint->GetID());
}

RetValue		Device::Deactivation
(
	Endpoint* _endpoint
)
{
	endpoint_scheduler_.Pop(_endpoint->GetID(), true);

	TRACE(this, "The endpoint[%s] is deactivated!", _endpoint->GetID().c_str());
	return	RET_VALUE_OK;
}


void	Device::OnMessage
(
	Message *_message
)
{
	TRACE(this,"Message Received[%s]", ToString(_message).c_str());
	switch(_message->type)
	{
	case	Message::TYPE_STARTED:
		{
			if (activation_)
			{
				Activation();	
			}
		}
		break;

	case	Message::TYPE_STOP:
		{
			if (activation_)
			{
				Deactivation();	
			}
		}
		break;

	default:
		MessageProcess::OnMessage(_message);
	}
}

void	Device::OnActivation
(
	Message *_message
)
{
	Activation();
}

void	Device::OnDeactivation
(
	Message *_message
)
{
	Deactivation();
}

void	Device::OnQuit
(
	Message *_message
)
{
	MessageProcess::OnQuit(_message);
}

Device::Type	Device::StringToType
(
	const std::string& _name
)
{
	struct	Device::TypeInfo *type_info = kDeviceTypeInfoList;

	while(type_info->type != Device::TYPE_UNKNOWN)
	{
		if (type_info->name.compare(_name) == 0)
		{
			return	type_info->type;
		}

		type_info++;
	}

	return	Device::TYPE_UNKNOWN;	
}

const 
std::string& Device::TypeToString
(
	Device::Type	_type
)
{
	struct	Device::TypeInfo *type_info = kDeviceTypeInfoList;

	while(type_info->type != Device::TYPE_UNKNOWN)
	{
		if (type_info->type == _type)
		{
			break;
		}

		type_info++;
	}

	return	type_info->name;
}


void Device::DeviceScheduleProcess
(
	Device *_device
)
{

	TRACE(_device, "The device[%s] Scheduler started!", _device->GetID().c_str());
	_device->schedule_stop_ = false;

	auto it = _device->endpoint_list_.begin();
	for(; it != _device->endpoint_list_.end(); it++)
	{
		it->second->Activation();
	}

	while(!_device->schedule_stop_)
	{
		ScheduleItem<std::string>* item = _device->endpoint_scheduler_.Top();
		if (item != NULL)
		{
			if (item->timer.IsExpired())
			{
				_device->endpoint_scheduler_.Pop();

				Endpoint*	endpoint = _device->GetEndpoint(item->data);
				if (endpoint == NULL)
				{
					TRACE(_device,"Endpoint not found!");
					delete item;
				}
				else
				{
					if (endpoint->IsActivated())
					{
						RetValue	ret_value;

						ret_value = endpoint->Synchronize();
						if (ret_value != RET_VALUE_OK)		
						{
							ERROR(_device, ret_value, "Failed to synchronize endpoint.");
						}

						item->timer.Add(endpoint->UpdateInterval());
						_device->endpoint_scheduler_.Push(item);
					}
					else
					{
						TRACE(_device, "The endpoint[%s] is deactivated.", endpoint->GetID().c_str());
					}
				}
			}
		}

		usleep(100000);	
	}

	TRACE(_device, "The device[%s] Scheduler stopped!", _device->GetID().c_str());
}
///////////////////////////////////////////////////////
// Global functions
///////////////////////////////////////////////////////
Device*		Device::Create
(
	Device::Type _type
)
{
	Device* device ;

	switch(_type)
	{
	case	Device::TYPE_FTE:
		device = new DeviceFTE();
		break;

	default:
		device = NULL;
	}

	return	device;
}

Device*		Device::Create
(
	const Device::Properties* _properties
)
{
	Device* device ;

	switch(_properties->type)
	{
	case	Device::TYPE_FTE:
		device = new DeviceFTE(*(DeviceFTE::Properties *)_properties);
		break;

	default:
		device = NULL;
	}

	return	device;
}

ostream& operator<<
(
	ostream& _os, 
	const Device& _device
)
{
	_os << "Name : " << _device.properties_.name << endl;
	_os << "  ID : " << _device.properties_.id << endl;

	return	_os;
}

