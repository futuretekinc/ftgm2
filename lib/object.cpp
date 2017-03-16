#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>
#include <cstdarg>
#include <ctime>
#include "object.h"

using namespace std;

Object::Object()
: trace_on_(false), error_on_(false)
{
}

Object::~Object()
{
}

const string Object::ClassName()
{
	return	typeid(*this).name();
}

bool	Object::Lock()
{
	return	mutex_.Lock();	
}

bool	Object::Unlock()
{
	return	mutex_.Unlock();
}

bool	Object::IsLocked()
{
	return	mutex_.IsLocked();
}

void	Object::Trace
(
	bool	_on
)
{
	trace_on_ = _on;
}

void	Object::Error
(
	bool	_on
)
{
	error_on_ = _on;	
}

void	Object::Error
(
	ostream& out,
	int	_index,
	const std::string& _function, 
	int _line, 
	int _code, 
	const char* _format,
	va_list	_variable_list
)
{
	time_t          current_time;
	char			output_buffer[2048];
	char			time_string[32];
	ostringstream	buffer;

	if (error_on_)
	{
		current_time = time(NULL);

		out << setw(4) << _index << " : ";
		strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
		out << "[" << time_string << "]";
		out << "[ERROR]";
		buffer << ClassName() << "." << _function;
		out << "[" << setw(20) << buffer.str().substr(0, 20) << "]";
		out << "[" << setw(4) << _line << "]";
		out << "[" << setw(8) << _code << "] ";

		vsnprintf( output_buffer, sizeof(output_buffer), _format, _variable_list);

		out << output_buffer << endl;
	}
}

void Object::Trace
(
 ostream& out,
	int	_index,
	const std::string& _function, 
	int _line, 
	const char* _format,
	va_list _variable_list
)
{
	time_t          current_time;
	char			output_buffer[2048];
	char			time_string[32];
	ostringstream	buffer;

	if (trace_on_)
	{
		current_time = time(NULL);

		out << setw(4) << _index << " : ";
		strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
		out << "[" << time_string << "]";
		out << "[TRACE]";
		buffer << ClassName() << "." << _function << ":" << _line;
		out << "[" << setw(20) << buffer.str().substr(0, 20) << "]";
		out << "[" << setw(4) << _line << "] ";

		vsnprintf( output_buffer, sizeof(output_buffer), _format, _variable_list);

		out << output_buffer << endl;
	}
}

void	Object::ReleaseParent()
{
	parent_ = NULL;
}
