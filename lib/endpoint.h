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
					Properties(Type _type);
		virtual		~Properties();

		static
		Properties*	Create(Type _type);
		static
		Properties* Create(const Properties* _properties);
		static
		Properties*	Create(const JSONNode& _node);
		static
		Properties*	Create(const Kompex::SQLiteStatement*	_statement);
		
		static
		RetValue	Create(const JSONNode& _node, std::list<Properties*>& _properties_list);

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
		uint32		GetOptionsSize();
		virtual
		uint32		GetOptions(uint8_t *options, uint32 options_len);
		virtual
		uint32		GetOptions(char *options, uint32 options_len);

		Type		type;
		std::string	id;
		uint32		index;
		std::string	name;
		std::string	device_id;
		bool		enable;
		uint32		update_interval;
		uint32		value_count;	
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
	std::string&	GetName();
	RetValue		SetName(const std::string& _name);

	uint32			GetIndex();
	RetValue		SetIndex(uint32_t _index);

	const
	std::string&	GetDeviceID();
	RetValue		SetDeviceID(const std::string& _device_id);

	bool			GetEnabled();
	RetValue		SetEnable(bool _enable);

	uint32			GetUpdateInterval();
	RetValue		SetUpdateInterval(uint32	_interval);
	
	uint32			GetMaxValueCount();
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


	virtual
	RetValue		Start();
	RetValue        Stop();
	
	bool            IsRun() { return        activation_;    };

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

	RetValue		Show(std::ostream& _os);

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
