#ifndef __ATaskThread_h__
#define __ATaskThread_h__

#include "AThread.h"
#include "ATask.h"
#include "ACriticalSection.h"
#include "ALock.h"

namespace Atom {

class ATaskQueue;

class ATaskThread : public AThread
{
public:
	ATaskThread(ATaskQueue* queue, int id);
	~ATaskThread();

	int assignTask(ATask *task);
	long isBusy (void);

protected:
private:
	void threadProcess (void);

	ACriticalSection csTask;

	ATaskQueue *m_queue;
	int m_id;
	ATask *m_task;
	HANDLE hEventTaskAssigned;
	long m_busy;
};

}

#endif // __ATaskThread_h__