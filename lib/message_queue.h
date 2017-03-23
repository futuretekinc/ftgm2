#ifndef	MESSAGE_QUEUE_H_
#define	MESSAGE_QUEUE_H_

#include <queue>
#include <mutex>
#include "message.h"

class	MessageQueue
{
public:
	MessageQueue();
	~MessageQueue();

	void	Push(Message *_message);
	Message*	Front();
	Message*	Pop(bool _do_relese =false);

	bool TimedWait(int _milliseconds);

private:
	std::queue<Message *>	queue_;
	std::timed_mutex		lock_;
};

#endif
