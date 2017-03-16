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

	Type			GetType();
	uint32_t		GetIndex();
	const
	std::string&	GetID();
	const 
	std::string&	GetName();
	const
	std::string&	SetName(const std::string& _name);

	bool			IsEnabled();
	RetValue		SetEnable(bool _enable);

	uint32			UpdateInterval();
	void			UpdateInterval(uint32_t	_interval);
	
	RetValue		Activation();
	RetValue		Deactivation();
	bool			IsActivated();

	virtual
	RetValue		Synchronize();

	uint32_t		MaxValueCount()	;
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
	void			ReleaseParent();

	Properties		properties_;

	bool			activation_;

	std::list<TimedValue>	value_list_;
};



#include "endpoint_sensor.h"
#include "endpoint_control.h"
#endif
