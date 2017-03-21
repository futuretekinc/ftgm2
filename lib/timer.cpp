#include <cstddef>
#include <unistd.h>
#include "timer.h"

using namespace std;

Time::Time()
{
	gettimeofday(&time, NULL);
}

void Time::Update()
{
	gettimeofday(&time, NULL);
}

void	Time::Add(uint64_t _milliseconds)
{
	time.tv_sec += _milliseconds / 1000;
	time.tv_usec += (_milliseconds % 1000) * 1000;
	if (time.tv_usec / 1000000)
	{
		time.tv_sec += 1;
		time.tv_usec -= 1000000;
	}
}

uint64_t	Time::Milliseconds()
{
	return	time.tv_sec * (uint64_t)1000 + (time.tv_usec / 1000);
}

const Time&	Time::GetCurrentTime()
{
	static	Time current_time;

	gettimeofday(&current_time.time, NULL);

	return	current_time;
}

string	Time::ToString()
{
	char	buffer[64];

	strftime(buffer, sizeof(buffer), "%Y%m%d-%H:%M:%S", localtime(&time.tv_sec));

	return	string(buffer);
}

void	Timer::Start()
{
	gettimeofday(&time_, NULL);
}

uint64_t Timer::StartTime()
{
	return	time_.tv_sec * (uint64_t)1000 + (time_.tv_usec / 1000);
}

uint64_t Timer::ElapsedTime()
{
	struct	timeval current_time_;

	gettimeofday(&current_time_, NULL);

	long int usec_diff = current_time_.tv_usec - time_.tv_usec;
	long int sec_diff = current_time_.tv_sec - time_.tv_sec;

	return	(sec_diff * (uint64_t)1000) + (usec_diff / 1000);
}

bool	Timer::IsExpired
(
	uint64_t	_milliseconds
)
{
	return	(ElapsedTime() > _milliseconds);
}

string	Timer::ToString()
{
	char	buffer[64];

	strftime(buffer, sizeof(buffer), "%Y%m%d-%H:%M:%S", localtime(&time_.tv_sec));

	return	string(buffer);
}

////////////////////////////////////////////////////////////
// Class TimeoutTimer
////////////////////////////////////////////////////////////
TimeoutTimer::TimeoutTimer()
{
	gettimeofday(&time_, NULL);
}

TimeoutTimer::TimeoutTimer
(
	const TimeoutTimer& _timer
)
{
	time_ = _timer.time_;
}

TimeoutTimer::TimeoutTimer
(
	const Time& _time
)
{
	time_ = _time.time;
}

uint64_t	TimeoutTimer::Milliseconds()
{
	return	time_.tv_sec * (uint64_t)1000 + (time_.tv_usec / 1000);
}

uint64_t	TimeoutTimer::RemainTime()
{
	Time	current_time = Time::GetCurrentTime();

	if (current_time.Milliseconds() >= Milliseconds())
	{
		return	0;	
	}

	return	Milliseconds() - current_time.Milliseconds();
}

bool	TimeoutTimer::IsExpired()
{
	struct timeval	current_time_;
	
	gettimeofday(&current_time_, NULL);

	return	((time_.tv_sec < current_time_.tv_sec) || ((time_.tv_sec == current_time_.tv_sec) && (time_.tv_usec < current_time_.tv_usec)));
}

uint64_t	TimeoutTimer::WaitingForExpired()
{
	uint64_t	remain_time;
	
	remain_time = RemainTime();
	if (remain_time != 0)
	{
		usleep(remain_time * 1000);	
	}

	return	remain_time;
}

void	TimeoutTimer::Add
(
	uint64_t _milliseconds
)
{
	time_.tv_sec += _milliseconds / 1000;
	time_.tv_usec += (_milliseconds % 1000) * 1000;
	if (time_.tv_usec / 1000000)
	{
		time_.tv_sec += 1;
		time_.tv_usec -= 1000000;
	}
}

string	TimeoutTimer::ToString()
{
	char	buffer[64];

	strftime(buffer, sizeof(buffer), "%Y%m%d-%H:%M:%S", localtime(&time_.tv_sec));

	return	string(buffer);
}

LoopTimer::LoopTimer
(
	uint32_t _interval
)
:	TimeoutTimer(), interval_(_interval)
{
}

LoopTimer::LoopTimer
(
	const Time& _time, 
	uint32_t _interval
)
:	TimeoutTimer(_time), interval_(_interval)
{
}

void	LoopTimer::Start()
{
	gettimeofday(&time_, NULL);
	Add(interval_);
}

uint64_t	LoopTimer::WaitingForExpired()
{
	TimeoutTimer::WaitingForExpired();

	Add(interval_);

	return	RemainTime();
}
