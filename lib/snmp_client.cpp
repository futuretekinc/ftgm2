#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <iostream>
#include "defined.h"
#include "trace.h"
#include "snmp_client.h"
#include "snmp_object.h"

using namespace std;

SNMPClient::SNMPClient()
:	name_("snmp_client")
{
	TRACE(this, "The snmp client[%s] has been created.", name_.c_str());
}

SNMPClient::SNMPClient
(
	const Properties& _properties
)
{
	name_ = _properties.name;
	TRACE(this, "The snmp client[%s] has been created.", name_.c_str());
}

SNMPClient::~SNMPClient()
{
}

void SNMPClient::PreProcess()
{
	init_snmp(name_.c_str());

}

void	SNMPClient::PostProcess()
{
}

void	SNMPClient::OnMessage(Message *_message)
{
	switch(_message->type)
	{
	default:
		MessageProcess::OnMessage(_message);
	}
}

RetValue	SNMPClient::Append
(
	SNMPSession* _session
)
{
	RetValue	ret_value = RET_VALUE_OK;

	map<const string, SNMPSession *>::iterator it = session_list_.find(_session->GetPeer());
	if (it != session_list_.end())
	{
		ret_value = RET_VALUE_OBJECT_EXISTS;
		ERROR(this, ret_value, "Peer already exist!");
		return	ret_value;	
	}

	session_list_[_session->GetPeer()] = _session;
	if (session_list_.size() > 0)
	{
		Start();			
	}

	return	ret_value;

}

RetValue	SNMPClient::Remove
(
	SNMPSession* _session
)
{
	RetValue ret_value = RET_VALUE_OK;

	if (session_list_.erase(_session->GetPeer()) == 0)
	{
		ret_value = RET_VALUE_OBJECT_NOT_FOUND;
		ERROR(this, ret_value, "");
	}

	if (session_list_.size() == 0)
	{
		Stop();			
	}

	return	ret_value;
}
