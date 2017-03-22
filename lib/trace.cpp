#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdarg>
#include <cstring>
#include <iomanip>
#include "trace.h"

using namespace std;

struct	TraceLevelString
{
	string			name;
	Trace::Level	level;
}	
_level_strings[] =
{
	{	.name = "info",			.level = Trace::Level::INFO	},
	{	.name = "information",	.level = Trace::Level::INFO	},
	{	.name = "warn",			.level = Trace::Level::WARN	},
	{	.name = "warning",		.level = Trace::Level::WARN	},
	{	.name = "erro",			.level = Trace::Level::ERROR	},
	{	.name = "error",		.level = Trace::Level::ERROR	},
	{	.name = "crit",			.level = Trace::Level::CRITICAL},
	{	.name = "critical",		.level = Trace::Level::CRITICAL},
	{	.name = "fatl",			.level = Trace::Level::FATAL	},
	{	.name = "fatal",		.level = Trace::Level::FATAL	}
};

Trace	_trace;

Trace::Trace()
{
	output_line_= 0;
	file_name_  = "trace.log";
	indent_		= 0;
}

const
std::string	Trace::GetLevelString
(
	Level _level
)
{
	uint32	i;

	for(i = 0 ; i < sizeof(_level_strings) / sizeof(TraceLevelString) ; i++)
	{
		if (_level_strings[i].level == _level)
		{
			return	_level_strings[i].name;	
		}
	}

	return	"off";	
}

Trace::Level	Trace::ToLevel
(
	const std::string& _level
)
{
	uint32	i;

	for(i = 0 ; i < sizeof(_level_strings) / sizeof(TraceLevelString) ; i++)
	{
		if (_level_strings[i].name == _level)
		{
			return	_level_strings[i].level;	
		}
	}

	return	OFF;	
}

void	Trace::Print
(
	Object *	_object,
	Level 		_level,
	const std::string& _function, 
	uint32_t	_line, 
	RetValue	_code, 
	const char* _format,
	...
)
{
	va_list			variable_list;
	char			output_buffer[2048];
	Properties*		properties = NULL;
	string			class_name;

	if (_object != NULL)
	{
		class_name = _object->ClassName();
	}
	else
	{
		class_name = "global";	
	}

	map<string, Properties *>::iterator it = class_map_.find(class_name);
	if (it == class_map_.end())
	{
		try
		{
			properties = new Properties;
			properties->name	= class_name;
			properties->level 	= OFF;

			class_map_[class_name] = properties	;
		}
		catch(std::bad_alloc& e)
		{
			cerr << "Exception bad alloc!" << endl;
			return;
		}
		catch(std::exception& e)
		{
			cerr << "Exception occurred!" << endl;
			return;
		}
	}
	else
	{
		properties = it->second;	
	}

//	if (properties->level >= _level)
	{
		va_start(variable_list, _format);
		vsnprintf( output_buffer, sizeof(output_buffer), _format, variable_list);
		va_end(variable_list);

		Output(_level, class_name, _function, _line, _code, output_buffer);
	}
}

RetValue	Trace::SetLevel
(
	const char *_name,
	Level	_level
)
{
	map<string, Properties *>::iterator it = class_map_.find(_name);
	if (it == class_map_.end())
	{
		try
		{
			Properties *properties;
			properties = new Properties;
			properties->name = _name;
			properties->level = _level;

			class_map_[_name] = properties	;
		}
		catch(std::bad_alloc& e)
		{
			cerr << "Exception bad alloc!" << endl;
			return RET_VALUE_NOT_ENOUGH_MEMORY;
		}
		catch(std::exception& e)
		{
			cerr << "Exception occurred!" << endl;
			return RET_VALUE_ERROR;
		}
	}
	else
	{
		it->second->level = _level;	
	}

	return	RET_VALUE_OK;
}

RetValue	Trace::SetLevel
(
	const string&	_name,
	Level		_level
)
{
	return	SetLevel(_name.c_str(), _level);
}

RetValue	Trace::SetLevel
(
	const char *	_name,
	const char * 	_level
)
{
	return	SetLevel(_name, ToLevel(_level));
}

RetValue	Trace::SetLevel
(
	const string&	_name,
	const string& _level
)
{
	return	SetLevel(_name.c_str(), ToLevel(_level));
}

RetValue	Trace::SetLevel
(
	Object*	_object,
	Level	_level
)
{
	return	SetLevel(_object->ClassName(), _level);
}

void	Trace::Output
(
	Level		_level,
	std::string _name,
	std::string _function,
	uint32		_line,
	RetValue	_ret_value,
	const char*	_message
)
{
	time_t	current_time;
	char	time_string[32];
	char	buffer[1024];
	ostringstream	field_function;
	ostringstream	header;
	istringstream	message(_message);
	ofstream	out;

	memset(buffer, 0, sizeof(buffer));

	current_time = time(NULL);
	strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", localtime(&current_time));

	field_function << _name << "." << _function;


	header << "[" << GetLevelString(_level) << "]";
	header << "[" << time_string << "]";
	header << "[" << setw(32) << field_function.str().substr(0, 32) << "]";
	header << "[" << setw(4) << _line << "]";
	header << "[" << setbase(16) << setw(4) << setfill(' ') << _ret_value << "] ";
	if (indent_ != 0)
	{
		header << setw(indent_*4) << setfill(' ') << "";
	}

	out.open(file_name_, ofstream::out | ofstream::app);

	while(!message.eof())
	{
		message.getline(buffer, sizeof(buffer) - 1);
		out << setw(4) << ++output_line_ << " : " << header.str() << buffer << endl;
	}

	out.close();
}

Trace::Properties*	Trace::GetProperties
(
	const std::string& _name
)
{
	map<string, Properties *>::iterator it = class_map_.find(_name);
	if (it != class_map_.end())
	{
		return	it->second;
	}

	return	NULL;
}

Trace::Properties* Trace::GetPropertiesAt
(
	uint32 index
)
{
	map<string, Properties *>::iterator it = class_map_.begin();
	while(it != class_map_.end())
	{
		if (index == 0)
		{
			return	it->second;
		}

		index--;
		it++;
	}

	return	NULL;
}
