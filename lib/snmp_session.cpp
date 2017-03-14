#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <iostream>
#include "defined.h"
#include "trace.h"
#include "snmp_client.h"
#include "snmp_object.h"

using namespace std;

static SNMPClient	snmp_client_;

SNMPSession::SNMPSession()
{
	peer_ 		= "127.0.0.1";
	community_ 	= "public";
	timeout_ 	= 1000;
	sess_handle_= NULL;

	snmp_client_.Append(this);
}

SNMPSession::SNMPSession
(
	const Properties& _properties
)
{
	peer_ 		= _properties.peer;
	community_ 	= _properties.community;
	timeout_ 	= 1000;
	sess_handle_= NULL;

	snmp_client_.Append(this);
}

SNMPSession::~SNMPSession()
{
	Close();

	snmp_client_.Remove(this);
}

RetValue	SNMPSession::Open()
{
	RetValue	ret_value = RET_VALUE_OK;

	if (sess_handle_ == NULL)
	{
		snmp_session	session;

		snmp_sess_init(&session);

		session.version = SNMP_VERSION_2c;
		session.peername = new char[peer_.size() + 1];
		strcpy(session.peername, peer_.c_str());

		session.community = new u_char[community_.size()];
		memcpy(session.community, community_.c_str(), community_.size());
		session.community_len = community_.size();

		sess_handle_ = snmp_open(&session);

		delete [] session.peername;
		delete [] session.community;

		if (sess_handle_ == NULL)
		{
			ret_value = RET_VALUE_SNMP_SESSION_OPEN_FAILED;
			ERROR(this, ret_value, "Failed to open SNMP session.");	
		}
		else
		{
			TRACE(this, "The SNMP session[%s:%s] opened.", peer_.c_str(), community_.c_str());
		}
	}

	return	ret_value;
}

RetValue	SNMPSession::Close()
{
	if (sess_handle_ != NULL)
	{
		snmp_close(sess_handle_);	
	
		sess_handle_ = NULL;
	}

	return	RET_VALUE_OK;	
}

bool		SNMPSession::IsOpened()
{
	return	sess_handle_ != NULL;
}

RetValue	SNMPSession::Get(SNMPObject* _object)
{

	RetValue			ret_value = RET_VALUE_OK;
	netsnmp_pdu*	request_pdu;
	netsnmp_pdu*	response_pdu = NULL;

	if (sess_handle_ != NULL)
	{
		request_pdu = _object->CreateGetPDU(timeout_);
		if (request_pdu != NULL)
		{
			snmp_add_null_var(request_pdu, _object->GetOID().id, _object->GetOID().length);

			Lock();

			int ret = snmp_synch_response(sess_handle_, request_pdu, &response_pdu);
			if (ret == STAT_SUCCESS)
			{ 
				if (response_pdu != NULL)
				{
					if (response_pdu->errstat == SNMP_ERR_NOERROR)
					{
						ret_value = _object->SetValue(response_pdu->variables);
					}
					else
					{
						ERROR(this, ret_value, "SNMP repsponse error[%d].", response_pdu->errstat);
					}
				}
				else
				{
					ERROR(this, ret_value, "SNMP repsponse is NULL.");
				}
			}
			else
			{
				ret_value = RET_VALUE_SNMP_RESPONSE_ERROR;
				ERROR(this, ret_value, "");
			}

			if (response_pdu != NULL)
			{
				snmp_free_pdu(response_pdu);	
			}
			Unlock();
		}
		else
		{    
			ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;
			ERROR(this, ret_value, "");
		}    
	}
	else
	{
		ret_value = RET_VALUE_SNMP_SESSION_IS_NOT_CONNECTED;
		ERROR(this, ret_value, "");
	}

	return	ret_value;
}

RetValue	SNMPSession::Set(SNMPObject* _object)
{

	RetValue			ret_value = RET_VALUE_OK;
	netsnmp_pdu*	request_pdu;
	netsnmp_pdu*	response_pdu = NULL;

	if (sess_handle_ != NULL)
	{
		request_pdu = _object->CreateSetPDU(timeout_);
		if (request_pdu != NULL)
		{
			int ret = snmp_synch_response(sess_handle_, request_pdu, &response_pdu);
			if ((ret == STAT_SUCCESS) && (response_pdu->errstat == SNMP_ERR_NOERROR))
			{
				ret_value = RET_VALUE_OK;
			}
			else
			{
				ret_value = RET_VALUE_SNMP_RESPONSE_ERROR | response_pdu->errstat;	
				ERROR(this, ret_value, "");
			}

			if (response_pdu != NULL)
			{
				snmp_free_pdu(response_pdu);	
			}
		}
		else
		{    
			ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;
			ERROR(this, ret_value, "");
		}    
	}
	else
	{
		ret_value = RET_VALUE_SNMP_SESSION_IS_NOT_CONNECTED;
		ERROR(this, ret_value, "");
	}

	return	ret_value;
}
