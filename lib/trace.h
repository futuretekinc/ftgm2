#ifndef	TRACE_H_
#define	TRACE_H_

#include "defined.h"
#include "object.h"
#include <string>
#include <ostream>
#include <sstream>
#include <mutex>
#include <map>

class	Trace
{
public:
	enum	Level
	{
		OFF=0,
		INFO,
		WARN,
		ERROR,
		CRITICAL,
		FATAL
	};

	struct	Properties
	{
		std::string	name;
		Level		level;
	};

	Trace();

	RetValue	SetLevel(Object *object, Level _level);
	RetValue	SetLevel(const std::string& _name, Level _level);
	RetValue	SetLevel(const char *_name, Level _level);
	RetValue	SetLevel(const std::string& _name, const std::string& _level);
	RetValue	SetLevel(const char * _name, const char *_level);
	static const
	std::string	GetLevelString(Level _level);
	static 
	Level		ToLevel(const std::string& _level);

	Properties*	GetProperties(const std::string& _name);
	Properties*	GetPropertiesAt(uint32 index);

	void		PushIndent()	{	indent_++;	}
	void		PopIndent()		{	if (indent_ > 0) indent_--;	}
	void		ResetIndent()	{	indent_ = 0; }

	void		Print(Object *object, Level _level, const std::string& _function, uint32_t _line, RetValue _code, const char* _format, ...);

protected:
	void	Output(Level		_level, std::string _name, std::string _function, uint32_t _line, RetValue _code, const char* _message);

	int				output_line_;
	std::string		file_name_;
	uint32_t		indent_;
	std::map<std::string, Properties *>	class_map_;
};

extern	Trace	_trace;

#define	ASSERT(x)						{}
#define	TRACE_SET_LEVEL(name, level)	{	_trace.SetLevel(name, level);	}
#define	INFO(object, format, ...)		{	_trace.Print(object, 	Trace::Level::INFO,	__func__, __LINE__, 0, 	format, ## __VA_ARGS__); }
#define	ERROR(object, code, format, ...){	_trace.Print(object,	Trace::Level::ERROR,__func__, __LINE__, code,format, ## __VA_ARGS__); }
#define	TRACE_PUSH()					{	_trace.PushIndent();	}
#define	TRACE_POP()						{	_trace.PopIndent();	}
#define	TRACE_RESET()					{	_trace.ResetIndent();	}
#endif
