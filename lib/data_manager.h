#ifndef	DATA_MANAGER_H_
#define	DATA_MANAGER_H_

#include <string>
#include <list>
#include "message_process.h"
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteStatement.h"
#include "device.h"

class	DataManager : public MessageProcess
{
public:
	struct	Properties
	{
		std::string	file_name;

		Properties();
	};

	DataManager();


	RetValue	CreateDeviceTable();
	RetValue	AddDevice(Device::Properties* _properties);
	RetValue	DeleteDevice(const std::string& _id);
	bool		IsDeviceExist(const std::string& _id);
	uint32_t	GetDeviceCount();
	RetValue	GetDeviceProperties(uint32_t _index, uint32_t _count, std::list<Device::Properties*>& _list);
	Device::Properties*	GetDeviceProperties(const std::string& _id);

	RetValue	SetDeviceProperties(Device::Properties *_properties);
	RetValue	SetDeviceProperty(const std::string& _id, const std::string& _field, const std::string& _value);
	RetValue	SetDeviceProperty(const std::string& _id, const std::string& _field, uint32_t _value);
	RetValue	SetDeviceProperty(const std::string& _id, const std::string& _field, bool _value);

	RetValue	CreateEndpointTable();
	RetValue	AddEndpoint(Endpoint::Properties* _properties);
	RetValue	DeleteEndpoint(const std::string& _id);
	bool		IsEndpointExist(const std::string& _id);
	uint32_t	GetEndpointCount();
	RetValue	GetEndpointProperties(uint32_t _index, uint32_t _count, std::list<Endpoint::Properties*>& _list);
	Endpoint::Properties*	GetEndpointProperties(const std::string& _id);

	RetValue	SetEndpointProperties(Endpoint::Properties* _properties);
	RetValue	SetEndpointProperty(const std::string& _id, const std::string& _field, const std::string& _value);
	RetValue	SetEndpointProperty(const std::string& _id, const std::string& _field, uint32_t _value);
	RetValue	SetEndpointProperty(const std::string& _id, const std::string& _field, bool _value);

	RetValue	CreateValueTable(const std::string& _id);
	RetValue	AddValue(const std::string& _id, TimedValue* _value_list, uint32_t _count);
	RetValue	GetValueCount(const std::string& _id, uint32_t& _count);

	bool		IsTableExist(const std::string& _name);

private:
	Properties	properties_;
	Kompex::SQLiteDatabase*		database_;
	Kompex::SQLiteStatement*	statement_;

	void		PreProcess();
	void		Process();
	void		PostProcess();

};

#endif
