#ifndef	VALUE_H_
#define	VALUE_H_

#include <string>
#include "timer.h"

class	Value
{
public:
	enum	Type
	{
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_STRING
	};

			Value(int _value = 0);
			Value(float _value);
			Value(const std::string& _value);
			Value(const Value& _value);

	Type	GetType()	{	return	type_;	}

	int				ToInt();
	float			ToFloat();
	const
	std::string&	ToString();

protected:
	Type		type_;	
	union
	{
		int		int_value;
		float	float_value;
	}	number_value_;
	std::string	string_value_;
};

class	TimedValue : public Value
{
public:
			TimedValue(int _value = 0);
			TimedValue(float _value);
			TimedValue(const std::string& _value);
			TimedValue(const TimedValue& _value);

			Time&	GetTime()	{	return	time_;	}
protected:
	Time	time_;
};
#endif
