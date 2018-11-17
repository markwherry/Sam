#ifndef CSTASKTHREAD_H
#define CSTASKTHREAD_H

#include "AThread.h"
#include "ATaskOld.h"
#include "ACriticalSection.h"
#include "ALock.h"

namespace Atom {

class ATaskQueue;

class ATaskThread : public AThread
{
public:
	ATaskThread (ATaskQueue* queue, int id);
	~ATaskThread ();

	int assignTask (ATask *task);
//	bool isBusy (void);
protected:
private:
	void threadProcess (void);

	ACriticalSection csTask;

	ATaskQueue *m_queue;
	int m_id;
	ATask *m_task;
	HANDLE hEventTaskAssigned;
};

}

#endif