#include <iostream>
#include "data_manager.h"
#include "KompexSQLiteException.h"
#include "trace.h"
#include "device_snmp.h"

using namespace	Kompex;
using namespace	std;

DataManager::Properties::Properties()
{
	file_name = "ftgm.db";
}

DataManager::DataManager()
: MessageProcess()
{

}


void	DataManager::PreProcess()
{
	try 
	{
		database_ = new SQLiteDatabase(properties_.file_name, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
		INFO(this, "SQLite Version : %d", database_->GetLibVersionNumber());

 		statement_ = new Kompex::SQLiteStatement(database_);

		if (!IsTableExist("devices"))
		{
			CreateDeviceTable();	
		}

		if (!IsTableExist("endpoints"))
		{
			CreateEndpointTable();	
		}
	}
  	catch(Kompex::SQLiteException &exception)
	{
		std::cerr << "\nException occured" << std::endl;
		exception.Show();
		std::cerr << "SQLite result code: " << exception.GetSqliteResultCode() << std::endl;
	}
}

void	DataManager::Process()
{
}

void	DataManager::PostProcess()
{
	try 
	{
		if (statement_ != NULL)
		{
			delete statement_;	
		}

		if (database_ != NULL)
		{
			delete database_;
		}
	}
  	catch(Kompex::SQLiteException &exception)
	{
		std::cerr << "\nException occured" << std::endl;
		exception.Show();
		std::cerr << "SQLite result code: " << exception.GetSqliteResultCode() << std::endl;
	}
}

RetValue	DataManager::CreateDeviceTable()
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		if (!IsTableExist("devices"))
		{
			ostringstream	query;

			query << "CREATE TABLE devices (_id TEXT NOT NULL PRIMARY KEY, _type INTEGER, _enable INTEGER, _name TEXT, _options TEXT)";
			INFO(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->SqlStatement(query.str());
			}
			catch(SQLiteException &exception)
			{
				ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
				ERROR(this, ret_value, "Failed to create table[devices]");
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::AddDevice
(
	Device::Properties *_properties
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Add a device[%s]", _properties->id.c_str());
	if (statement_ != NULL)
	{
		if (IsDeviceExist(_properties->id))
		{
			ret_value = CreateDeviceTable();
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to create table[devices]");
				return	ret_value;	
			}
		}

		char*		options = NULL;
		uint32_t	options_len = _properties->GetOptionsSize()*2+1;

		options = new char [options_len];	
		memset(options, 0, options_len);

		_properties->GetOptions(options, options_len);

		try 
		{
			ostringstream	query;
			uint32_t		index = 0;

			query << "INSERT INTO devices (_id, _type, _enable, _name, _options) VALUES (?,?,?,?,?);";
			INFO(this, "Query : %s", query.str().c_str());

			statement_->Sql(query.str());
			statement_->BindString(++index, _properties->id);
			statement_->BindInt(++index, 	_properties->type);
			statement_->BindInt(++index, 	_properties->enable);
			statement_->BindString(++index, _properties->name);
			statement_->BindString(++index, options);

			statement_->ExecuteAndFree();

		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to add device to table[%s] - %s", _properties->id.c_str(), exception.GetString().c_str());
		}

		if (options != NULL)
		{
			delete options;	
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::SetDeviceProperties
(
	Device::Properties *_properties
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Set a device properties[%s]", _properties->id.c_str());
	if (statement_ != NULL)
	{
		if (!IsDeviceExist(_properties->id))
		{
			ret_value = RET_VALUE_DB_ENTRY_DOES_NOT_EXIST;
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to set device properties!");
				return	ret_value;	
			}
		}

		char*	options = NULL;
		uint32_t	options_len = _properties->GetOptionsSize() * 2 + 1;

		options = new char[options_len];	
		memset(options, 0, options_len);
		_properties->GetOptions(options, options_len);

		try 
		{
			int	index = 0;
			ostringstream	query;

			query << "UPDATE devices SET _enable=@enable, _name =@name, _options=@options WHERE _id=@id";
			INFO(this, "Query : %s", query.str().c_str());


			statement_->Sql(query.str());

			statement_->BindInt(++index, _properties->enable);
			statement_->BindString(++index, _properties->name);
			statement_->BindString(++index, options);
			statement_->BindString(++index, _properties->id);

			statement_->ExecuteAndFree();
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to add device to table[%s] - %s", _properties->id.c_str(), exception.GetString().c_str());
		}

		if(options != 0)
		{
			delete options;
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::DeleteDevice
(
	const string& _id
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "DELETE FROM devices WHERE _id=" << "'" << _id << "';";
		INFO(this, "Query : %s", query.str().c_str());

		try 
		{
			statement_->Sql(query.str());

			if (statement_->GetColumnCount() != 0)
			{
				statement_->FreeQuery();
				return	true;
			}

			statement_->FreeQuery();
		}
		catch(SQLiteException& exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to delete device from table[%s]", _id.c_str());
		}

	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

bool	DataManager::IsDeviceExist
(
	const string&	_id
 )
{
	bool		exist = false;
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "SELECT COUNT(*) FROM devices WHERE _id = " << "'" << _id << "';";
		INFO(this, "Query : %s", query.str().c_str());

		try 
		{
			if (statement_->SqlAggregateFuncResult(query.str()) != 0)
			{
				exist = true;
			}
		}
		catch(SQLiteException& exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to check device from table[%s]", _id.c_str());
		}

	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	exist;
}

uint32_t	DataManager::GetDeviceCount()
{
	RetValue	ret_value = RET_VALUE_OK;
	uint32_t	count = 0;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "SELECT COUNT(*) FROM devices;";
		INFO(this, "Query : %s", query.str().c_str());

		try
		{
			count = statement_->SqlAggregateFuncResult(query.str());
		}
		catch(SQLiteException& exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to get device count");
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	count;
}

RetValue	DataManager::GetDeviceProperties
(
	uint32_t 	_index, 
	uint32_t 	_count, 
	list<Device::Properties*>& _list
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		if (IsTableExist("devices"))
		{
			ostringstream	query;

			query << "SELECT * FROM devices ORDER BY _id DESC LIMIT " << _count << " OFFSET " << _index <<";";
			INFO(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->Sql(query.str());
				while(statement_->FetchRow())
				{
					Device::Type type = (Device::Type)statement_->GetColumnInt("_type");

					Device::Properties* properties = Device::Properties::Create(type);
	
					if (properties != NULL)
					{
						INFO(this, "Set Properties");
						properties->Set(statement_);
						INFO(this, "Set Properties");
				
					}
					
					_list.push_back(properties);
					
				}
				statement_->FreeQuery();
			}
			catch(SQLiteException &exception)
			{
				ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
				ERROR(this, ret_value, "Failed to get device properties");
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

Device::Properties*	DataManager::GetDeviceProperties
(
	const string& _id
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Device::Properties* properties = NULL;

	if (statement_ != NULL)
	{
		if (IsTableExist("devices"))
		{
			ostringstream	query;

			query << "SELECT * FROM devices WHERE _id = " << _id << ";";
			INFO(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->Sql(query.str());
				if (statement_->FetchRow())
				{
					Device::Type type = (Device::Type)statement_->GetColumnInt("_type");

					properties = Device::Properties::Create(type);
					if (properties != NULL)
					{
						ret_value = properties->Set(statement_);
						if (ret_value != RET_VALUE_OK)
						{
							delete properties;	
						}
					}

				}

				statement_->FreeQuery();
			}
			catch(SQLiteException &exception)
			{
				ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
				ERROR(this, ret_value, "Failed to get device properties");
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	properties;
}

RetValue	DataManager::SetDeviceProperty
(
	const string&	_device_id,
	const string&	_field,
	const string&	_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Set a device[%s] name", _device_id.c_str());
	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "UPDATE devices SET _" << _field << "=\'" << _value << "\' WHERE _id=\'" << _device_id << "\'";
		INFO(this, "Query : %s", query.str().c_str());
		try 
		{
			statement_->SqlStatement(query.str());
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to update device name - %s", exception.GetString().c_str());
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::SetDeviceProperty
(
	const string&	_device_id,
	const string&	_field,
	bool	_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Set a device[%s] enable", _device_id.c_str());
	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "UPDATE devices SET _" << _field << "=" << _value << " WHERE _id=\'" << _device_id << "\'";
		INFO(this, "Query : %s", query.str().c_str());
		try 
		{
			statement_->SqlStatement(query.str());
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to update device enable - %s", exception.GetString().c_str());
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::SetDeviceProperty
(
	const string&	_device_id,
	const string&	_field,
	uint32_t		_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Set a device[%s] enable", _device_id.c_str());
	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "UPDATE devices SET _" << _field << "=" << _value << " WHERE _id=\'" << _device_id << "\'";
		INFO(this, "Query : %s", query.str().c_str());
		try 
		{
			statement_->SqlStatement(query.str());
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to update device enable - %s", exception.GetString().c_str());
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::CreateEndpointTable()
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		if (!IsTableExist("endpoints"))
		{
			ostringstream	query;

			query << "CREATE TABLE endpoints (";
			query << "_id TEXT NOT NULL PRIMARY KEY";
			query << ", ";
			query << "_type INTEGER";
			query << ", ";
			query << "_index INTEGER";
			query << ", ";
			query << "_enable INTEGER";
			query << ", ";
			query << "_name TEXT";
			query << ", ";
			query << "_device_id TEXT";
			query << ", ";
			query << "_update_interval INTEGER";
			query << ", ";
			query << "_value_count INTEGER";
			query << ", ";
			query << "_options TEXT)";
			INFO(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->SqlStatement(query.str());
			}
			catch(SQLiteException &exception)
			{
				ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
				ERROR(this, ret_value, "Failed to create table[endpoints]");
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::AddEndpoint
(
	Endpoint::Properties* _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		if (!IsTableExist("endpoints"))
		{
			ret_value =  CreateEndpointTable();
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to create table[endpoints]");
				return	ret_value;
			}
		}

		char*	options= NULL;
		uint32_t	options_len = _properties->GetOptionsSize() * 2 + 1;

		options = new char[options_len];	
		memset(options, 0, options_len);

		_properties->GetOptions(options, options_len);

		try 
		{
			ostringstream	query;
			uint32_t		index = 0;

			query << "INSERT INTO endpoints (_id, _type, _index, _enable, _name, _device_id, _update_interval, _value_count, _options) ";
			query << "Values (?,?,?,?,?,?,?,?,?);";

			statement_->Sql(query.str());
			statement_->BindString(++index, _properties->id);
			statement_->BindInt(++index, _properties->type);
			statement_->BindInt(++index, _properties->index);
			statement_->BindInt(++index, _properties->enable);
			statement_->BindString(++index, _properties->name);
			statement_->BindString(++index, _properties->device_id);
			statement_->BindInt(++index, _properties->update_interval);
			statement_->BindInt(++index, _properties->value_count);
			statement_->BindString(++index, options);

			statement_->ExecuteAndFree();
			
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to add device to table[%s]", _properties->id.c_str());
		}

		if (options != NULL)
		{
			delete options;	
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::DeleteEndpoint
(
	const string& _id
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "DELETE FROM endpoints WHERE _id=" << "'" << _id << "';";
		INFO(this, "Query : %s", query.str().c_str());

		try 
		{
			statement_->Sql(query.str());

			if (statement_->GetColumnCount() != 0)
			{
				statement_->FreeQuery();
				return	true;
			}

			statement_->FreeQuery();
		}
		catch(SQLiteException& exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to delete endpoint from table[%s]", _id.c_str());
		}

	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

bool		DataManager::IsEndpointExist
(
	const string& _id
)
{
	bool		exist = false;
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "SELECT COUNT(_id) FROM endpoints WHERE _id=" << "'" << _id << "';";
		INFO(this, "Query : %s", query.str().c_str());

		if (statement_->SqlAggregateFuncResult(query.str()) != 0)
		{
			exist = true;
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	exist;
}

RetValue	DataManager::GetEndpointProperties
(
	uint32_t 	_index, 
	uint32_t 	_count, 
	list<Endpoint::Properties*>& _list
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		if (IsTableExist("endpoints"))
		{
			ostringstream	query;

			query << "SELECT * FROM endpoints ORDER BY _id DESC LIMIT " << _count << " OFFSET " << _index <<";";
			INFO(this, "Query : %s", query.str().c_str());
			try 
			{
				statement_->Sql(query.str());

				while(statement_->FetchRow())
				{
					Endpoint::Type 	type = (Endpoint::Type)statement_->GetColumnInt("_type");

					INFO(this, "Endpoint type : %s", Endpoint::TypeToString(type).c_str());

					Endpoint::Properties* properties = Endpoint::Properties::Create(type);
					if (properties != NULL)
					{
						ret_value = properties->Set(statement_);
						if (ret_value == RET_VALUE_OK)
						{
							INFO(this, "Add endpoint [%s]", properties->id.c_str());
							_list.push_back(properties);
						}
						else
						{
							delete properties;	
							ERROR(this, ret_value, "Failed to create endpoint properties!");
						}
					}					
				}

				statement_->FreeQuery();
			}
			catch(SQLiteException &exception)
			{
				ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
				ERROR(this, ret_value, "Failed to get endpoint properties");
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

Endpoint::Properties*	DataManager::GetEndpointProperties
(
	const string& _id
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Endpoint::Properties* properties = NULL;

	if (statement_ != NULL)
	{
		if (IsTableExist("endpoints"))
		{
			ostringstream	query;

			query << "SELECT * FROM endpoints WHERE _id = " << _id << ";";
			INFO(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->Sql(query.str());
				if (statement_->FetchRow())
				{
					Endpoint::Type type = (Endpoint::Type)statement_->GetColumnInt("_type");

					properties = Endpoint::Properties::Create(type);
					if (properties != NULL)
					{
						ret_value = properties->Set(statement_);
						if (ret_value != RET_VALUE_OK)
						{
							delete properties;	
						}
					}

				}

				statement_->FreeQuery();
			}
			catch(SQLiteException &exception)
			{
				ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
				ERROR(this, ret_value, "Failed to get endpoint properties");
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	properties;
}

RetValue	DataManager::SetEndpointProperties
(
	Endpoint::Properties* _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		if (!IsTableExist("endpoints"))
		{
			ret_value =  CreateEndpointTable();
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to create table[endpoints]");
				return	ret_value;
			}
		}

		char*	options = NULL;
		uint32_t	options_len = _properties->GetOptionsSize() * 2 + 1;

		options = new char[options_len];	
		memset(options, 0, options_len);

		_properties->GetOptions(options, options_len);

		try 
		{
			int				index=0;
			ostringstream	query;

			query << "UPDATE endpoints SET _index=@index, _enable=@enable, _name=@name, _device_id=@device_id, _update_interval=@update_interval, _value_count=@value_count, _options=@options WHERE _id=@id";
			INFO(this, "Query : %s", query.str().c_str());
			statement_->Sql(query.str());
			statement_->BindInt(++index, _properties->index);
			statement_->BindInt(++index, _properties->enable);
			statement_->BindString(++index, _properties->name);
			statement_->BindString(++index, _properties->device_id);
			statement_->BindInt(++index, _properties->update_interval);
			statement_->BindInt(++index, _properties->value_count);
			statement_->BindString(++index, options);
			statement_->BindString(++index, _properties->id);

			statement_->ExecuteAndFree();
			
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to add endpoint[%s] to table[endpoints] - %s", _properties->id.c_str(), exception.GetString().c_str());
		}

		if (options != NULL)
		{
			delete options;	
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::SetEndpointProperty
(
	const string&	_endpoint_id,
	const string&	_field,
	const string&	_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Set a endpoint[%s] name", _endpoint_id.c_str());
	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "UPDATE endpoints SET _" << _field << "=\'" << _value << "\' WHERE _id=\'" << _endpoint_id << "\'";
		INFO(this, "Query : %s", query.str().c_str());
		try 
		{
			statement_->SqlStatement(query.str());
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to update endpoint name - %s", exception.GetString().c_str());
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::SetEndpointProperty
(
	const string&	_endpoint_id,
	const string&	_field,
	bool	_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Set a endpoint[%s] enable", _endpoint_id.c_str());
	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "UPDATE endpoints SET _" << _field << "=" << _value << " WHERE _id=\'" << _endpoint_id << "\'";
		INFO(this, "Query : %s", query.str().c_str());
		try 
		{
			statement_->SqlStatement(query.str());
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to update endpoint enable - %s", exception.GetString().c_str());
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::SetEndpointProperty
(
	const string&	_endpoint_id,
	const string&	_field,
	uint32_t		_value
)
{
	RetValue	ret_value = RET_VALUE_OK;

	INFO(this, "Set a endpoint[%s] enable", _endpoint_id.c_str());
	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "UPDATE endpoints SET _" << _field << "=" << _value << " WHERE _id=\'" << _endpoint_id << "\'";
		INFO(this, "Query : %s", query.str().c_str());
		try 
		{
			statement_->SqlStatement(query.str());
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to update endpoint enable - %s", exception.GetString().c_str());
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::CreateValueTable
(
	const std::string& _id
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		if (!IsTableExist(_id))
		{
			ostringstream	query;

			query << "CREATE TABLE " << _id << " (_time INTEGER NOT NULL PRIMARY KEY, _value DOUBLE);";
			INFO(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->Sql(query.str());
				statement_->FreeQuery();
			}
			catch(SQLiteException &exception)
			{
				ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
				ERROR(this, ret_value, "Failed to create table[endpoints(%s)]", _id.c_str());
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

uint32_t	DataManager::GetEndpointCount()
{
	RetValue	ret_value = RET_VALUE_OK;
	uint32_t	count = 0;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "SELECT COUNT(_id) FROM endpoints;";

		try
		{
			count = statement_->SqlAggregateFuncResult(query.str());
		}
		catch(SQLiteException& exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to get endpoint count");
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	count;
}

RetValue	DataManager::AddValue
(
	const string&	_id,
	TimedValue* 	_value_list, 
	uint32_t 		_count
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		for(uint32_t i = 0 ; i < _count ; i++)
		{
			ostringstream	query;

			query << "INSERT INTO " << _id <<" (time, value) Values (" << _value_list[i].GetTime().ToString() << "," << _value_list[i].ToFloat() << ");";
			INFO(this, "Query : %s", query.str().c_str());
			try 
			{
				statement_->Sql(query.str());
				statement_->FreeQuery();
			}
			catch(SQLiteException &exception)
			{
				ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
				ERROR(this, ret_value, "Failed to add value to table[%s]", _id.c_str());
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::GetValueCount
(
	const string& _id,
	uint32_t& _count
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "SELECT COUNT(*) FROM " << _id << ";";
		INFO(this, "Query : %s", query.str().c_str());

		try
		{
			_count = statement_->SqlAggregateFuncResult(query.str());
		}
		catch(SQLiteException& exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to get endpoint count");
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

bool	DataManager::IsTableExist
(
	const std::string& _name
)
{
	RetValue	ret_value = RET_VALUE_OK;
	
	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "SELECT COUNT(*) FROM sqlite_master WHERE name=" << "'" << _name << "';";
		INFO(this, "Query : %s", query.str().c_str());

		if (statement_->SqlAggregateFuncResult(query.str()) != 0)
		{
			return	true;
		}
	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	false;
}

