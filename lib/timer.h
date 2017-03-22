#ifndef	TIMER_H_
#define	TIMER_H_
#include <sys/time.h>
#include <cstdint>
#include <string>
class	Time
{
public:
	Time();
	Time(const Time& _time)	: time(_time.time){}

	void 		Update();
	
	uint64_t	Milliseconds();
	uint64_t	Microseconds();
	
	void		Add(uint64_t	_milliseconds);

	static
	const Time&	GetCurrentTime();

	std::string	ToString();

	struct timeval	time;
};

class Timer 
{
public:
	void 		Start();
	uint64_t	StartTime();
	uint64_t	ElapsedTime();
	bool		IsExpired(uint64_t	_milliseconds);

	std::string	ToString();

private:
	struct timeval	time_;
};

class	TimeoutTimer
{
public:
	TimeoutTimer();
	TimeoutTimer(const Time& _time);
	TimeoutTimer(const TimeoutTimer& _timer);

	uint64_t	Milliseconds();
	uint64_t	RemainTime();
	bool		IsExpired();
	uint64_t	WaitingForExpired();
	void		Add(uint64_t	_milliseconds);

	std::string	ToString();

protected:
	struct timeval time_;
};

class	LoopTimer : public TimeoutTimer
{
public:
	LoopTimer(uint32_t _interval);
	LoopTimer(const Time& _time, uint32_t _interval);
	uint64_t	WaitingForExpired();

	void	Start();

protected:
	uint32_t	interval_;
};

#endif

