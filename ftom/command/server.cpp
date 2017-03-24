#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <map>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "tcp_server.h"

using namespace std;

RetValue	ShellCommandServer
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell* _shell
)
{
	RetValue	ret_value = RET_VALUE_OK;
	ObjectManager*	object_manager = (ObjectManager*)_shell->Data();
	TCPServer*		tcp_server = object_manager->GetTCPServer();

	if (_count ==1)
	{
		_shell->Out() << "[ TCP Server ]" << endl;
		_shell->Out() << setw(16) << "State" << " : " << (tcp_server->IsRun()?"Run":"Stop") << endl;
		_shell->Out() << setw(16) << "Session count" << " : " << tcp_server->GetSessionCount() << endl;
	}
	else if (IsCorrectOption(_arguments[1], "session_list"))
	{
		list<TCPSession::Information>	session_information_list;

		ret_value = tcp_server->GetSessionInformationList(session_information_list);
		if (ret_value == RET_VALUE_OK)
		{
			_shell->Out() << setw(16) << "IP Address" << setw(8) << "Port" << setw(24) << "Start Time" << setw(8) << "Timeout" << endl;
			for(auto it = session_information_list.begin(); it != session_information_list.end() ; it++)
			{
				_shell->Out() << setw(16) << inet_ntoa((*it).addr_info.sin_addr) << setw(8) << ntohs((*it).addr_info.sin_port) << setw(24) << (*it).start_time.ToString() << setw(8) << (*it).timeout << endl;
			}
		}
		else
		{
			_shell->Out() << "Error : Failed to get session list!" << endl;	
		}

	}
	else if (IsCorrectOption(_arguments[1], "stop"))
	{
	}

finished:
	return	ret_value;
}

Shell::Command	object_manager_command_server=
{
	.command	= "server",
	.help		= "server",
	.short_help	= "server",
	.function	= ShellCommandServer
};
