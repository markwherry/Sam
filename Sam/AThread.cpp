#include "AThread.h"
#include "ALog.h"

#include <math.h>

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AThread::AThread()
: m_hThread(NULL)
, m_threadId(0)
{
	m_waitForExit = new AWaitEvent(true);
}

AThread::~AThread()
{
	delete m_waitForExit;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int AThread::startThread(void)
{
	m_hThread = CreateThread(NULL, 0, ThreadProc, (void*)this, 0, &m_threadId);

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int AThread::stopThread(void)
{
	setThreadRunning(false);

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AThread::setPriority(int priority)
{
	int ret = SetThreadPriority(m_hThread, priority);
	if(ret == 0) {
		aLog("Error: CsThread::setPriority() = %d", ret);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AThread::setAffinity(int processorId)
{
	int ret = SetThreadAffinityMask(m_hThread, (int)pow(2.0, processorId));
	//csDebug("---> SetThreadAffinityMask()=%d", );
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AThread::waitForThreadToExit(void)
{
	m_waitForExit->wait();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
DWORD WINAPI AThread::ThreadProc(LPVOID lpParam)
{
	if(lpParam == NULL) {
		return 0;
	}

	AThread *thread = (AThread*)lpParam;
	thread->setThreadRunning(true);
	thread->threadProcess();
	thread->setThreadRunning(false);
	thread->m_waitForExit->set();

	return 1;
}

}