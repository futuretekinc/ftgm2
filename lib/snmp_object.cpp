#include <iostream>
#include "snmp_object.h"
#include <typeinfo>
#include "trace.h"

using namespace std;

SNMPObject::SNMPObject
(
	const SNMPObject& _object
)
{
	oid_ = _object.oid_;	
}

SNMPObject::SNMPObject
(
	oid *_id, 
	int _length
)
{
	if (_length < 32)
	{
		memcpy(oid_.id, _id, sizeof(oid) * _length);
		oid_.length = _length;
	}
}

SNMPObject::SNMPObject
(
	const OID&	_oid
)
{
	oid_ = _oid;
}

SNMPObject::~SNMPObject()
{
} 

netsnmp_pdu*	SNMPObject::CreateGetPDU
(
	int _timeout
)
{
	netsnmp_pdu*	pdu;

	pdu = snmp_pdu_create(SNMP_MSG_GET);
	if (pdu != NULL)
	{
		pdu->time = _timeout;
		snmp_add_null_var(pdu, oid_.id, oid_.length);	
	}

	return	pdu;
}

netsnmp_pdu*	SNMPObject::CreateSetPDU
(
	int 	_timeout
)
{
	netsnmp_pdu*	pdu;

	pdu = snmp_pdu_create(SNMP_MSG_SET);
	if (pdu != NULL)
	{
		pdu->time = _timeout;

		if (value_.GetType() == Value::INT)
		{
			int	value;
			value_.Get(value);

			snmp_add_var(pdu, oid_.id, oid_.length, 'i', (char *)&value);
		}
		else
		{
			snmp_add_var(pdu, oid_.id, oid_.length, 's', value_.ToString().c_str());	
		}
	}

	return	pdu;
}

const
Value&		SNMPObject::GetValue()
{
	return	value_;
}


RetValue	SNMPObject::SetValue
(
	const Value& _value
)
{
	value_ = _value;

	return	RET_VALUE_OK;
}

RetValue	SNMPObject::SetValue
(
	struct variable_list *_variable
)
{
	RetValue 	ret_value = RET_VALUE_OK;

	if (_variable != NULL) 
	{
		switch(_variable->type)
		{
		case	ASN_INTEGER:
			{
				int	value;

				switch (_variable->val_len)
				{
					case	1: 	value = *(int8_t *)_variable->val.integer; break;
					case	2:	value = *(int16_t *)_variable->val.integer; break;
					case	4:	value = *(int32_t *)_variable->val.integer; break;
					default: 	value = 0;
				}

				value_ = Value(value);
			}
			break;

		case	ASN_OCTET_STR:
			{
				if (_variable->val_len != 0)
				{
					char* buffer = new char [_variable->val_len + 1];

					memcpy(buffer, _variable->val.string, _variable->val_len);
					buffer[_variable->val_len] = 0;

					value_ = Value(buffer);
				}
				else
				{
					ret_value = RET_VALUE_INVALID_DATA;
				}
			}
			break;
		default:
			{
				ret_value = RET_VALUE_INVALID_DATA;
				ERROR(this, ret_value, "");
			}
		}
	}

	return	ret_value;
}


