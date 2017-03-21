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
		Properties(Type _type = TYPE_FTE);
	};

				DeviceFTE();
				~DeviceFTE();

	virtual
	RetValue	GetEndpointValue(Endpoint* _endpoint);
	virtual
	RetValue	SetEndpointValue(Endpoint* _endpoint);

protected:
	SNMPObject*		GetValueObject(Endpoint::Type _type, uint32_t _index);
};

#endif
