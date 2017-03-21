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
		Properties();
	};

				DeviceFTE();
				~DeviceFTE();

	virtual	
	RetValue	SetProperties (const JSONNode&	node);

	virtual
	RetValue	GetEndpointValue(Endpoint* _endpoint);
	virtual
	RetValue	SetEndpointValue(Endpoint* _endpoint);

protected:
	SNMPObject*		GetValueObject(Endpoint::Type _type, uint32_t _index);
};

#endif
