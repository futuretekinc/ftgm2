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
#include "KompexSQLiteStatement.h"

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

	struct Properties
	{
		Type			type;
		std::string		id;
		std::string		name;
		bool			enable;

		std::list<std::string>	endpoint_list;

					//Properties();
					Properties(Type _type);
					//Properties(const Properties& _properties);
					//Properties(const JSONNode&	node);
		virtual		~Properties();

		static
		Properties*	Create(Type _type);
		static
		Properties*	Create(const JSONNode& _node);

		virtual
		RetValue	Set(const JSONNode&	node);
		RetValue	Set(Kompex::SQLiteStatement*	_statement);
		virtual
		RetValue	SetProperty(const std::string& _name, const std::string& _value);
		virtual
		RetValue	SetProperty(const std::string& _name, bool  _value);
		virtual
		RetValue	SetProperty(const std::string& _name, uint32_t	_value);

		virtual
		uint32		GetOptionsSize()	{	return	0;	};
		virtual
		uint32		GetOptions(uint8_t *options, uint32 options_len);
	};

	class	PropertiesList : public std::list<Properties *>
	{
	public:
		~PropertiesList();
	};

	// Constructor & Destructor
					Device(Type _type);
	virtual			~Device();
	
	// Attribute
	Type			GetType();
	const 
	std::string&	GetID();
	const 
	std::string&	GetName();
	RetValue		SetName(const std::string& _name);

	bool			GetEnable();
	RetValue		SetEnable(bool _enable);

	bool			GetActivation();
	RetValue		SetActivation(bool _activation);

	virtual
	RetValue		SetProperty(const std::string& _name, const std::string& _value);
	virtual
	RetValue		SetProperty(const std::string& _name, bool  _value);
	virtual
	RetValue		SetProperty(const std::string& _name, uint32_t	_value);

	virtual 
	Properties*		GetProperties();

	virtual
	RetValue		SetProperties(const Properties& _properties);
	virtual
	RetValue		SetProperties(const Properties* _properties);
	virtual			
	RetValue		SetProperties(Kompex::SQLiteStatement*	_statement);
	virtual	
	RetValue		SetProperties (const JSONNode&	node);


	// Operator
	RetValue		Activation();
	RetValue		Deactivation();


	RetValue		Connect(const std::string& _endpoint_id);
	RetValue		Disconnect(const std::string& _endpoint_id);

	virtual
	RetValue		GetEndpointValue(Endpoint* _endpoint);

	virtual
	RetValue		SetEndpointValue(Endpoint* _endpoint);

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
	// Internal functions
	void			PreProcess();
	void			Process();
	void			PostProcess();

	static 
	void			DeviceScheduleProcess(Device *_mp);

	Properties*		properties_;

	ObjectManager*	object_manager_;
	std::thread*	schedule_thread_;
	bool			schedule_stop_;
	bool			activation_;
	Scheduler<std::string>	endpoint_scheduler_;

};




#endif	// DEVICE_H_
