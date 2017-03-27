#include <sstream>
#include "value.h"

using namespace	std;


Value::Value
(
	const Value& _value
)
{
	type_ = _value.type_;
	time_ = _value.time_;
	if(type_ == FLOAT)
	{
		value_.float_ = _value.value_.float_;	
	}
	else
	{
		value_.uint32_ = _value.value_.uint32_;	
	}
	value_string_ = _value.value_string_;
}

Value::Value
(
	Type	_type
)
{
	type_	= type_;
	time_ 	= Time::GetCurrentTime();
	if(_type == FLOAT)
	{
		value_.float_ = 0;	
	}
	else
	{
		value_.uint32_ = 0;	
	}
	value_string_= "";
}

Value::Value
(
	bool _value
)
{
	type_ = BOOL;
	value_.bool_ = _value;
}

Value::Value
(
	int _value
)
{
	type_ = INT;
	value_.int_ = _value;
}

Value::Value
(
	uint32_t	_value
)
{
	type_ = UINT32;
	value_.uint32_ = _value;
}

Value::Value
(
	float _value
)
{
	type_ = FLOAT;
	value_.float_ = _value;
}

Value::Value
(
	const string& _value
)
{
	type_ = STRING;
	value_string_ = _value;
}

bool	Value::Set
(
	bool _value
)
{
	switch(type_)
	{
	case	UNDEFINED:
		{
			type_ = BOOL;
			value_.bool_ = _value;
		}
		break;

	case	BOOL:
		{
			value_.bool_ = _value;
		}
		break;

	case	INT:
		{
			value_.int_ = _value;
		}
		break;

	case	UINT32:
		{
			value_.uint32_ = _value;
		}
		break;
	
	case	FLOAT:
		{
			value_.float_ = _value;
		}
		break;

	case	STRING:
		{
			ostringstream	oss;

			oss << _value;

			value_string_ = oss.str();
		}
		break;

	default:
		return	false;
	}

	return	true;
}

bool	Value::Set
(
	int _value
)
{
	switch(type_)
	{
	case	UNDEFINED:
		{
			type_ = INT;
			value_.int_ = _value;
		}
		break;

	case	BOOL:
		{
			value_.bool_ = (_value != 0);
		}
		break;

	case	INT:
		{
			value_.int_ = _value;
		}
		break;

	case	UINT32:
		{
			value_.uint32_ = _value;
		}
		break;
	
	case	FLOAT:
		{
			value_.float_ = _value;
		}
		break;

	case	STRING:
		{
			ostringstream	oss;

			oss << _value;

			value_string_ = oss.str();
		}
		break;

	default:
		return	false;
	}

	return	true;
}

bool	Value::Set
(
	uint32_t _value
)
{
	switch(type_)
	{
	case	UNDEFINED:
		{
			type_ = UINT32;
			value_.uint32_ = _value;
		}
		break;

	case	BOOL:
		{
			value_.bool_ = (_value != 0);
		}
		break;

	case	INT:
		{
			value_.int_ = _value;
		}
		break;

	case	UINT32:
		{
			value_.uint32_ = _value;
		}
		break;
	
	case	FLOAT:
		{
			value_.float_ = _value;
		}
		break;

	case	STRING:
		{
			ostringstream	oss;

			oss << _value;

			value_string_ = oss.str();
		}
		break;

	default:
		return	false;
	}

	return	true;
}

bool	Value::Set
(
	float _value
)
{
	switch(type_)
	{
	case	UNDEFINED:
		{
			type_ = FLOAT;
			value_.float_ = _value;
		}
		break;

	case	BOOL:
		{
			value_.bool_ = (_value != 0);
		}
		break;

	case	INT:
		{
			value_.int_ = _value;
		}
		break;

	case	UINT32:
		{
			value_.uint32_ = _value;
		}
		break;
	
	case	FLOAT:
		{
			value_.float_ = _value;
		}
		break;

	case	STRING:
		{
			ostringstream	oss;

			oss << _value;

			value_string_ = oss.str();
		}
		break;

	default:
		return	false;
	}

	return	true;
}

bool	Value::Set
(
	const std::string& _value
)
{
	switch(type_)
	{
	case	UNDEFINED:
		{
			ostringstream	oss;

			oss << _value;

			type_ = STRING ;
			value_string_ = oss.str();
		}
		break;

	case	BOOL:
		{
			if ((_value.compare("0") == 0) || (_value.compare("FALSE") == 0) || (_value.compare("false") == 0))
			{
				value_.bool_ = false;
			}
			else if ((_value.compare("1") == 0) || (_value.compare("TRUE") == 0) || (_value.compare("true") == 0))
			{
				value_.bool_ = true;
			}
			else
			{
				return	false;	
			}
		}
		break;

	case	INT:
		{
			value_.int_ = strtol(_value.c_str(), 0, 10);
		}
		break;

	case	UINT32:
		{
			value_.uint32_ = strtoul(_value.c_str(), 0, 10);
		}
		break;
	
	case	FLOAT:
		{
			value_.float_ = strtof(_value.c_str(), 0);
		}
		break;

	case	STRING:
		{
			value_string_ = _value;
		}
		break;

	default:
		return	false;
	}

	return	true;
}

bool	Value::Get
(
	bool& _value
)
{
	switch(type_)
	{
	case	BOOL:
		_value = value_.bool_;
		break;

	case	INT:
		_value = (value_.int_ != 0);
		break;

	case	UINT32:
		_value = (value_.uint32_ != 0);
		break;

	case	FLOAT:
		_value = (value_.float_ != 0);
		break;

	case	STRING:
		if ((value_string_.compare("0") == 0) || (value_string_.compare("false") == 0) || (value_string_.compare("off") == 0))
		{
			_value = false;
		}
		else if ((value_string_.compare("0") == 0) || (value_string_.compare("false") == 0) || (value_string_.compare("off") == 0))
		{
			_value = true;
		}
	default:
		return	false;
	}

	return	true;
}

bool	Value::Get
(
	int& _value
)
{
	switch(type_)
	{
	case	BOOL:
		_value = value_.bool_;
		break;

	case	INT:
		_value = value_.int_;
		break;

	case	UINT32:
		_value = value_.uint32_;
		break;

	case	FLOAT:
		_value = value_.float_;
		break;

	case	STRING:
		_value = strtol(value_string_.c_str(),0, 10);
		break;

	default:
		return	false;
	}

	return	true;
}

bool	Value::Get
(
	uint32_t& _value
)
{
	switch(type_)
	{
	case	BOOL:
		_value = value_.bool_;
		break;

	case	INT:
		_value = value_.int_;
		break;

	case	UINT32:
		_value = value_.uint32_;
		break;

	case	FLOAT:
		_value = value_.float_;
		break;

	case	STRING:
		_value = strtoul(value_string_.c_str(),0, 10);
		break;

	default:
		return	false;
	}

	return	true;
}

bool	Value::Get
(
	float& _value
)
{
	switch(type_)
	{
	case	BOOL:
		_value = value_.bool_;
		break;

	case	INT:
		_value = value_.int_;
		break;

	case	UINT32:
		_value = value_.uint32_;
		break;

	case	FLOAT:
		_value = value_.float_;
		break;

	case	STRING:
		_value = strtof(value_string_.c_str(),0);
		break;

	default:
		return	false;
	}

	return	true;
}

bool	Value::Get
(
	std::string& _value
)
{
	switch(type_)
	{
	case	BOOL:
		{
			ostringstream	oss;

			oss << value_.bool_;

			_value = oss.str();
		}
		break;

	case	INT:
		{
			ostringstream	oss;

			oss << value_.int_;

			_value = oss.str();
		}
		break;

	case	UINT32:
		{
			ostringstream	oss;

			oss << value_.uint32_;

			_value = oss.str();
		}
		break;

	case	FLOAT:
		{
			ostringstream	oss;

			oss << value_.float_;

			_value = oss.str();
		}
		break;

	case	STRING:
		_value = value_string_;
		break;

	default:
		return	false;
	}

	return	true;
}

const
Value&	Value::operator =
(
	const Value& _value
)
{
	type_ = _value.type_;
	switch(type_)
	{
	case	BOOL:	value_.bool_ = _value.value_.bool_;	break;
	case	FLOAT:	value_.float_= _value.value_.float_;	break;
	case	STRING:	value_string_= _value.value_string_;	break;
	case	INT:
	case	UINT32:
	default:	value_.uint32_ = _value.value_.uint32_;
	}

	return	*this;
}

const 
string&	Value::ToString()
{
	ostringstream	oss;

	switch(type_)
	{
	case	BOOL:	oss << value_.bool_; 	value_string_ = oss.str();	break;
	case	INT:	oss << value_.int_; 	value_string_ = oss.str();	break;
	case	UINT32:	oss << value_.uint32_; 	value_string_ = oss.str();	break;
	case	FLOAT:	oss << value_.float_; 	value_string_ = oss.str();	break;
	}

	return	value_string_;
}
