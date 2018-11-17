#include "ATaskQueue.h"

#include "ALog.h"

#include <math.h>

namespace Atom {

ATaskQueue::ATaskQueue ()
: m_activeTasks (0)
{
	aLog("CsTaskQueue::CsTaskQueue ()\n");

	taskQueue = new ARingBuffer<ATask*>(1024);
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	m_numProcessors = sysInfo.dwNumberOfProcessors;
	aLog("Number of processors = %d\n", m_numProcessors);

	m_numThreads = m_numProcessors;
	
	aLog("m_numThreads = %d", m_numThreads);
	if (m_numThreads>MAX_THREADS)
		m_numThreads = MAX_THREADS;

	//if(m_numThreads>4)
	//	m_numThreads--;

	// Force single processor mode:
	//m_numThreads = 4;

	for (int i=0; i<m_numThreads; i++) {
		aLog("Creating thread %d/%d...", i, m_numThreads);
		thread[i] = new ATaskThread(this, i);	
		thread[i]->startThread();
		thread[i]->setPriority(AThread::TimeCritical);
		//thread[i]->setPriority(THREAD_PRIORITY_TIME_CRITICAL);
		thread[i]->setAffinity(i);
		//thread[i]->setAffinityMask((int)pow(2.0,i));
		////threadBusy[i] = false;
	}

	hEventComplete = CreateEvent( NULL , false , false , NULL );
}

ATaskQueue::~ATaskQueue ()
{
}

int ATaskQueue::addTask(ATask* task)
{
	//csDebug("CsTaskQueue::addTask() id=%d", task->id());
	ALock lock(&csTaskQueue);

	//csDebug("CsTaskQueue::addTask ()");

	if(!task) {
		return 0;
	}

	m_activeTasks++;

	task->setQueued();
	taskQueue->add(task);

	//csDebug("EXIT: CsTaskQueue::addTask()");

	return 1;
}

void ATaskQueue::runInCurrentThread(void)
{
	while(taskQueue->size()>0) {
		ATask *task = taskQueue->get();
		//csDebug("Executing task...");
		if(!task->dependenciesCompleted()) {
			addTask(task);
		} else {
			task->main();
			task->setCompleted();
		}
	}
}

void ATaskQueue::run(void)
{
	ALock lock (&csTaskQueue);

	//csDebug("CsTaskQueue::start ()");

	int tasksToStart = taskQueue->size ();
	//csDebug("tasksToStart = %d", tasksToStart);
	if (tasksToStart>m_numThreads)
		tasksToStart = m_numThreads;
	//csDebug("tasksToStart = %d", tasksToStart);

	for (int i=0; i<tasksToStart; i++) {
		thread[i]->assignTask(taskQueue->get());
	}

	if(tasksToStart<1) {
		SetEvent(hEventComplete);
	}
}

void ATaskQueue::waitForAllTasksToComplete(void)
{
	WaitForSingleObject(hEventComplete, INFINITE);
	//aAssert(m_activeTasks==0);
}

void ATaskQueue::getNextTask(int threadId)
{
	//csDebug("CsTaskQueue::getNextTask(%d)", threadId);
	ALock lock(&csTaskQueue);

	m_activeTasks--;
	//csDebug("m_activeTasks = %d",m_activeTasks);

	if (m_activeTasks<1) {
		SetEvent(hEventComplete);
		return;
	}

	if (taskQueue->size()>0) {
		thread[threadId]->assignTask(taskQueue->get());

	}

	//csDebug("EXIT: CsTaskQueue::getNextTask(%d)", threadId);

}

}