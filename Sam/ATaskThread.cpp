#include "ATaskThread.h"
#include "ATaskQueue.h"
#include "ALog.h"

namespace Atom {

ATaskThread::ATaskThread(ATaskQueue* queue, int id)
	: m_queue(queue)
	, m_id(id)
{
	hEventTaskAssigned = CreateEvent( NULL , false , false , NULL );
	InterlockedExchange(&m_busy, 0);
}

ATaskThread::~ATaskThread ()
{
}

int ATaskThread::assignTask(ATask *task)
{
	ALock lock(&csTask);

	if (task==NULL)
		return 0;

	m_task = task;
	SetEvent(hEventTaskAssigned);

	return 1;
}

long ATaskThread::isBusy(void)
{
	return InterlockedExchange(&m_busy, m_busy);
	//return m_busy;
}

void ATaskThread::threadProcess(void)
{
	for (;;) {
		WaitForSingleObject(hEventTaskAssigned, INFINITE);

		if (m_task!=NULL) {
			//qDebug ("[Thread:%d] About to execute main()...", m_id);
			
			InterlockedExchange(&m_busy, 1);

			if(!m_task->dependenciesCompleted()) {
				//csDebug("Task %d has incomplete dependencies.", m_task->id());
				m_queue->addTask(m_task);
			} else {
				m_task->setRunning();
				m_task->main();
				m_task->resetRunning();
				m_task->setCompleted();
			}
			
			//qDebug ("[Thread:%d] After main ()", m_id);
			
			csTask.enter();
			m_task = NULL;
			csTask.leave();

			InterlockedExchange(&m_busy, 0);
		} else {
			//qDebug ("m_task = NULL (%d)", m_id);
		}

		m_queue->getNextTask(m_id);
	}
}

}