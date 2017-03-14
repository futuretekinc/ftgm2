#ifndef	SCHEDULER_H_
#define	SCHEDULER_H_

#include "timer.h"
#include <list>
#include <vector>

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
	:	list_()
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
		typename std::list<ScheduleItem<T>*>::iterator it = list_.begin();
		while(it != list_.end())
		{
			if ((*it)->timer.Milliseconds() > _item->timer.Milliseconds())
			{
				std::cout << "Insert : " << _item->data << "  " << _item->timer.ToString() << std::endl;
				list_.insert(it, _item);
				return	RET_VALUE_OK;
			}
			it++;
		}
	
		list_.push_back(_item);

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

	void	Pop(bool destroy = false)
	{
		ScheduleItem<T> *item = list_.front();

		list_.pop_front();	

		if (destroy && (item != NULL))
		{
			delete item;
		}
	}

	void	Pop(const T& _data, bool destroy = false)
	{	
		typename std::list<ScheduleItem<T>*>::iterator it = list_.begin();
		while(it != list_.end())
		{
			if ((*it)->data == _data)
			{
				ScheduleItem<T>* item = (*it);

				list_.erase(it);

				if (destroy && (item != NULL))
				{
					delete item;
				}
			}
		}

		
	}

private:
	std::list<ScheduleItem<T> *> list_;
};

#endif
