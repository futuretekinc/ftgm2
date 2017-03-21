#include <iomanip>
#include "device_snmp.h"
#include "trace.h"
#include "snmp_client.h"
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"

using namespace std;

DeviceSNMP::Properties::Properties
(	Type _type
)
: Device::Properties(_type)
{
}

#if 0
DeviceSNMP::Properties::Properties
(
	const JSONNode& _node
) 
: Device::Properties(_node)
{
	INFO(NULL, "The DeviceSNMP properties created." );

	type = TYPE_SNMP;
}
#endif

RetValue DeviceSNMP::Properties::Set
(
	Kompex::SQLiteStatement*	_statement
)
{
	RetValue ret_value;
	
	ret_value = Device::Properties::Set(_statement);
	if (ret_value == RET_VALUE_OK)
	{
		try
		{
			Options* options = (Options *)_statement->GetColumnBlob("_options");
			if (options != NULL)
			{
				peer = options->peer;
				community = options->community;
			}
		}
		catch(Kompex::SQLiteException& exception)
		{
			ret_value = RET_VALUE_DB_ENTRY_DOES_NOT_EXIST;
			ERROR(NULL, ret_value, "Failed to set properties!");
		}
	}

	return	ret_value;
}

RetValue DeviceSNMP::Properties::Set
(
	const JSONNode&	_node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (strcasecmp(_node.name().c_str(), "peer") == 0)
	{
		if ((_node.type() != JSON_STRING) && (_node.type() != JSON_NULL))
		{
			ret_value = RET_VALUE_INVALID_FIELD;
			ERROR(NULL, ret_value, "Failed to set field[%s].", _node.name().c_str());
		}
		else
		{
			peer = _node.as_string();
		}

	}
	else if (strcasecmp(_node.name().c_str(), "community") == 0)
	{
		if ((_node.type() != JSON_STRING) && (_node.type() != JSON_NULL))
		{
			ret_value = RET_VALUE_INVALID_FIELD;
			ERROR(NULL, ret_value, "Failed to set field[%s].", _node.name().c_str());
		}
		else
		{
			community = _node.as_string();
		}

	}
	else 
	{
		ret_value = Device::Properties::Set(_node);
	}

	return	ret_value;
}

RetValue	DeviceSNMP::Properties::SetProperty
(
	const std::string& _name, 
	const std::string& _value
)
{
	if (_name == "peer")
	{
		peer= _value;	
	}
	else if (_name == "community")
	{
		community= _value;	
	}
	else
	{
		return	Device::Properties::SetProperty(_name, _value);
	}

	return	RET_VALUE_OK;
}

uint32	DeviceSNMP::Properties::GetOptionsSize()
{
	return	sizeof(Options);
}

uint32	DeviceSNMP::Properties::GetOptions
(
	uint8_t *_options, 
	uint32 _options_len
)
{
	if (_options_len > sizeof(Options))
	{
		peer.copy(((Options*)_options)->peer, sizeof(((Options*)_options)->peer) - 1); 
		community.copy(((Options*)_options)->community, sizeof(((Options*)_options)->community) - 1); 

		return	sizeof(Options);
	}

	return	0;
}

///////////////////////////////////////////////////////
// Device for SNMP
///////////////////////////////////////////////////////

DeviceSNMP::DeviceSNMP
(
	Type _type
)
:	Device(_type)
{
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);

	properties->peer 		= "";
	properties->community 	= "public";
}

DeviceSNMP::~DeviceSNMP()
{
	snmp_session_.Close();
}

const 
std::string&	DeviceSNMP::GetPeer()
{	
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);

	return	properties->peer;	
}

void	DeviceSNMP::SetPeer
(
	const std::string& _peer
)	
{	
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);

	properties->peer = _peer;
}

const 
std::string&	DeviceSNMP::GetCommunity()	
{	
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);

	return	properties->community;	
}

void	DeviceSNMP::SetCommunity
(
	const std::string& _community
)	
{	
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);

	properties->community = _community;
}

RetValue DeviceSNMP::SetProperty
(
	const string&	_field,
	const string&	_value
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);

	if (_field == "peer")
	{
		properties->peer = _value;
	}
	else if (_field == "community")
	{
		properties->community = _value;
	}
	else
	{
		return	Device::SetProperty(_field, _value);	
	}

	return	ret_value;
	
}

RetValue DeviceSNMP::SetProperties
(
	const Properties& _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);
	
	ret_value = Device::SetProperties(_properties);
	if (ret_value == RET_VALUE_OK)
	{
		properties->peer 		= _properties.peer;
		properties->community 	= _properties.community;
	}

	return	ret_value;
}

RetValue DeviceSNMP::SetProperties
(
	const Properties* _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);
	
	ret_value = Device::SetProperties(_properties);
	if (ret_value == RET_VALUE_OK)
	{
		properties->peer 		= _properties->peer;
		properties->community 	= _properties->community;
	}

	return	ret_value;
}

RetValue DeviceSNMP::SetProperties
(
	Kompex::SQLiteStatement*	_statement
)
{
	RetValue ret_value;
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);

	ret_value = Device::SetProperties(_statement);
	if (ret_value == RET_VALUE_OK)
	{
		try
		{
			Options* options = (Options *)_statement->GetColumnBlob("_options");
			if (options != NULL)
			{
				properties->peer = options->peer;
				properties->community = options->community;
			}
		}
		catch(Kompex::SQLiteException& exception)
		{
			ret_value = RET_VALUE_DB_ENTRY_DOES_NOT_EXIST;
			ERROR(NULL, ret_value, "Failed to set properties!");
		}
	}

	return	ret_value;
}

RetValue	DeviceSNMP::SetProperties
(
	const	JSONNode&	node
)
{
	RetValue	ret_value(RET_VALUE_OK);
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);

	if (node.type() == JSON_NODE)
	{
		for(size_t i = 0 ; i < node.size() ; i++)
		{
			ret_value = SetProperties(node[i]);
			if (ret_value != RET_VALUE_OK)
			{
				break;	
			}
		}
	}
	else 
	{
		if (node.name() == "peer")
		{
			if ((node.type() != JSON_STRING) && (node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "");	

			}
			else
			{
				properties->peer = node.as_string();
			}
		}
		else if (node.name() == "community")
		{
			if ((node.type() != JSON_STRING) && (node.type() != JSON_NULL))
			{
				ret_value = RET_VALUE_INVALID_FIELD;
				ERROR(this, ret_value, "");	
			}
			else
			{
				properties->community = node.as_string();
			}
		}
		else
		{
			ret_value = Device::SetProperties(node);	
		}
	}

	return	ret_value;
}

RetValue	DeviceSNMP::GetEndpointValue
(
	Endpoint* endpoint
)
{
	return	RET_VALUE_OK;
}

void 	DeviceSNMP::Process()
{
	RetValue	ret_value = RET_VALUE_OK;
	Properties	*properties = dynamic_cast<DeviceSNMP::Properties*>(properties_);

	if (!snmp_session_.IsOpened())
	{
		if (properties->peer.length() != 0)
		{
			snmp_session_.SetPeer(properties->peer);
			snmp_session_.SetCommunity(properties->community);

			ret_value = snmp_session_.Open();	
			if (ret_value != RET_VALUE_OK)
			{
				ERROR(this, ret_value, "Failed to open SNMP session!");		
			}
		}
	}
}

// Message Handler
void	DeviceSNMP::OnActivated()
{
}

void	DeviceSNMP::OnDeactivated()
{
}
