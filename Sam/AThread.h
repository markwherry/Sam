#ifndef __AThread_h__
#define __AThread_h__

#include "APlatform.h"
#include "AWaitEvent.h"

namespace Atom {

class AThread
{
public:
	AThread();
	~AThread();

	int startThread(void);
	int stopThread(void);

	void setPriority(int priority);
	void setAffinity(int processorId);

	bool isThreadRunning(void) { return m_threadRunning; }

	enum Priorities {
		Lowest = THREAD_PRIORITY_LOWEST,
		Normal = THREAD_PRIORITY_NORMAL,
		Highest = THREAD_PRIORITY_HIGHEST,
		TimeCritical = THREAD_PRIORITY_TIME_CRITICAL
	};
	
	void waitForThreadToExit(void);

protected:
	virtual void threadProcess(void) = 0;
	static DWORD WINAPI ThreadProc(LPVOID lpParam);
	void setThreadRunning(bool running) { m_threadRunning = running; }

private:
	HANDLE m_hThread;
	DWORD m_threadId;
	bool m_threadRunning;
	AWaitEvent *m_waitForExit;
};

}

#endif