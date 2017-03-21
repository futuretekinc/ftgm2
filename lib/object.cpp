#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>
#include <cstdarg>
#include <ctime>
#include "object.h"
#include "timer.h"

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
	if (class_name_.length() == 0)
	{
		uint32	i = 0;
		string	class_name = typeid(*this).name();
	
		for(i = 0 ; i < class_name.length() ; i++)
		{
			if (!isdigit(class_name[i]))
			{
				break;
			}
		}

		class_name_ = class_name.substr(i, class_name.length() - i);
	}

	return	class_name_;
}

bool	Object::Lock()
{
	return	locker_.Lock();	
}

bool	Object::Unlock()
{
	return	locker_.Unlock();
}

bool	Object::IsLocked()
{
	return	locker_.IsLocked();
}

void	Object::ReleaseParent()
{
	parent_ = NULL;
}
