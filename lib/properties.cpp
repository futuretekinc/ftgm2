#include <cassert>
#include <string>
#include "properties.h"

using namespace	std;

Properties::Properties()
{
}

Properties::~Properties()
{
}


size_t 	Properties::Count()
{
	return	map_.size();
}

bool	Properties::Add
(
	const std::string& _name, 
	const Value& _value
)
{
	auto it = map_.find(_name);
	if (it != map_.end())
	{
		it->second = _value;
	}
	else
	{
		map_[_name] = _value;
	}

	return	true;
}

bool	Properties::Add
(
	const std::string& _name, 
	bool 	_value
)
{
	Value	value(_value);

	return	Add(_name, value);
}

bool	Properties::Add
(
	const std::string& _name, 
	int	_value
)
{
	Value	value(_value);

	return	Add(_name, value);
}

bool	Properties::Add
(
	const std::string& _name, 
	uint32_t	_value
)
{
	Value	value(_value);

	return	Add(_name, value);
}

bool	Properties::Add
(
	const std::string& _name, 
	float	_value
)
{
	Value	value(_value);

	return	Add(_name, value);
}

bool	Properties::Add
(
	const std::string& _name, 
	const std::string&	_value
)
{
	Value	value(_value);

	return	Add(_name, value);
}

bool	Properties::Del
(
	const std::string& _name
)
{
	auto it = map_.find(_name);
	if (it == map_.end())
	{
		return	false;
	}

	map_.erase(it);

	return	true;
}


bool	Properties::Get
(
	const std::string& _name, 
	bool& 	_value
)
{
	Value*	value;

	value = Get(_name);
	if (value == NULL)
	{
		return	false;	
	}

	return	value->Get(_value);
}

bool	Properties::Get
(
	const std::string& _name, 
	int&	_value
)
{
	Value*	value;

	value = Get(_name);
	if (value == NULL)
	{
		return	false;	
	}

	return	value->Get(_value);
}

bool	Properties::Get
(
	const std::string& _name, 
	uint32_t&	_value
)
{
	Value*	value;

	value = Get(_name);
	if (value == NULL)
	{
		return	false;	
	}

	return	value->Get(_value);
}

bool	Properties::Get
(
	const std::string& _name, 
	float&	_value
)
{
	Value*	value;

	value = Get(_name);
	if (value == NULL)
	{
		return	false;	
	}

	return	value->Get(_value);
}

bool	Properties::Get
(
	const std::string& _name, 
	std::string&	_value
)
{
	Value*	value;

	value = Get(_name);
	if (value == NULL)
	{
		return	false;	
	}

	return	value->Get(_value);
}

bool	Properties::Set
(
	const std::string& _name, 
	bool  _value
)
{
	Value*	value;

	value = Get(_name);
	if (value == NULL)
	{
		return	false;	
	}

	return	value->Set(_value);
}

bool	Properties::Set
(
	const std::string& _name, 
	int	_value
)
{
	Value*	value;

	value = Get(_name);
	if (value == NULL)
	{
		return	false;	
	}

	return	value->Set(_value);
}

bool	Properties::Set
(
	const std::string& _name, 
	uint32_t	_value
)
{
	Value*	value;

	value = Get(_name);
	if (value == NULL)
	{
		return	false;	
	}

	return	value->Set(_value);
}

bool	Properties::Set
(	
	const std::string& _name, 
	const std::string& _value
)
{
	Value*	value;

	value = Get(_name);
	if (value == NULL)
	{
		return	false;	
	}

	return	value->Set(_value);
}

Value*	Properties::operator[]
(
	const std::string& _name
)
{	
	return	Get(_name);

}

Value*	Properties::operator[]
(
	int _index
)
{
	if (_index < map_.size())
	{
		auto it = map_.begin();
		for(; (_index > 0) && (it != map_.end()) ; it++)
		{
		}

		return	&it->second;
	}

	return	NULL;
}

Value*	Properties::Get
(
	const std::string& _name
)
{	
	auto it = map_.find(_name);
	if (it == map_.end())
	{
		return	NULL;	
	}

	return	&it->second;
}

JSONNode	Properties::JSON()
{
	JSONNode	root;

	for(auto it = map_.begin(); it != map_.end() ; it++)
	{
		switch(it->second.GetType())
		{
		case	Value::BOOL:
			{
				bool	value;
				
				it->second.Get(value);
				root.push_back(JSONNode(it->first, value));
			}
			break;

		case	Value::INT:
			{
				int	value;
				
				it->second.Get(value);
				root.push_back(JSONNode(it->first, value));
			}
			break;

		case	Value::UINT32:
			{
				uint32_t	value;
				
				it->second.Get(value);
				root.push_back(JSONNode(it->first, value));
			}
			break;

		case	Value::FLOAT:
			{
				float	value;
				
				it->second.Get(value);
				root.push_back(JSONNode(it->first, value));
			}
			break;

		case	Value::STRING:
			{
				string	value;
				
				it->second.Get(value);
				root.push_back(JSONNode(it->first, value));
			}
			break;
		}
	}

	return	root;
}
