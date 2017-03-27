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

	class	Properties : public Device::Properties
	{
	public:
		std::string	mib;
		std::string	peer;
		std::string	community;

					Properties(Type _type = TYPE_SNMP);

		RetValue	Set(Device::Properties* _properties);
		RetValue 	Set(const JSONNode&	_node);
		RetValue	Set(Kompex::SQLiteStatement*	_statement);

		RetValue	SetProperty(const std::string& _name, const std::string& _value);

		uint32		GetOptionsSize();
		uint32		GetOptions(uint8_t *options, uint32 options_len);
		uint32		GetOptions(char *buffer, uint32 buffer_len);

		JSONNode	JSON();
		JSONNode	JSON(JSONNode& root);
	};

	// Constructor & Destructor
					DeviceSNMP(Type _type = TYPE_SNMP);
					~DeviceSNMP();

	// Attribute 
	const 
	std::string&	GetPeer();
	void			SetPeer(const std::string& _peer);

	const 
	std::string&	GetCommunity();
	void			SetCommunity(const std::string& _community);

	RetValue		SetProperty(const std::string& _name, const std::string& _value);

	RetValue		SetProperties(const Device::Properties* _properties);
	RetValue 		SetProperties(Kompex::SQLiteStatement*	_statement);
	RetValue		SetProperties(const JSONNode&	_node);

	RetValue		GetEndpointValue(Endpoint* _endpoint);

	void			Show(std::ostream& _os);
protected:

	SNMPSession		snmp_session_;

	void 			Process();

	void			OnActivated();
	void			OnDeactivated();
};

#endif
