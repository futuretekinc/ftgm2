#include <iostream>
#include <iomanip>
#include "shell.h"
#include "object_manager.h"
#include "data_manager.h"
#include "device.h"

using namespace std;

RetValue	ShellCommandHelp
(
	std::string* _arguments, 
	uint32_t	_count,
	Shell<ObjectManager, DataManager>* _shell
)
{
	RetValue	ret_value;
	size_t		title_width = 16;

	if (_count == 1)
	{
		cout << "Help called" << endl;
	}
	else 
	{
	}

	return	ret_value;
}
