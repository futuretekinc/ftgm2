#ifndef	OBJECT_H_
#define OBJECT_H_

#include "defined.h"
#include <iostream>
#include <cstdarg>
#include "locker.h"
#include <string>

class	Object
{
public:
	Object();
	virtual	~Object();

	virtual const std::string ClassName();

	virtual	bool	Lock();
	virtual	bool	Unlock();
	virtual	bool	IsLocked();

protected:
			void	SetParent(Object* _parent)	{	parent_ = _parent; }
			Object*	GetParent()					{	return	parent_;	}
	virtual	void	ReleaseParent();

	std::string	class_name_;
	Locker		locker_;
	bool		trace_on_;
	bool		error_on_;

	Object*		parent_;
};


#endif
