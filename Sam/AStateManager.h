#ifndef __StateManager_h__
#define __StateManager_h__

#include "AThread.h"
#include "AList.h"
#include "ARingBuffer.h"
#include "AWaitEvent.h"
#include "AQueue.h"
#include "ALock.h"
#include "AString.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class AStateEvent
{
public:
	AStateEvent(int context, int item, void* updatedObject, void *fromObject, float value = 0.0f, AString str = "");

	int context(void) { return m_context; }
	int item(void) { return m_item; }
	void* updatedObject(void) { return m_updatedObject; }
	void* fromObject(void) { return m_fromObject; }
	float value(void) { return m_value; }
	AString string(void) { return m_string; }

protected:
private:
	int m_context;
	int m_item;
	void* m_updatedObject;
	void* m_fromObject;
	float m_value;
	AString m_string;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class AStateManager;

class AStateListener
{
	friend AStateManager;
public:
protected:
	virtual void onState(AStateEvent *ev) = 0;
private:
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class AStateManager : public AThread
{
public:
	AStateManager();
	~AStateManager();

	void threadProcess(void);

	void registerListener(AStateListener *listener, int context);
	void unregisterListener(AStateListener *listener, int context);
	void updateState(int context, int item, void* updatedObject, void *fromObject, float value = 0.0f, AString str = "");

protected:
private:
	struct AStateListenerInfo {
		AStateListener* listener;
		int context;
	};
	AList<AStateListenerInfo*> m_listeners;
	AQueue<AStateEvent*> *eventQueue;
	AWaitEvent *waitForEvent;
	ACriticalSection cs;
};

}

#endif