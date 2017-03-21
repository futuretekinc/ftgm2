#ifndef	ENDPOINT_H_
#define	ENDPOINT_H_

#include "defined.h"
#include <string>
#include <list>
#include "object.h"
#include "libjson/libjson.h"
#include "KompexSQLiteStatement.h"
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
		UNKNOWN = 0,
		TEMPERATURE_SENSOR,
		HUMIDITY_SENSOR,
		VOLTAGE_SENSOR,
		CURRENT_SENSOR,
		DI_SENSOR,

		DO_CONTROL = 100
	};

	struct Properties
	{
					Properties(Type _type = UNKNOWN);
//					Properties(const Properties& _properties);
//					Properties(const Properties* _properties);
		virtual		~Properties();

		static
		Properties*	Create(Type _type);
		static
		Properties* Create(const Properties* _properties);
		static
		Properties*	Create(const JSONNode& _node);
		static
		Properties*	Create(const Kompex::SQLiteStatement*	_statement);

		virtual
		RetValue	Set(const JSONNode& _node);
		virtual		
		RetValue	Set(const Properties* _properties);
		virtual
		RetValue	Set(const Kompex::SQLiteStatement*	_statement);

		virtual
		RetValue	SetProperty(const std::string& _name, const std::string& _value);
		virtual
		RetValue	SetProperty(const std::string& _name, bool  _value);
		virtual
		RetValue	SetProperty(const std::string& _name, uint32_t	_value);
		virtual
		RetValue	SetProperty(const std::string& _name, double	_value);
	
		virtual
		uint32		GetOptionsSize()	{	return	0;	};
		virtual
		uint32		GetOptions(uint8_t *options, uint32 options_len);

		Type		type;
		std::string	id;
		uint32		index;
		std::string	name;
		std::string	device_id;
		bool		enable;
		uint32		update_interval;
		uint32		value_count;	
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

	Type			GetType()	{	return	properties_->type;	};

	const 
	std::string&	GetID()		{	return	properties_->id;	};

	const 
	std::string&	GetName()	{	return	properties_->name;	};
	const
	RetValue		SetName(const std::string& _name);

	const
	uint32			GetIndex()	{	return	properties_->index;	};
	RetValue		SetIndex(uint32_t _index);

	const
	std::string&	GetDeviceID()	{	return	properties_->device_id;	}
	RetValue		SetDeviceID(const std::string& _device_id);

	bool			GetEnabled()	{	return	properties_->enable;};
	RetValue		SetEnable(bool _enable);

	uint32			GetUpdateInterval()	{	return	properties_->update_interval;}	;
	RetValue		SetUpdateInterval(uint32	_interval);
	
	uint32			GetMaxValueCount()		{	return	properties_->value_count;	};
	RetValue		SetMaxValueCount(uint32 _count);

	virtual
	RetValue		SetProperty(const std::string& _name, const std::string& _value);
	virtual
	RetValue		SetProperty(const std::string& _name, bool  _value);
	virtual
	RetValue		SetProperty(const std::string& _name, uint32_t	_value);
	virtual
	RetValue		SetProperty(const std::string& _name, double _value);

	virtual
	Properties*		GetProperties();

	RetValue		Activation();
	RetValue		Deactivation();

	bool			GetActivation() { return	activation_;	};
	RetValue		SetActivation(bool _activation);

	virtual
	RetValue		Synchronize();

	uint32			ValueCount();
	RetValue		AddValue(const TimedValue& _value);
	RetValue		DeleteValue(uint32 _count);

	TimedValue		GetValue();
	TimedValue		GetValue(uint32 _index);
	uint32			GetValueList(TimedValue* _value_list, uint32 _count);

	virtual	
	RetValue		Set(const JSONNode&	_endpoint);
	virtual	
	RetValue		Set(const Properties* _properties);
	virtual	
	RetValue		Set(const Kompex::SQLiteStatement*	_statement);

	static 	
	Type			StringToType(const std::string& _name);
	static 	const
	std::string&	TypeToString(Type _type);

	friend	
	std::ostream& 	operator<<(std::ostream& _os, const Endpoint& _ep);

protected:
	void			ReleaseParent();

	Properties*		properties_;

	bool			activation_;

	ObjectManager*	object_manager_;
	std::list<TimedValue>	value_list_;
};



#include "endpoint_sensor.h"
#include "endpoint_control.h"
#endif
