#include "ATaskThreadOld.h"
#include "ATaskQueueOld.h"

#include "ALog.h"

namespace Atom {

ATaskThread::ATaskThread (ATaskQueue* queue, int id)
:
m_queue (queue),
m_id (id)
{
	hEventTaskAssigned = CreateEvent ( NULL , false , false , NULL );
}

ATaskThread::~ATaskThread ()
{
}

int ATaskThread::assignTask (ATask *task)
{
	ALock lock (&csTask);

	if (task==NULL)
		return 0;

	m_task = task;
	SetEvent (hEventTaskAssigned);

	return 1;
}

void ATaskThread::threadProcess (void)
{
	for (;;) {
		WaitForSingleObject (hEventTaskAssigned, INFINITE);

		if (m_task!=NULL) {
			//qDebug ("[Thread:%d] About to execute main()...", m_id);
			m_task->main ();
			//qDebug ("[Thread:%d] After main ()", m_id);
			csTask.enter ();
			m_task = NULL;
			csTask.leave ();
		} else {
			//qDebug ("m_task = NULL (%d)", m_id);
		}

		m_queue->getNextTask (m_id);
	}
}

}