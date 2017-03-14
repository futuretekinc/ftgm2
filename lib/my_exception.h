#ifndef	ERROR_H_
#define	ERROR_H_
#include <exception>
#include "defined.h"

class	Exception : public exception
{
public:
	Exception(RetValue _ret_vlaue, std::string _message)
	{
		ret_value_ = _ret_value;
		message_ = _message;	
	}

	virtual	const char* what() const throw()
	{
		return	message_.c_str();
	}

private:
	std::string	message_;
	RetValue	ret_value_;
};

#endif
