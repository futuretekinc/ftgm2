#ifndef	MUTEX_H_
#define	MUTEX_H_

#include <mutex>

class	Mutex
{
public:
			Mutex();

	bool	Lock();
	bool	Unlock();
	bool	IsLocked();

private:
	bool		locked_;
	std::mutex	mutex_;
};
#endif
