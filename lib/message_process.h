#ifndef	MESSAGE_PROESS_H_
#define	MESSAGE_PROESS_H_

#include <thread>
#include "defined.h"
#include "object.h"
#include "message.h"
#include "message_queue.h"

class	MessageProcess : public Object
{
public:

	MessageProcess();
			
	virtual	RetValue	Start(uint32_t _timeout = 5000, bool _sync = false);
	virtual	RetValue	Stop();
	virtual	bool		IsRun();

	virtual	void		OnMessage(Message *_message);
	virtual	void		OnQuit(Message* _message);

	virtual	void		Post(Message* _message);

protected:
	std::thread*		thread_;
	bool				stop_;
	unsigned int		loop_interval_;
	MessageQueue		message_queue_;

	virtual	void		PreProcess();
	virtual	void 		Process();
	virtual	void		PostProcess();

	static	void 		Thread(MessageProcess* _mp);

};

#endif
