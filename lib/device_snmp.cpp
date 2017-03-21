#include <iomanip>
#include "device_snmp.h"
#include "trace.h"
#include "snmp_client.h"
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"

using namespace std;

DeviceSNMP::Properties::Properties
(
	const JSONNode& _node
) 
: Device::Properties(_node)
{
	INFO(NULL, "The DeviceSNMP properties created." );

	type = TYPE_SNMP;
}

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
	peer_ 		= "127.0.0.1";
	community_ 	= "public";
}

DeviceSNMP::~DeviceSNMP()
{
	snmp_session_.Close();
}

const 
std::string&	DeviceSNMP::GetPeer()
{	
	return	peer_;	
}

void	DeviceSNMP::SetPeer
(
	const std::string& _peer
)	
{	
	peer_ = _peer;
}

const 
std::string&	DeviceSNMP::GetCommunity()	
{	
	return	community_;	
}

void	DeviceSNMP::SetCommunity
(
	const std::string& _community
)	
{	
	community_ = _community;
}

RetValue DeviceSNMP::SetProperties
(
	const Properties& _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;
	
	ret_value = Device::SetProperties(_properties);
	if (ret_value == RET_VALUE_OK)
	{
		peer_ 		= _properties.peer;
		community_ 	= _properties.community;
	}

	return	ret_value;
}

RetValue DeviceSNMP::SetProperties
(
	const Properties* _properties
)
{
	RetValue	ret_value = RET_VALUE_OK;
	
	ret_value = Device::SetProperties(_properties);
	if (ret_value == RET_VALUE_OK)
	{
		peer_ 		= _properties->peer;
		community_ 	= _properties->community;
	}

	return	ret_value;
}

RetValue DeviceSNMP::SetProperties
(
	Kompex::SQLiteStatement*	_statement
)
{
	RetValue ret_value;

	ret_value = Device::SetProperties(_statement);
	if (ret_value == RET_VALUE_OK)
	{
		try
		{
			Options* options = (Options *)_statement->GetColumnBlob("_options");
			if (options != NULL)
			{
				peer_ = options->peer;
				community_ = options->community;
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
				peer_ = node.as_string();
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
				community_ = node.as_string();
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

	if (!snmp_session_.IsOpened())
	{
		snmp_session_.SetPeer(peer_);
		snmp_session_.SetCommunity(community_);

		ret_value = snmp_session_.Open();	
		if (ret_value != RET_VALUE_OK)
		{
			ERROR(this, ret_value, "Failed to open SNMP session!");		
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
