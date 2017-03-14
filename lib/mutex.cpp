#include "mutex.h"

Mutex::Mutex()
{
	locked_ = false;
}

bool	Mutex::Lock()
{
	mutex_.lock();

	locked_ = true;

	return	true;
}

bool	Mutex::Unlock()
{
	mutex_.unlock();

	locked_ = false;

	return	true;
}

bool	Mutex::IsLocked()
{
	return	locked_;
}
