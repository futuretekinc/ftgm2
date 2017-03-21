#ifndef	SCHEDULER_H_
#define	SCHEDULER_H_

#include "timer.h"
#include <list>
#include <vector>
#include "locker.h"

template <typename T>
struct	ScheduleItem
{
	ScheduleItem(const ScheduleItem& _item)
	{
		timer = _item.timer;
		data  = _item.data;
	}

	ScheduleItem(const TimeoutTimer& _timer, const T& _data)
	{
		timer = _timer;
		data  = _data;
	}

	TimeoutTimer 	timer;
	T				data;	
};

template <typename T>
class	Scheduler
{
public:

	Scheduler()
	:	list_(),locker_()
	{
	}

	~Scheduler()
	{
		while(list_.size() != 0)
		{
			Pop();
		}
	}

	int	Count()
	{
		return	list_.size();	
	}

	ScheduleItem<T>*	Top()
	{
		if (list_.size() != 0)
		{
			return	list_.front();
		}

		return	NULL;
	}

	RetValue	Push(ScheduleItem<T>* _item)
	{
		locker_.Lock();

		typename std::list<ScheduleItem<T>*>::iterator it = list_.begin();
		while(it != list_.end())
		{
			if ((*it)->timer.Milliseconds() > _item->timer.Milliseconds())
			{
				break;
			}
			it++;
		}
	
		if (it != list_.end())
		{
			list_.insert(it, _item);
		}
		else
		{
			list_.push_back(_item);
		}

		locker_.Unlock();

		return	RET_VALUE_OK;
	}

	RetValue	Push(const TimeoutTimer& _timer, const T& _data)
	{
		RetValue	ret_value = RET_VALUE_NOT_ENOUGH_MEMORY;

		ScheduleItem<T> *item = new ScheduleItem<T>(_timer, _data);
		if (item != NULL)
		{
			return	Push(item);
		}

		return	ret_value;
	}

	void	Pop(bool _destroy = false)
	{
		locker_.Lock();

		ScheduleItem<T> *item = list_.front();

		list_.pop_front();	

		if (_destroy && (item != NULL))
		{
			delete item;
		}

		locker_.Unlock();
	}

	RetValue	Pop(const T& _data, bool _destroy = false)
	{	
		RetValue	ret_value = RET_VALUE_OBJECT_NOT_FOUND;

		locker_.Lock();
		
		typename std::list<ScheduleItem<T>*>::iterator it = list_.begin();
		while(it != list_.end())
		{
			if ((*it)->data == _data)
			{
				ScheduleItem<T>* item = (*it);

				list_.erase(it);

				if (_destroy && (item != NULL))
				{
					delete item;
				}

				ret_value = RET_VALUE_OK;
				break;
			}
		}

		locker_.Unlock();

		return	ret_value;	
	}

private:
	std::list<ScheduleItem<T> *> list_;
	Locker	locker_;						
};

#endif
