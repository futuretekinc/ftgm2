#ifndef	TRACE_H_
#define	TRACE_H_

#include "defined.h"
#include "object.h"
#include <string>
#include <ostream>

class	Debug
{
public:
	Debug();

	bool	Trace()	{	return	trace_on_;	};
	bool	Error()	{	return	error_on_;	};

	void	Trace(bool _on);
	void	Error(bool _on);

	void	Trace(Object *object, const std::string& function_, int line_, const char* format, ...);
	void	Error(Object *object, const std::string& function_, int line_, int code_, const char* format, ...);

protected:
	int				output_line_;
	std::ostream*	output_;
	bool			trace_on_;
	bool			error_on_;
};

extern	Debug	debug;

#define	TRACE_ON()						{	debug.Trace(true);	}
#define	TRACE_OFF()						{	debug.Trace(false);	}
#define	TRACE(object, format, ...)		{	debug.Trace(object, __func__, __LINE__, format, ## __VA_ARGS__); }
#define	TRACE_ENTRY(object)				{	debug.Trace(object, __func__, __LINE__, "entry"); }
#define	TRACE_EXIT(object)				{	debug.Trace(object, __func__, __LINE__, "exit") ; }

#define	ERROR_ON()						{	debug.Error(true);	}
#define	ERROR_OFF()						{	debug.Error(false);	}
#define	ERROR(object, code, format, ...){	debug.Error(object, __func__, __LINE__, code, format, ## __VA_ARGS__); }

#endif
