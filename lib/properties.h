#ifndef	PROPERTIES_H_
#define	PROPERTIES_H_

#include <map>
#include <libjson/libjson.h>
#include "defined.h"
#include "value.h"

class	Properties
{
public:
	Properties();
	~Properties();

	size_t 	Count();
	bool	Add(const std::string& _name, const Value& _value);
	virtual
	bool	Add(const std::string& _name, bool 	_value);
	virtual
	bool	Add(const std::string& _name, int	_value);
	virtual
	bool	Add(const std::string& _name, uint32_t	_value);
	virtual
	bool	Add(const std::string& _name, float	_value);
	virtual
	bool	Add(const std::string& _name, const std::string&	_value);

	virtual
	bool	Del(const std::string& _name);

	virtual
	bool	Get(const std::string& _name, bool&  _value);
	virtual
	bool	Get(const std::string& _name, int&	_value);
	virtual
	bool	Get(const std::string& _name, uint32_t&	_value);
	virtual
	bool	Get(const std::string& _name, float& _value);
	virtual
	bool	Get(const std::string& _name, std::string& _value);

	virtual
	bool	Set(const std::string& _name, const Value&	_value);
	virtual
	bool	Set(const std::string& _name, bool  _value);
	virtual
	bool	Set(const std::string& _name, int	_value);
	virtual
	bool	Set(const std::string& _name, uint32_t	_value);
	virtual
	bool	Set(const std::string& _name, float _value);
	virtual
	bool	Set(const std::string& _name, const std::string& _value);

	Value*	operator[](const std::string& _name);
	Value*	operator[](int _index);

	JSONNode	JSON();
protected:
	std::map<std::string, Value>	map_;

	Value*	Get(const std::string& _name);

};

#endif
