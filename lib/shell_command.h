#ifndef	SHELL_COMMAND_H_
#define	SHELL_COMMAND_H_

#include <string>
#include "object_manager.h"
#include "data_manager.h"

template <typename T1, typename T2>
struct ShellCommand
{
	std::string	command;
	std::string	short_help;
	std::string	help;	

	RetValue	(*function)(std::string _arguments[], uint32_t _count, Shell<T1, T2>* _shell);

	ShellCommand(const ShellCommand<T1, T2>& _command)
	{
		command = _command.command;
		help = _command.help;
		short_help = _command.short_help;
		function = _command.function;
	};

	ShellCommand(const std::string&  _command, const std::string&  _help, const std::string& _short_help, RetValue	(*_function)(std::string [], uint32_t, Shell<T1, T2>*))
	{
		command = _command;
		help = _help;
		short_help = _short_help;
		function = _function;
	};
};

extern	ShellCommand<ObjectManager, DataManager>	object_manager_shell_commands[];
extern	int							object_manager_shell_command_count;

#endif
