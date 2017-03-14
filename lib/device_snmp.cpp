#include <iomanip>
#include "device_snmp.h"
#include "trace.h"
#include "snmp_client.h"

using namespace std;

DeviceSNMP::Properties::Properties
(
	const JSONNode& _node
) 
: Device::Properties(_node)
{
	TRACE(NULL, "The DeviceSNMP properties created.");

	type = TYPE_SNMP;
}

RetValue DeviceSNMP::Properties::Set
(
	const JSONNode&	_node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (strcasecmp(_node.name().c_str(), "peer") == 0)
	{
		ret_value = SetPeer(_node);
	}
	else if (strcasecmp(_node.name().c_str(), "community") == 0)
	{
		ret_value = SetCommunity(_node);
	}
	else 
	{
		ret_value = Device::Properties::Set(_node);
	}

	return	ret_value;
}

RetValue DeviceSNMP::Properties::SetPeer
(
	const JSONNode& _node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if ((_node.type() != JSON_STRING) && (_node.type() != JSON_NULL))
	{
		ret_value = RET_VALUE_INVALID_FIELD;
		ERROR(NULL, ret_value, "Failed to set field[%s].", _node.name().c_str());
	}
	else
	{
		peer = _node.as_string();
	}

	return	ret_value;
}

RetValue DeviceSNMP::Properties::SetCommunity
(
	const JSONNode& _node
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if ((_node.type() != JSON_STRING) && (_node.type() != JSON_NULL))
	{
		ret_value = RET_VALUE_INVALID_FIELD;
		ERROR(NULL, ret_value, "Failed to set field[%s].", _node.name().c_str());
	}
	else
	{
		community = _node.as_string();
	}

	return	ret_value;
}

void DeviceSNMP::Properties::Show()
{
	Device::Properties::Show();

	cout << setw(16) << "Peer : " << peer << endl;
	cout << setw(16) << "Community : " << community << endl;
}


///////////////////////////////////////////////////////
// Device for SNMP
///////////////////////////////////////////////////////

DeviceSNMP::DeviceSNMP()
:	Device()
{
	properties_.type	= TYPE_SNMP;
	peer_ 		= "127.0.0.1";
	community_ 	= "public";
}

DeviceSNMP::DeviceSNMP
(
	const Properties& _properties
)
: Device(_properties)
{
	peer_ 		= _properties.peer;
	community_ 	= _properties.community;
}

DeviceSNMP::DeviceSNMP
(
	const Properties* _properties
)
: Device(_properties)
{
	peer_ 		= _properties->peer;
	community_ 	= _properties->community;
}

DeviceSNMP::~DeviceSNMP()
{
	snmp_session_.Close();
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

RetValue	DeviceSNMP::GetValue
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
