#ifndef	SHELL_H_
#define	SHELL_H_

#include "defined.h"
#include <string>
#include <map>
#include <thread>
#include <unistd.h>
#include "trace.h"
#include "object_manager.h"
#include <iostream>

template <typename T1, typename T2>	class	ShellCommand;

template <typename T1, typename T2>
class	Shell
{
public:
	Shell(ShellCommand<T1, T2>* _commands, int 	_command_count, T1* _data1, T2* _data2)
	{
		out_	= &std::cout;
		data1_ 	= _data1;
		data2_ 	= _data2;
		thread_ = NULL;
		stop_ 	= true;

		for(int i = 0 ; i < _command_count ; i++)
		{
			typename std::map<const std::string, ShellCommand<T1, T2> *>::iterator it = command_map_.find(_commands[i].command);
			if (it == command_map_.end())
			{
				TRACE(NULL, "Add command[%s]", _commands[i].command.c_str());
				command_map_[_commands[i].command] = &_commands[i];	
			}
			else
			{
				TRACE(NULL, "Already exist command[%s]", _commands[i].command.c_str());
			}
		}
	}

	~Shell()
	{
		typename std::map<const std::string, ShellCommand<T1, T2> *>::iterator	it = command_map_.begin();
		for(;it != command_map_.end() ; it++)
		{
			delete it->second;
		}
	}

	std::ostream&	Out()	{	return	*out_; }

	T1*	Data1()	{	return	data1_;	}
	T2*	Data2()	{	return	data2_;	}

	RetValue	Start(bool _sync = false)
	{
		if (thread_ != NULL)
		{
			return	RET_VALUE_OK;	
		}

		thread_ = new std::thread(Process, this);

		while(stop_)
		{
			usleep(1000);		
		}

		if (_sync)
		{
			thread_->join();	

			delete thread_;

			thread_ = NULL;
		}

		return	RET_VALUE_OK;
	}

	RetValue	Stop()
	{
		if (thread_ != NULL)
		{
			delete thread_;

			thread_ = NULL;
		}

		return	RET_VALUE_OK;
	}


	void	Add(ShellCommand<T1, T2>* _commands, int 	_command_count)
	{
		thread_ = NULL;
		stop_ 	= true;

		for(int i = 0 ; i < _command_count ; i++)
		{
			typename std::map<const std::string, ShellCommand<T1, T2> *>::iterator it = command_map_.find(_commands[i].command);
			if (it == command_map_.end())
			{
				TRACE(NULL, "Add command[%s]", _commands[i].command.c_str());
				command_map_[_commands[i].command] = &_commands[i];	
			}
			else
			{
				TRACE(NULL, "Already exist command[%s]", _commands[i].command.c_str());
			}
		}
	}

protected:
	T1*				data1_;
	T2*				data2_;
	std::thread*	thread_;
	bool			stop_;
	std::string		prompt_;
	std::map<const std::string, ShellCommand<T1, T2>*>	command_map_;
	std::ostream	*out_;

	static
	int		Parser
	(
		const std::string& _command_line, 
		std::string* _arguments, 
		int _max_count
	)
	{
		int	count = 0;
		
		if (_command_line.length() != 0)
		{
			char *word = NULL;
			const char *seperators = "\t \n\r";
			char *buffer = new char[_command_line.length() + 1];

			strcpy(buffer, _command_line.c_str());
				
			word = strtok(buffer, seperators);
			while((word != NULL) && (count < _max_count))
			{
				_arguments[count++] = word;
				word = strtok(NULL, seperators);
			}

			delete [] buffer;
		}

		return	count;
	}

	static
	void 	Process(Shell *_shell)
	{
		std::string	command_line;
		std::string	arguments[16];
		int		count;

		_shell->stop_	= false;

		while(!_shell->stop_)
		{
			std::cout << _shell->prompt_ << "> ";
			std::getline(std::cin, command_line);
			count = 0;

			count = Shell<T1, T2>::Parser(command_line, arguments, 16);
			if (count != 0)
			{
				typename std::map<const std::string, ShellCommand<T1, T2>*>::iterator it = _shell->command_map_.find(arguments[0])	;
				if (it != _shell->command_map_.end())
				{
					it->second->function(arguments, count, _shell);
				}
				else
				{
					std::cout << "Command not found : " << arguments[0] << std::endl;	
				}
			}
		}
	}
	
};

inline bool	IsCorrectOption(const std::string& _argument, const char *_option)
{
	return	(strcasecmp(_argument.c_str(), _option) == 0);
}
#endif
