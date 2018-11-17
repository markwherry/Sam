#ifndef __ATask_h__
#define __ATask_h__

#include "ALog.h"
#include "ACriticalSection.h"
#include "ALock.h"
#include "AList.h"

namespace Atom {

class ATask
{
public:
	ATask()
	{
		//cDebug("Task()::Task()");
		
		m_queued = 0;
		m_running = 0;
		m_completed = 0;
		m_taskId = -1;
	}

	virtual void main(void) = 0;

	int taskId(void) { return m_taskId; }
	void setTaskId(int id) { m_taskId = id; }

	int numDependencies(void)
	{
		return (int)m_dependencies.size();
	}

	ATask* dependency(int id)
	{
		return m_dependencies[id];
	}

	bool dependenciesCompleted(void)
	{
		//CsLock lock(&csDependency);
		for(int i=0; i<m_dependencies.size(); i++) {
			if(!m_dependencies[i]->isCompleted()) {
				return false;
			}
		}

		return true;
	}

	void addDependency(ATask *task)
	{
		m_dependencies.append(task);
	}

	void clearDependencies(void)
	{
		m_dependencies.clear();
	}

	long isQueued(void)
	{
		return InterlockedExchange(&m_queued, m_queued);
	}

	void setQueued(void)
	{
		InterlockedExchange(&m_queued, 1);
	}

	long isCompleted(void)
	{
		ALock lock(&m_csCompleted);
		return m_completed;
		//return InterlockedExchange(&m_completed, m_completed);
	}

	void setCompleted(void)
	{
		ALock lock(&m_csCompleted);
		m_completed = 1;
		//InterlockedExchange(&m_completed, 1);
	}

	void resetCompleted(void)
	{
		ALock lock(&m_csCompleted);
		m_completed = 0;
		//InterlockedExchange(&m_completed, 0);
	} 

	long isRunning(void)
	{
		return InterlockedExchange(&m_running, m_completed);
	}

	void setRunning(void)
	{
		InterlockedExchange(&m_running, 1);
	}

	void resetRunning(void)
	{
		InterlockedExchange(&m_running, 0);
	} 

	void resetTask(void)
	{
		m_running = 0;
		m_completed = 0;
		m_queued = 0;
	}

protected:
private:
	long m_queued;
	long m_running;
	long m_completed;
	int m_taskId;
	AList<ATask*> m_dependencies;
	ACriticalSection m_csCompleted;
};

}

#endif 