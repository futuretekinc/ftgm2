#ifndef	DEVICE_SNMP_H_
#define	DEVICE_SNMP_H_

#include "string.h"
#include "device.h"
#include "snmp_session.h"

class	DeviceSNMP : public Device
{
public:
	struct	Properties : Device::Properties
	{
		std::string	peer;
		std::string	community;

					Properties(Type _type = TYPE_SNMP) : Device::Properties(_type){}
					Properties(const JSONNode& _node);

		RetValue 	Set(const JSONNode&	_node);
		RetValue 	SetPeer(const JSONNode& _node);
		RetValue 	SetCommunity(const JSONNode& _node);

		virtual	
		void		Show();
	};

	// Constructor & Destructor
					DeviceSNMP();
					DeviceSNMP(const Properties& _properties);
					DeviceSNMP(const Properties* _properties);
					~DeviceSNMP();

	// Interface
	const 
	std::string&	Peer()		{	return	peer_;	}
	void			Peer(const std::string& _peer)	{	peer_ = _peer;	}
	const 
	std::string&	Community()	{	return	community_;	}
	void			Community(const std::string& _community)	{	community_ = _community;	}

	RetValue		SetProperties (const JSONNode&	_node);

	RetValue		GetValue(Endpoint* _endpoint);
protected:
	std::string		peer_;
	std::string		community_;

	SNMPSession		snmp_session_;

	void 			Process();

	void			OnActivated();
	void			OnDeactivated();
};

#endif
