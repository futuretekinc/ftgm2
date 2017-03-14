#ifndef	ENDPOINT_H_
#define	ENDPOINT_H_

#include "defined.h"
#include <string>
#include <list>
#include "object.h"
#include "libjson/libjson.h"
#include "value.h"

class	ObjectManager;
class	Device;

class	Endpoint : public Object
{
	friend class	ObjectManager;
	friend class	Device;
public:
	enum	Type
	{
		TYPE_UNKNOWN = 0,
		TYPE_SENSOR,
		TYPE_SENSOR_TEMPERATURE,
		TYPE_SENSOR_HUMIDITY,
		TYPE_SENSOR_VOLTAGE,
		TYPE_SENSOR_CURRENT,
		TYPE_SENSOR_DIGITAL_INPUT,

		TYPE_CONTROL = 1000,
		TYPE_CONTROL_DIGITAL_OUTPUT
	};

	struct	Properties
	{
					Properties(const Properties& _properties);
					Properties(const Properties* _properties);
					Properties(Type _type = TYPE_UNKNOWN, uint32_t _index = 0);
					Properties(const JSONNode& _node);
		virtual		~Properties();

		static
		Properties*	Create(const JSONNode& _node);
		static
		Properties*	Create(Type _type);
		virtual
		Properties*	Duplicate();
		RetValue	Set(const JSONNode& _node);

		Type		type;
		uint32_t	index;
		std::string	id;
		std::string	name;
		bool		enable;
		uint32_t	update_interval;
		uint32_t	value_count;	
	};

	class	PropertiesList : public std::list<Properties*>
	{
	public:
		~PropertiesList();
	};

					Endpoint(Type _type);
					Endpoint(const Properties& _properties);
					Endpoint(const Properties* _properties);
					Endpoint(Type _type, std::string _id);
	virtual 		~Endpoint();

	static
	Endpoint* 		Create(Type _type);
	static
	Endpoint*		Create(const Properties* properties);

	Type			GetType()	{	return	properties_.type;	};
	uint32_t		GetIndex()	{	return	properties_.index;	};
	const
	std::string		GetID()		{	return	properties_.id;	};
	const 
	std::string		GetName()	{	return	properties_.name;	};
	const
	std::string		SetName(const std::string& _name)	{	return	properties_.name = _name; }

	bool			IsEnabled()	{	return	properties_.enable;	}
	RetValue		SetEnable(bool _enable);

	uint32			UpdateInterval()	{	return	properties_.update_interval;	}
	void			UpdateInterval(uint32_t	_interval);
	
	RetValue		Activation();
	RetValue		Deactivation();
	bool			IsActivated()	{	return	activation_;	};

	virtual
	RetValue		Synchronize();

	uint32_t		MaxValueCount()	{	return	properties_.value_count;	}
	void			MaxValueCount(uint32_t _count);

	uint32_t		ValueCount();

	RetValue		AddValue(const TimedValue& _value);
	RetValue		DeleteValue(uint32_t _count);

	TimedValue		GetValue();
	TimedValue		GetValue(uint32_t _index);
	uint32_t		GetValueList(TimedValue* _value_list, uint32_t _count);

	virtual	
	RetValue		SetProperties(const	JSONNode&	_endpoint);

	static 	
	Type			StringToType(const std::string& _name);
	static 	const
	std::string&	TypeToString(Type _type);

	friend	
	std::ostream& 	operator<<(std::ostream& _os, const Endpoint& _ep);

protected:
	RetValue		SetObjectManager(ObjectManager* _object_manager);
	ObjectManager*	GetObjectManager()	{	return	object_manager_;	}
	RetValue		ReleaseObjectManager();

	RetValue		SetDevice(Device *_device);
	Device*			GetDevice()		{	return	device_;	}
	RetValue		ReleaseDevice();

	Properties		properties_;

	ObjectManager*	object_manager_;
	Device*			device_;
	bool			activation_;

	std::list<TimedValue>	value_list_;
};



#include "endpoint_sensor.h"
#include "endpoint_control.h"
#endif
