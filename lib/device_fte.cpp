#include "device_fte.h"
#include "trace.h"
#include "snmp_object.h"

using namespace std;

SNMPObject::OID	fte_oid_temperature;

static	uint32_t	_OID_map_reference_count = 0;	
static map<string, SNMPObject *>	_OID_map;

DeviceFTE::Properties::Properties()
: DeviceSNMP::Properties(TYPE_FTE)
{
	if (_OID_map_reference_count == 0)
	{
		_OID_map_reference_count++;
	}
}

///////////////////////////////////////////////////////
// Device for FTE
///////////////////////////////////////////////////////

DeviceFTE::DeviceFTE()
:	DeviceSNMP(TYPE_FTE)
{
}

DeviceFTE::~DeviceFTE()
{
	if (--_OID_map_reference_count == 0)
	{
		map<string,SNMPObject *>::iterator it = _OID_map.begin();
		while(it != _OID_map.end())
		{
			delete it->second;

			_OID_map.erase(it);	
		}
	}
}

RetValue	DeviceFTE::SetProperties
(
	const	JSONNode&	node
)
{
	RetValue	retval(RET_VALUE_OK);

	if (node.type() == JSON_NODE)
	{
		for(size_t i = 0 ; i < node.size() ; i++)
		{
			retval = SetProperties(node[i]);
			if (retval != RET_VALUE_OK)
			{
				break;	
			}
		}
	}
	else 
	{
		retval = DeviceSNMP::SetProperties(node);	
	}

	return	retval;
}

RetValue	DeviceFTE::GetEndpointValue
(
	Endpoint* _endpoint
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (snmp_session_.IsOpened())
	{
		SNMPObject*	snmp_object = GetValueObject(_endpoint->GetType(), _endpoint->GetIndex());

		if (snmp_object == NULL)
		{
			ret_value = RET_VALUE_SNMP_OBJECT_NOT_FOUND;
			ERROR(this, ret_value, "SNMP object of the endpoint[%s] count not be found.", _endpoint->GetID().c_str());
		}
		else
		{
			ret_value = snmp_session_.Get(snmp_object);
			if (ret_value == RET_VALUE_OK)
			{
				_endpoint->AddValue(snmp_object->GetValue());
			}
			else
			{
				ERROR(this, ret_value, "Failed to get data.");
			}
		}
	}
	else
	{
		ret_value = RET_VALUE_SNMP_SESSION_IS_NOT_CONNECTED;
		ERROR(this, ret_value, "SNMP session is not connected.");
	}

	return ret_value;
}

RetValue	DeviceFTE::SetEndpointValue
(
	Endpoint* _endpoint
)
{
	RetValue	ret_value = RET_VALUE_OK;

	return ret_value;
}

SNMPObject*	DeviceFTE::GetValueObject
(
	Endpoint::Type 	_type, 
	uint32_t 		_index
)
{
	string	key = Endpoint::TypeToString(_type) + to_string(_index);
	SNMPObject*	object = NULL;

	object = _OID_map[key];
	if (object == NULL)
	{
		SNMPObject::OID	fte_oid_base_value =
		{
			.id = {1, 3, 6, 1, 4, 1, 42251, 1, 3, 0, 2, 1, 6, 0},
			.length = 14
		};

		switch(_type)
		{
		case	Endpoint::Type::TEMPERATURE_SENSOR:	fte_oid_base_value.id[9] = 1 << 8;	break;
		case	Endpoint::Type::HUMIDITY_SENSOR:  	fte_oid_base_value.id[9] = 2 << 8;	break;
		case	Endpoint::Type::VOLTAGE_SENSOR:  	fte_oid_base_value.id[9] = 3 << 8;	break;
		case	Endpoint::Type::CURRENT_SENSOR:  	fte_oid_base_value.id[9] = 4 << 8;	break;
		case	Endpoint::Type::DI_SENSOR:			fte_oid_base_value.id[9] = 5 << 8;	break;
		case	Endpoint::Type::DO_CONTROL:			fte_oid_base_value.id[9] = 6 << 8;	break;
		default: return	NULL;	
		}

		fte_oid_base_value.id[13] = _index;

		try 
		{
			object = new SNMPObject(fte_oid_base_value);
		}
		catch(exception& e)
		{
			object = NULL;
		}

		_OID_map[key] = object;
	}

	return	object;
}

