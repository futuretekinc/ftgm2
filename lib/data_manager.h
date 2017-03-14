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
	RetValue	AddDevice(Device* _device);
	bool		IsDeviceExist(Device* _device);
	RetValue	GetDeviceCount(uint32_t& _count);
	RetValue	GetDeviceProperties(uint32_t _index, uint32_t _count, std::list<Device::Properties*>& _list);

	RetValue	CreateEndpointTable();
	RetValue	AddEndpoint(Endpoint* _endpoint);
	bool		IsEndpointExist(Endpoint* _endpoint);
	RetValue	GetEndpointCount(uint32_t& _count);
	RetValue	GetEndpointProperties(uint32_t _index, uint32_t _count, std::list<Endpoint::Properties*>& _list);

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
