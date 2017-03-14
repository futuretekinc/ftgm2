#ifndef	OBJECT_H_
#define OBJECT_H_

#include "defined.h"
#include <iostream>
#include <cstdarg>
#include "mutex.h"

class	Object
{
public:
	Object();
	virtual	~Object();

	virtual const std::string ClassName();

	virtual	bool	Lock();
	virtual	bool	Unlock();
	virtual	bool	IsLocked();

	virtual	void	Error(std::ostream& out, int index, const std::string& _function, int _line, int _code,  const char* _format, va_list _variable_list);
	virtual	void	Trace(std::ostream& out, int index, const std::string& _function, int _line, const char* _format, va_list _variable_list);

	virtual	void	Error(bool on);
	virtual	void	Trace(bool on);

protected:
	Mutex	mutex_;
	bool	trace_on_;
	bool	error_on_;
};


#endif
