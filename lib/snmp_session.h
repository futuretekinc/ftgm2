#ifndef	SNMP_SESSION_H_
#define	SNMP_SESSION_H_

#include "object.h"
#include "message_process.h"

class	SNMPObject;

class	SNMPSession  : public Object
{
public:
	struct	Properties
	{
		std::string	peer;
		std::string	community;
	};

	SNMPSession();
	SNMPSession(const Properties& _properties);
	~SNMPSession();

	const 
	std::string&	GetPeer()		{	return	peer_;	};
	const		
	void			SetPeer(const std::string& _peer)		{	peer_ = _peer;	};
	const 
	std::string&	GetCommunity()	{	return	community_;	};
	const
	void			SetCommunity(const std::string& _community)	{	community_ = _community; }
	int				Timeout()	{	return	timeout_; };

	RetValue		Open();
	RetValue		Close();
	bool			IsOpened();

	RetValue		Get(SNMPObject* _object);
	RetValue		Set(SNMPObject* _object);

private:
	std::string		peer_;
	std::string 	community_;
	int				timeout_;
	
	struct snmp_session* 	sess_handle_;
};
#endif
