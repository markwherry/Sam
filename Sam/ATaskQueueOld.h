#ifndef CSTASKQUEUE_H
#define CSTASKQUEUE_H

#include "ATaskOld.h"
#include "ATaskThreadOld.h"
#include "ARingBuffer.h"
#include "ACriticalSection.h"
#include "ALock.h"

#define MAX_THREADS 32

namespace Atom {

class ATaskQueue
{
public:
	ATaskQueue ();
	~ATaskQueue ();

	int addTask (ATask* task);
	void putTask(ATask* task);
	void getNextTask (int threadId);

	int start (void);
	void waitForAllTasksToComplete (void);

protected:
private:
	ARingBuffer<ATask*> *taskQueue;
	ACriticalSection csTaskQueue;

	int m_numProcessors;
	ATaskThread *thread[MAX_THREADS];
	int m_numThreads;
	bool threadBusy[MAX_THREADS];

	int m_activeTasks;

	HANDLE hEventComplete;
};

}

#endif