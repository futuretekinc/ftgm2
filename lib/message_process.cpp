#include <iostream>
#include <unistd.h>
#include "trace.h"
#include "timer.h"
#include "message_process.h"

using namespace std;

MessageProcess::MessageProcess()
{
	thread_ = NULL;
	stop_	= true;
	loop_interval_ = 1000;
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
		Message *message = new MessageQuit();

		Post(message);

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
	TRACE(this, "Message[%s] Received.", ToString(_message).c_str());
	switch(_message->type)
	{
	case	Message::TYPE_QUIT:
		{
			OnQuit(_message);
		}
		break;

	default:			
		{
			TRACE(this, "Unknown message[%s]!", ToString(_message).c_str());
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
	TRACE(_mp, "%s started.", _mp->ClassName().c_str());

	_mp->PreProcess();

	_mp->stop_	= false;

	TimeoutTimer	timeout(Time::GetCurrentTime());

	_mp->Post(new MessageStarted);

	while(!_mp->stop_)
	{
		timeout.Add(_mp->loop_interval_);
	
		_mp->Process();
		
		if (_mp->message_queue_.TimedWait(timeout.RemainTime()))
		{
			Message* message = _mp->message_queue_.Pop();		

			if (message != NULL)
			{
				_mp->OnMessage(message);

				delete message;
			}
		}
	}

	_mp->PostProcess();
}

