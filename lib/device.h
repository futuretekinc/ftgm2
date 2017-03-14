#ifndef	DEVICE_H_
#define	DEVICE_H_

#include "defined.h"
#include <string>
#include <list>
#include <thread>
#include <queue>
#include <libjson/libjson.h>
#include "object.h"
#include "message_queue.h"
#include "message_process.h"
#include "scheduler.h"
#include "endpoint.h"

class	Device : public MessageProcess
{
	friend	class	ObjectManager;
public:
	enum	Type
	{
		TYPE_UNKNOWN=0,
		TYPE_SNMP,
		TYPE_FTE
	};

	struct	TypeInfo
	{
		Type	type;
		const std::string name;
	};

	struct	Properties
	{
		Type			type;
		std::string		id;
		std::string		name;
		bool			enable;

		std::list<Endpoint::Properties*>	endpoint_list;

					Properties();
					Properties(Type _type);
					Properties(const Properties& _properties);
					Properties(const JSONNode&	node);
		virtual		~Properties();

		static
		Properties*	Create(Type _type);
		static
		Properties*	Create(const JSONNode& _node);

		virtual
		RetValue	Set(const JSONNode&	node);
		RetValue	AddEndpoint(const	JSONNode&	_node);
		RetValue	AddEndpoints(const	JSONNode&	_node);

		virtual
		void		Show();
	};

	class	PropertiesList : public std::list<Properties *>
	{
	public:
		~PropertiesList();
	};

	// Constructor & Destructor
					Device();
					Device(const Properties& _properties);
					Device(const Properties* _properties);
					Device(const std::string &_id);
	virtual			~Device();
	
	// Interface 
	Type			GetType()	{	return	properties_.type;	};
	const 
	std::string&	GetID()		{	return	properties_.id;	};
	const 
	std::string&	GetName()	{	return	properties_.name;	};
	void			SetName(const std::string& _name);

	bool			IsEnabled()	{	return	properties_.enable;};
	void			SetEnable(bool _enable);

	RetValue		Connect(Endpoint* _endpoint);
	RetValue		Disconnect(Endpoint* _endpoint);

	Endpoint*		CreateEndpoint(Endpoint::Type _type);
	Endpoint*		CreateEndpoint(const Endpoint::Properties& _properties);
	Endpoint*		CreateEndpoint(const Endpoint::Properties* _properties);

	RetValue		DeleteEndpoint(Endpoint* _endpoint);
	RetValue		DeleteEndpoint(const std::string& _id);

	uint32			GetEndpointCount();
	Endpoint*		GetEndpoint(int	_index);
	Endpoint*		GetEndpoint(const std::string _id);

	virtual
	RetValue		GetValue(Endpoint* _endpoint);
	virtual
	RetValue		SetValue(Endpoint* _endpoint);

	virtual	
	RetValue		SetProperties (const JSONNode&	node);

	// Operator
	RetValue		Activation();
	RetValue		Deactivation();
	bool			IsActivated()	{	return	activation_;	};

	RetValue		Activation(const std::string& _id);
	RetValue		Deactivation(const std::string& _id);
	RetValue		Activation(Endpoint* _endpoint);
	RetValue		Deactivation(Endpoint* _endpoint);

	// Message handler
	void			OnMessage(Message *_message);
	void			OnActivation(Message *_message);
	void			OnDeactivation(Message *_message);
	void			OnQuit(Message *_message);

	// External constructor
	static
	Device*			Create(Type _type);
	static
	Device*			Create(const Properties* _properties);

	// Utilities
	static
	Type			StringToType(const std::string& _name);
	static const 
	std::string&	TypeToString(Type _type);

	friend std::ostream& operator<<(std::ostream& _os, const Device& _device);

protected:
	RetValue		SetObjectManager(ObjectManager* _object_manager);
	ObjectManager*	GetObjectManager()	{	return	object_manager_;	}
	RetValue		ReleaseObjectManager();


	// Internal functions
	void			PreProcess();
	void			Process();
	void			PostProcess();

	static 
	void		DeviceScheduleProcess(Device *_mp);

	Properties		properties_;
	
	ObjectManager*	object_manager_;
	std::thread*	schedule_thread_;
	bool			schedule_stop_;
	bool			activation_;

	std::map<const std::string, class Endpoint *>	endpoint_list_;
	Scheduler<std::string>	endpoint_scheduler_;

};




#endif	// DEVICE_H_
