#include "AStateManager.h"
#include "ALog.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStateEvent::AStateEvent(int context, int item, void* updatedObject, void *fromObject, float value, AString str)
: m_context(context)
, m_item(item)
, m_updatedObject(updatedObject)
, m_fromObject(fromObject)
, m_value(value)
, m_string(str)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStateManager::AStateManager()
{
	//aLog("AStateManager::AStateManager()");

	eventQueue = new AQueue<AStateEvent*>;
	waitForEvent = new AWaitEvent();
	startThread();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStateManager::~AStateManager()
{
	stopThread();
	waitForEvent->set();
	waitForThreadToExit();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AStateManager::threadProcess(void)
{
	while(isThreadRunning()) {

		waitForEvent->wait();

		cs.enter();

		int size = eventQueue->size();
		//aLog("size = %d", size);

		for(int i=0; i<size; i++) {
			//cs.enter();
			AStateEvent* ev = eventQueue->pop();
			//cs.leave();
			if(!ev) { aLog("!ev"); }
			//aLog("m_listeners.size() = %d", m_listeners.size());
			for(int j=0; j<m_listeners.size(); j++) {
				//aLog("++ Listener %d/%d", j, m_listeners.size());
				if(m_listeners[j]->context == -1 || m_listeners[j]->context == ev->context()) {
					//aLog("Sending... [%d] context = %d, item =%d", m_listeners[j], ev->context(), ev->item());
					m_listeners[j]->listener->onState(ev);
					//aLog("Sent.");
				}
			}
			delete ev;
		} //else {
			
		//}

		cs.leave();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void AStateManager::threadProcess(void)
{
	while(isThreadRunning()) {
		waitForEvent->wait();
		//csDebug("Received Signal");
		int numEvents = eventQueue->size();
		for(int i=0; i<numEvents; i++) {
			//cDebug("***** Sending state %d/%d", i, numEvents);
			AStateEvent* ev = eventQueue->get();
			if(!ev) { aLog("!ev"); }
			for(int j=0; j<m_listeners.size(); j++) {
				//cDebug("++ Listener %d/%d", j, m_listeners.size());
				if(m_listeners[j]->context == -1 || m_listeners[j]->context == ev->context()) {
					m_listeners[j]->listener->onState(ev);
				}
			}
			delete ev;
		}
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AStateManager::registerListener(AStateListener* listener, int context)
{
	//aLog("AStateManager::registerListener()");

	AStateListenerInfo* listenerInfo = new AStateListenerInfo;
	listenerInfo->listener = listener;
	listenerInfo->context = context;
	m_listeners.append(listenerInfo);

	//aLog("m_listeners.size() = %d", m_listeners.size());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AStateManager::unregisterListener(AStateListener *listener, int context)
{
	cs.enter();
	//aLog("AStateManager::unregisterListener()");
	for(int i=0; i<m_listeners.size(); i++) {
		if(m_listeners[i]->listener == listener && m_listeners[i]->context == context) {
			//aLog("Removing Listener [%d]", m_listeners[i]);
			m_listeners.removeAt(i);
			break;
		}
	}
	cs.leave();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AStateManager::updateState(int context, int item, void* updatedObject, void *fromObject, float value, AString str)
{
	//aLog("AStateManager::updateState()");
	AStateEvent *ev = new AStateEvent(context, item, updatedObject, fromObject, value, str);
	cs.enter();
	eventQueue->push(ev);
	cs.leave();
	waitForEvent->set();
}

}