#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdarg>
#include <iomanip>
#include "trace.h"

using namespace std;

Debug	debug;

Debug::Debug()
:	trace_on_(false), error_on_(false)
{
	output_line_ = 0;
	output_ = &cout;
}

void	Debug::Trace
(
	bool _on
)
{
	trace_on_ = _on;
}

void	Debug::Error
(
	bool _on
)
{
	error_on_ = _on;
}

void	Debug::Error
(
	Object *_object,
	const std::string& _function, 
	int _line, 
	int _code, 
	const char* _format,
	...
)
{
	va_list	variable_list;
	time_t          current_time;
	char			output_buffer[2048];
	char			time_string[32];
	ofstream		out;
	ostringstream	buffer;

	if (error_on_)
	{
		out.open("trace.log", ofstream::out | ofstream::app);

		va_start(variable_list, _format);

		current_time = time(NULL);

		out << setw(4) << ++output_line_ << " : ";
		strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
		out << "[" << time_string << "]";
		out << "[ERROR]";
		if (_object != NULL)
		{
			buffer << _object->ClassName() << "." << _function;
		}
		else
		{
			buffer <<  _function;
		}
		out << "[" << setw(20) << buffer.str().substr(0, 20) << "]";
		out << "[" << setw(4) << _line << "]";
		out << "[" << setbase(16) << setw(4) << setfill('0') << _code << "] ";

		vsnprintf( output_buffer, sizeof(output_buffer), _format, variable_list);

		out << output_buffer << endl;;

		va_end(variable_list);

		out.close();
	}
}

void Debug::Trace
(
	Object *_object,
	const std::string& _function, 
	int _line, 
	const char* _format,
	...
)
{
	va_list	variable_list;
	time_t          current_time;
	char			output_buffer[2048];
	char			time_string[32];
	ofstream		out;
	ostringstream	buffer;

	if (trace_on_)
	{
		va_start(variable_list, _format);

		out.open("trace.log", ofstream::out | ofstream::app);

		current_time = time(NULL);

		out << setw(4) << ++output_line_ << " : ";
		strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
		out << "[" << time_string << "]";
		out << "[TRACE]";
		if (_object != NULL)
		{
			buffer << _object->ClassName() << "." << _function;
		}
		else
		{
			buffer <<  _function;
		}
		out << "[" << setw(20) << buffer.str().substr(0, 20) << "]";
		out << "[" << setw(4) << _line << "]";
		out << "[####] ";

		vsnprintf( output_buffer, sizeof(output_buffer), _format, variable_list);

		out << output_buffer << endl;;

		va_end(variable_list);

		out.close();
	}
}
