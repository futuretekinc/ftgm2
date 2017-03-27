#ifndef	VALUE_H_
#define	VALUE_H_

#include <string>
#include <list>
#include "timer.h"

class	Value
{
public:
	enum	Type
	{
		UNDEFINED,
		BOOL,
		INT,
		UINT32,
		FLOAT,
		STRING
	};

					Value(Type _type = INT);
					Value(bool _value);
					Value(int	_value);
					Value(uint32_t _value);
					Value(float _value);
					Value(const std::string& _value);
					Value(const Value& _value);

			Type	GetType()	{ return type_; };
	const	Value&	operator =(const Value& _value);

	virtual bool	Set(bool _value);
	virtual bool	Set(int _value);
	virtual bool	Set(uint32_t _value);
	virtual bool	Set(float _value);
	virtual bool	Set(const std::string& _value);

	virtual bool	Get(bool& _value);
	virtual bool	Get(int& _value);
	virtual bool	Get(uint32_t& _value);
	virtual bool	Get(float& _value);
	virtual bool	Get(std::string& _value);

			Time&	GetTime()	{	return	time_;	};

	const	std::string&	ToString();
protected:
	Type	type_;	
	Time	time_;
	union
	{
		bool		bool_;
		int			int_;
		uint32_t	uint32_;
		float		float_;
	}	value_;
	std::string	value_string_;
};

#endif
