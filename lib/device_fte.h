#ifndef	DEVICE_FTE_H_
#define	DEVICE_FTE_H_

#include "device_snmp.h"
#include <map>
#include "endpoint.h"

class	DeviceFTE : public DeviceSNMP
{
public:
	struct	Properties : DeviceSNMP::Properties
	{
		Properties() : DeviceSNMP::Properties(TYPE_FTE){}
		Properties(const JSONNode& _node);
	};

				DeviceFTE();
				DeviceFTE(const Properties& _properties);
				DeviceFTE(const Properties* _properties);
				~DeviceFTE();

	virtual	
	RetValue	SetProperties (const JSONNode&	node);

	virtual
	RetValue		GetValue(Endpoint* _endpoint);
	virtual
	RetValue		SetValue(Endpoint* _endpoint);

protected:
	SNMPObject*		GetValueObject(Endpoint::Type _type, uint32_t _index);
};

#endif
