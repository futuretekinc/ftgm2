#include <iostream>
#include "data_manager.h"
#include "KompexSQLiteException.h"
#include "trace.h"

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
		TRACE(this, "SQLite Version : %d", database_->GetLibVersionNumber());
		
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

			query << "CREATE TABLE devices (id TEXT NOT NULL PRIMARY KEY, type INTEGER, enable INTEGER, name TEXT)";
			TRACE(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->SqlStatement(query.str());
				statement_->ExecuteAndFree();
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
	Device *_device
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		if (IsDeviceExist(_device))
		{
			ret_value = CreateDeviceTable();
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to create table[devices]");
				return	ret_value;	
			}
		}

		ostringstream	query;

		query << "INSERT INTO devices (id, type, enable, name) Values (\"" << _device->GetID() << "\", " << _device->GetType() << ", " << _device->IsEnabled() << ", \"" << _device->GetName() << "\");";
		TRACE(this, "Query : %s", query.str().c_str());
		try 
		{
			statement_->SqlStatement(query.str());
			statement_->ExecuteAndFree();
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to add device to table[%s]", _device->GetID().c_str());
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
 Device* _device
 )
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "SELECT id FROM devices WHERE id=" << "'" << _device->GetID() << "';";

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
			ERROR(this, ret_value, "Failed to check device from table[%s]", _device->GetID().c_str());
		}

	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
}

RetValue	DataManager::GetDeviceCount
(
	uint32_t& _count
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "SELECT count(id) FROM devices;";

		try
		{
			_count = statement_->SqlAggregateFuncResult(query.str());
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

	return	ret_value;
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

			query << "SELECT * FROM devices ORDER BY id DESC LIMIT " << _count << " OFFSET " << _index;
			TRACE(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->Sql(query.str());
				while(statement_->FetchRow())
				{
					Device::Type type = (Device::Type)statement_->GetColumnInt("TYPE");
					string 		id = statement_->GetColumnString("ID");
					string 		name = statement_->GetColumnString("NAME");
					bool		enable = statement_->GetColumnInt("ENABLE");

					Device::Properties* properties = new Device::Properties(type);


					properties->id		= id;
					properties->name 	= name;
					properties->enable	=enable;

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

RetValue	DataManager::CreateEndpointTable()
{
	RetValue	ret_value = RET_VALUE_OK;

	TRACE_ENTRY(this);
	if (statement_ != NULL)
	{
		if (!IsTableExist("endpoints"))
		{
			ostringstream	query;

			query << "CREATE TABLE endpoints (id TEXT NOT NULL PRIMARY KEY, type INTEGER, _index INTEGER, enable INTEGER, name TEXT)";
			TRACE(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->SqlStatement(query.str());
				statement_->ExecuteAndFree();
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
	Endpoint* _endpoint
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

		ostringstream	query;

		query << "INSERT INTO endpoints (id, type, _index, enable, name) Values (\"" << _endpoint->GetID() << "\", " ;
		query << _endpoint->GetType() << ", " << _endpoint->GetIndex() << ", " << _endpoint->IsEnabled() << ", \"" << _endpoint->GetName() << "\")";
		TRACE(this, "Query : %s", query.str().c_str());
		try 
		{
			statement_->SqlStatement(query.str());
			statement_->ExecuteAndFree();
		}
		catch(SQLiteException &exception)
		{
			ret_value = RET_VALUE_DB_ERROR | exception.GetSqliteResultCode();
			ERROR(this, ret_value, "Failed to add device to table[%s]", _endpoint->GetID().c_str());
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
 Endpoint* _endpoint
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "SELECT id FROM endpoints WHERE id=" << "'" << _endpoint->GetID() << "';";

		statement_->Sql(query.str());

		if (statement_->GetColumnCount() != 0)
		{
			statement_->FreeQuery();
			return	true;
		}

		statement_->FreeQuery();

	}
	else
	{
		ret_value = RET_VALUE_DB_NOT_INITIALIZED;	
		ERROR(this, ret_value, "DB not initialized.");
	}

	return	ret_value;
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
		if (IsTableExist("devices"))
		{
			ostringstream	query;

			query << "SELECT * FROM endpoints ORDER BY id DESC LIMIT " << _count << " OFFSET " << _index <<";";
			TRACE(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->Sql(query.str());

				while(statement_->FetchRow())
				{
					TRACE(this, "get id : %s", statement_->GetColumnDeclaredDatatype(0));
					string 		id = statement_->GetColumnString("id");
					TRACE(this, "get type : %s", statement_->GetColumnDeclaredDatatype(1));
					Endpoint::Type type = (Endpoint::Type)statement_->GetColumnInt("type");
					TRACE(this, "get index");
					uint32_t	index = statement_->GetColumnInt("_index");
					TRACE(this, "get name");
					string 		name = statement_->GetColumnString("name");
					TRACE(this, "get enable");
					bool		enable = statement_->GetColumnInt("enable");

					Endpoint::Properties* properties = new Endpoint::Properties(type);


					properties->id		= id;
					properties->index	= index;
					properties->name 	= name;
					properties->enable	=enable;

					_list.push_back(properties);
					
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

			query << "CREATE TABLE " << _id << " (time INTEGER NOT NULL PRIMARY KEY, value DOUBLE)";
			TRACE(this, "Query : %s", query.str().c_str());

			try 
			{
				statement_->SqlStatement(query.str());
				statement_->ExecuteAndFree();
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

RetValue	DataManager::GetEndpointCount
(
	uint32_t& _count
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (statement_ != NULL)
	{
		ostringstream	query;

		query << "SELECT COUNT(id) FROM endpoints;";

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

			query << "INSERT INTO " <<  _id << " (time, value) Values (";
			query << _value_list[i].GetTime().ToString() << ", " ;
			query << _value_list[i].ToFloat();
			query << ")" ;
			TRACE(this, "Query : %s", query.str().c_str());
			try 
			{
				statement_->SqlStatement(query.str());
				statement_->ExecuteAndFree();
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
		TRACE(this, "Query : %s", query.str().c_str());

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

