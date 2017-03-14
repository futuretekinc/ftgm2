#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "device.h"

using namespace std;

RetValue	ShellCommandTest
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager, DataManager>* _shell
)
{
	RetValue	ret_value;
	ObjectManager*	object_manager = _shell->Data1();

	if (object_manager == NULL)
	{
		ret_value = RET_VALUE_OBJECT_MANAGER_NOT_INITIALIZED;	
	}
	else 
	{
	}

	if (ret_value == RET_VALUE_NOT_SUPPORTED_FUNCTION)
	{
		cout << "Not supported command : ";

		for(uint32_t i = 0 ; i < _count ; i++)
		{
			cout << _arguments[i] << " ";	
		}

		cout << endl;
	}

	return	ret_value;
}
