#ifndef	DEVICE_SNMP_H_
#define	DEVICE_SNMP_H_

#include "string.h"
#include "device.h"
#include "snmp_session.h"

class	DeviceSNMP : public Device
{
public:
	struct	Options
	{
		char			peer[SNMP_PEER_MAX_LEN + 1];
		char			community[SNMP_COMMUNITY_MAX_LEN + 1];
	};

	struct	Properties : Device::Properties
	{
		std::string	peer;
		std::string	community;

					Properties(Type _type = TYPE_SNMP) : Device::Properties(_type){}
					Properties(const JSONNode& _node);

		RetValue 	Set(const JSONNode&	_node);
		RetValue	Set(Kompex::SQLiteStatement*	_statement);

		virtual
		uint32		GetOptionsSize();
		virtual
		uint32		GetOptions(uint8_t *options, uint32 options_len);
	};

	// Constructor & Destructor
					DeviceSNMP(Type _type);
					~DeviceSNMP();

	// Interface
	const 
	std::string&	GetPeer();
	void			SetPeer(const std::string& _peer);

	const 
	std::string&	GetCommunity();
	void			SetCommunity(const std::string& _community);

	RetValue		SetProperties(const Properties* _properties);
	RetValue		SetProperties(const Properties& _properties);
	RetValue 		SetProperties(Kompex::SQLiteStatement*	_statement);
	RetValue		SetProperties(const JSONNode&	_node);

	RetValue		GetEndpointValue(Endpoint* _endpoint);
protected:
	std::string		peer_;
	std::string		community_;

	SNMPSession		snmp_session_;

	void 			Process();

	void			OnActivated();
	void			OnDeactivated();
};

#endif
