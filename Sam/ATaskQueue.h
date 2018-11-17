#ifndef __ATaskQueue_h__
#define __ATaskQueue_h__

#include "ATask.h"
#include "ATaskThread.h"
#include "ARingBuffer.h"
#include "ACriticalSection.h"
#include "ALock.h"

#define MAX_THREADS 32

namespace Atom {

class ATaskQueue
{
public:
	ATaskQueue();
	~ATaskQueue();

	int addTask(ATask* task);
	void getNextTask(int threadId);

	void runInCurrentThread(void);
	void run(void);
	void waitForAllTasksToComplete(void);

protected:
private:
	ARingBuffer<ATask*> *taskQueue;
	ACriticalSection csTaskQueue;

	int m_numProcessors;
	ATaskThread *thread[MAX_THREADS];
	int m_numThreads;
	//bool threadBusy[MAX_THREADS];

	int m_activeTasks;

	HANDLE hEventComplete;
};

}

#endif // __TaskQueue_h__