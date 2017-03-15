#ifndef	SHELL_COMMAND_H_
#define	SHELL_COMMAND_H_

#include <string>
#include "object_manager.h"
#include "data_manager.h"

template <typename T>
struct ShellCommand
{
	std::string	command;
	std::string	short_help;
	std::string	help;	

	RetValue	(*function)(std::string _arguments[], uint32_t _count, Shell<T>* _shell);

	ShellCommand(const ShellCommand<T>& _command)
	{
		command = _command.command;
		help = _command.help;
		short_help = _command.short_help;
		function = _command.function;
	};

	ShellCommand(const std::string&  _command, const std::string&  _help, const std::string& _short_help, RetValue	(*_function)(std::string [], uint32_t, Shell<T>*))
	{
		command = _command;
		help = _help;
		short_help = _short_help;
		function = _function;
	};
};


#endif
