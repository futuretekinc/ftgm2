#ifndef	SNMP_CLIENT_H_
#define	SNMP_CLIENT_H_

#include <map>
#include "message_process.h"
#include "snmp_session.h"

class	SNMPSession;

class	SNMPClient : public MessageProcess
{
public:
	struct	Properties
	{
		std::string	name;
	};

	SNMPClient();
	SNMPClient(const Properties& _properties);
	~SNMPClient();

	const 
	std::string&	Name()		{	return	name_;	};

	RetValue		Append(SNMPSession *_session);
	RetValue		Remove(SNMPSession* _session);

protected:
	virtual void	PreProcess();
	virtual	void	PostProcess();

	virtual	void	OnMessage(Message *_message);

	std::string	name_;
	std::map<const std::string, SNMPSession *>	session_list_;
};
#endif
