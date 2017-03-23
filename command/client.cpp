#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <map>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "tcp_client.h"

using namespace std;

TCPClient	*client = NULL;

RetValue	ShellCommandClient
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager>* _shell
)
{
	RetValue	ret_value = RET_VALUE_OK;

	if (_count < 2)
	{
		_shell->Out() << "Error : Invalid arguments" << endl;	
		ret_value = RET_VALUE_INVALID_ARGUMENTS;
		goto finished;
	}

	if (IsCorrectOption(_arguments[1], "connect"))
	{
		string	server_ip = "127.0.0.1";
		uint16_t	port;

		if (_count < 3)
		{
			_shell->Out() << "Error : Invalid arguments" << endl;	
			ret_value = RET_VALUE_INVALID_ARGUMENTS;
			goto finished;
		}

		if (_count == 3)
		{
			port = strtoul(_arguments[2].c_str(), 0, 10);
		}
		else if (_count == 4)
		{
			server_ip = _arguments[2];	
			port = strtoul(_arguments[3].c_str(), 0, 10);
		}

		if (client == NULL)
		{
			client = new TCPClient(server_ip, port);
			if (client == NULL)
			{
				_shell->Out() << "Error : Failed to create client[" << port << "].";	
				goto finished;
			}

			ret_value  = client->Connect();
			if (ret_value == RET_VALUE_OK)
			{
				client->Start();
			}
		}
	}
	else if (IsCorrectOption(_arguments[1], "disconnect"))
	{
		if (client != NULL)
		{
			delete client;	
			client = NULL;	
		
		}
	}
	else if (IsCorrectOption(_arguments[1], "send"))
	{
		if (_count < 3)
		{
			_shell->Out() << "Error : Invalid arguments" << endl;	
			ret_value = RET_VALUE_INVALID_ARGUMENTS;
			goto finished;
		}

		if (client == NULL)
		{
			_shell->Out() << "Client not connected" << endl;	
		}
		else
		{
			client->Send(_arguments[2].c_str(), _arguments[2].length());	
		}
	}
	else if (IsCorrectOption(_arguments[1], "receive"))
	{
		_shell->Out() << "Recieved packet count : " << client->GetReceivedPacketCount() << endl;	

		while(client->GetReceivedPacketCount())
		{
			uint8_t		buffer[4096];
			uint32_t	received_packet_len;

			received_packet_len = client->Receive(buffer, sizeof(buffer));	

			_shell->Out() << "Packet Len : " << received_packet_len;
		
		}
			
	}

finished:
	return	ret_value;
}

ShellCommand<ObjectManager>	object_manager_command_client=
{
	.command	= "client",
	.help		= "client",
	.short_help	= "client",
	.function	= ShellCommandClient
};
