#include "value.h"

using namespace	std;


Value::Value
(
	const Value& _value
)
{
	type_	= _value.type_;
	if (type_ == TYPE_INT)
	{
		number_value_.int_value = _value.number_value_.int_value;
	}
	else if (type_ == TYPE_FLOAT)
	{
		number_value_.float_value = _value.number_value_.float_value;
	}
	else
	{
		string_value_ = _value.string_value_;
	}
}

Value::Value
(
	int _value
)
{
	type_	= TYPE_INT;
	number_value_.int_value = _value;
}

Value::Value
(
	float _value
)
{
	type_	= TYPE_FLOAT;
	number_value_.float_value = _value;
}

Value::Value
(
	const std::string& _value
)
{
	type_	= TYPE_STRING;
	string_value_ = _value;
}

int		Value::ToInt()
{
	switch(type_)
	{
	case	TYPE_INT: 		return	number_value_.int_value;
	case	TYPE_FLOAT: 	return	(int)number_value_.float_value;
	case	TYPE_STRING:	return	atoi(string_value_.c_str());
	}

	return	0;
}

float	Value::ToFloat()
{
	switch(type_)
	{
	case	TYPE_INT: 		return	number_value_.int_value;
	case	TYPE_FLOAT: 	return	number_value_.float_value;
	case	TYPE_STRING:	return	strtof(string_value_.c_str(), NULL);
	}

	return	0;
}

const
std::string&	Value::ToString()
{
	switch(type_)
	{
	case	TYPE_INT: 		string_value_ = to_string(number_value_.int_value); break;
	case	TYPE_FLOAT: 	string_value_ = to_string(number_value_.float_value);
	default:
		break;
	}

	return	string_value_;
}

TimedValue::TimedValue
(
	const TimedValue& _value
)
:	Value(_value)
{
	time_ = _value.time_;
}

TimedValue::TimedValue
(
	int _value
)
{
	type_	= TYPE_INT;
	number_value_.int_value = _value;
	time_ 	= Time::GetCurrentTime();
}

TimedValue::TimedValue
(
	float _value
)
{
	type_	= TYPE_FLOAT;
	number_value_.float_value = _value;
	time_ 	= Time::GetCurrentTime();
}

TimedValue::TimedValue
(
	const std::string& _value
)
{
	type_	= TYPE_STRING;
	string_value_ = _value;
	time_ 	= Time::GetCurrentTime();
}
