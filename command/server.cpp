#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <map>
#include "shell.h"
#include "shell_command.h"
#include "object_manager.h"
#include "data_manager.h"
#include "tcp_server.h"

using namespace std;

map<uint16_t, TCPServer *>	server_map_;

RetValue	ShellCommandServer
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

	if (IsCorrectOption(_arguments[1], "start"))
	{
		if (_count < 3)
		{
			_shell->Out() << "Error : Invalid arguments" << endl;	
			ret_value = RET_VALUE_INVALID_ARGUMENTS;
			goto finished;
		}

		uint16_t	port = strtoul(_arguments[2].c_str(), 0, 10);

		TCPServer*	server = new TCPServer(port);
		if (server == NULL)
		{
			_shell->Out() << "Error : Failed to create server[" << port << "].";	
			goto finished;
		}

		server_map_[port] = server;

		server->Start();
	}
	else if (IsCorrectOption(_arguments[1], "stop"))
	{
		if (_count < 3)
		{
			_shell->Out() << "Error : Invalid arguments" << endl;	
			ret_value = RET_VALUE_INVALID_ARGUMENTS;
			goto finished;
		}

		uint16_t	port = strtoul(_arguments[2].c_str(), 0, 10);

		auto it = server_map_.find(port);

		if (it == server_map_.end())
		{
			_shell->Out() << "Error : Server[" << port << "] not found[";
			goto finished;
		}

		it->second->Stop();

		delete it->second;

		server_map_.erase(it);
	}

finished:
	return	ret_value;
}

ShellCommand<ObjectManager>	object_manager_command_server=
{
	.command	= "server",
	.help		= "server",
	.short_help	= "server",
	.function	= ShellCommandServer
};
