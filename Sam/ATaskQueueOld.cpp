#include "ATaskQueueOld.h"

#include "ALog.h"

#include <math.h>

namespace Atom {

ATaskQueue::ATaskQueue ()
:
m_activeTasks (0)
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
	//m_numThreads = 1;

	for (int i=0; i<m_numThreads; i++) {
		aLog("Creating thread %d/%d...", i, m_numThreads);
		thread[i] = new ATaskThread (this, i);	
		thread[i]->startThread ();
		thread[i]->setPriority (THREAD_PRIORITY_TIME_CRITICAL);
		thread[i]->setAffinity (i); //(int)pow(2.0,i)
		threadBusy[i] = false;
	}

	hEventComplete = CreateEvent ( NULL , false , false , NULL );
}

ATaskQueue::~ATaskQueue ()
{
}

int ATaskQueue::addTask (ATask* task)
{
	ALock lock (&csTaskQueue);

	//CSDEBUG("CsTaskQueue::addTask ()");

	if (task==NULL)
		return 0;

	m_activeTasks++;

	taskQueue->add (task);

	return 1;
}

void ATaskQueue::putTask(ATask* task)
{
	m_activeTasks++;
	taskQueue->add(task);
}

int ATaskQueue::start (void)
{
	ALock lock (&csTaskQueue);

	//aLog("CsTaskQueue::start ()");

	int tasksToStart = taskQueue->size ();
	//aLog("tasksToStart = %d", tasksToStart);
	if (tasksToStart>m_numThreads)
		tasksToStart = m_numThreads;
	//aLog("tasksToStart = %d", tasksToStart);

	for (int i=0; i<tasksToStart; i++) {
		thread[i]->assignTask (taskQueue->get ());
	}


	return 1;
}

void ATaskQueue::waitForAllTasksToComplete (void)
{
	//aLog("ATaskQueue::waitForAllTasksToComplete ()");
	WaitForSingleObject (hEventComplete, INFINITE);
	CSASSERT (m_activeTasks==0);
}

void ATaskQueue::getNextTask (int threadId)
{
	ALock lock (&csTaskQueue);

	//CSDEBUG("getNextTask (%d)", threadId);

	threadBusy[threadId] = false;

	m_activeTasks--;
	//CSDEBUG("m_activeTasks = %d",m_activeTasks);

	if (m_activeTasks<1) {
		SetEvent (hEventComplete);
		return;
	}

	if (taskQueue->size()>0)
		thread[threadId]->assignTask (taskQueue->get());

}

}