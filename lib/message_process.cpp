#include <iostream>
#include <unistd.h>
#include "trace.h"
#include "timer.h"
#include "message_process.h"

using namespace std;

MessageProcess::MessageProcess()
: Object()
{
	thread_ = NULL;
	stop_	= true;
	loop_interval_ = 10;
}

RetValue	MessageProcess::Start(uint32_t _timeout, bool _sync)
{
	TimeoutTimer	timer;

	timer.Add(_timeout);

	if (thread_ != NULL)
	{
		ERROR(this, RET_VALUE_OK, "Already started!");
		return	RET_VALUE_OK;	
	}

	thread_ = new thread(Thread, this);

	while((!timer.IsExpired()) &&  (!thread_->joinable()))
	{
		usleep(1000);		
	}
	
	while((!timer.IsExpired()) &&  (!IsRun()))
	{
		usleep(1000);		
	}

	if (_sync)
	{
		thread_->join();	

		delete thread_;

		thread_ = NULL;
	}

	return	RET_VALUE_OK;
}

RetValue	MessageProcess::Stop()
{
	if (thread_ != NULL)
	{
		Post(new MessageQuit());

		thread_->join();

		delete thread_;

		thread_ = NULL;
	}

	return	RET_VALUE_OK;
}

bool	MessageProcess::IsRun()
{
	return	!stop_;
}

void	MessageProcess::PreProcess()
{
}

void	MessageProcess::Process()
{
}

void	MessageProcess::PostProcess()
{
}

void	MessageProcess::OnMessage
(
	Message* _message
)
{
	switch(_message->type)
	{
	case	Message::TYPE_QUIT:
		{
			OnQuit(_message);
		}
		break;

	default:			
		{
			INFO(this, "Unknown message[%s].", ToString(_message).c_str());
		}
	}
}

void	MessageProcess::OnQuit
(
	Message*	_message
)
{
	stop_ = true;
}

void	MessageProcess::Post
(
	Message* _message
)
{
	message_queue_.Push(_message);
}

void MessageProcess::Thread
(
	MessageProcess* _mp
)
{
	_mp->PreProcess();

	_mp->stop_	= false;

	TimeoutTimer	timeout(Time::GetCurrentTime());

	_mp->Post(new MessageStarted);

	while(!_mp->stop_)
	{
		timeout.Add(_mp->loop_interval_);
	
		_mp->Process();

		while ((!_mp->stop_) && (_mp->message_queue_.TimedWait(timeout.RemainTime())))
		{
			Message* message = _mp->message_queue_.Pop();		

			if (message != NULL)
			{
				INFO(_mp, "Message : %s", ToString(message).c_str());
				_mp->OnMessage(message);

				delete message;
			}
		}
	}

	_mp->PostProcess();
}

