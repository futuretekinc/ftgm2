#ifndef	SNMP_OBJECT_H_
#define	SNMP_OBJECT_H_

#include <string>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/library/oid.h>
#include "defined.h"
#include "object.h"
#include "value.h"

class	SNMPObject : public  Object
{
public:
	struct	OID
	{
		oid		id[32];
		int		length;
	};

					SNMPObject(const SNMPObject& _object);
					SNMPObject(oid *_oid, int _length);
					SNMPObject(const OID& _oid);
	virtual			~SNMPObject();

	const 
	OID&			GetOID()	{	return	oid_;	}

	virtual 
	RetValue		SetValue(struct variable_list *_variable);
	virtual 
	RetValue		SetValue(const TimedValue& _value);
	const
	TimedValue&		GetValue();

	virtual	
	netsnmp_pdu*	CreateGetPDU(int _timeout = 5);
	virtual	
	netsnmp_pdu*	CreateSetPDU(int _timeout = 5);

protected:
	OID				oid_;
	TimedValue		value_;
};

#endif

