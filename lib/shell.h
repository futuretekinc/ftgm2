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

template <typename T>	class	ShellCommand;

template <typename T>
class	Shell
{
public:
	Shell(ShellCommand<T>* _commands, int 	_command_count, T* _data)
	{
		out_	= &std::cout;
		data_ 	= _data;
		thread_ = NULL;
		stop_ 	= true;
		sync_   = false;
		prompt_	= "shell";

		for(int i = 0 ; i < _command_count ; i++)
		{
			typename std::map<const std::string, ShellCommand<T> *>::iterator it = command_map_.find(_commands[i].command);
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
		typename std::map<const std::string, ShellCommand<T> *>::iterator	it = command_map_.begin();
		for(;it != command_map_.end() ; it++)
		{
		}
	}

	std::ostream&	Out()	{	return	*out_; }

	T*	Data()	{	return	data_;	}

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

		sync_ = _sync;
		if (sync_)
		{
			thread_->join();	

			thread_ = NULL;
		}

		return	RET_VALUE_OK;
	}

	RetValue	Stop()
	{
		stop_ = true;

		return	RET_VALUE_OK;
	}


	void	Add(ShellCommand<T>* _commands, int 	_command_count)
	{
		thread_ = NULL;
		stop_ 	= true;

		for(int i = 0 ; i < _command_count ; i++)
		{
			typename std::map<const std::string, ShellCommand<T> *>::iterator it = command_map_.find(_commands[i].command);
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
	T*				data_;
	std::thread*	thread_;
	bool			stop_;
	bool			sync_;
	std::string		prompt_;
	std::map<const std::string, ShellCommand<T>*>	command_map_;
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

			count = Shell<T>::Parser(command_line, arguments, 16);
			if (count != 0)
			{
				typename std::map<const std::string, ShellCommand<T>*>::iterator it = _shell->command_map_.find(arguments[0])	;
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

		std::cout << "finished." << std::endl;
	}
	
};

inline bool	IsCorrectOption(const std::string& _argument, const char *_option)
{
	return	(strcasecmp(_argument.c_str(), _option) == 0);
}
#endif
