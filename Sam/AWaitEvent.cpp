#include "AWaitEvent.h"
//#include "CsDebug.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AWaitEvent::AWaitEvent(bool manualReset)
{
	hEvent = CreateEvent(0, manualReset, false, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AWaitEvent::~AWaitEvent()
{
	CloseHandle(hEvent);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool AWaitEvent::set(void)
{
	if(SetEvent(hEvent)) {
		return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool AWaitEvent::pulse(void)
{
	if(PulseEvent(hEvent)) {
		return true;
	} 

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool AWaitEvent::wait(int msTime)
{
	if(msTime<0) {
		msTime = INFINITE;
	}
	int ret = WaitForSingleObject(hEvent, msTime);

	/*if(ret==WAIT_ABANDONED) {
			CSDEBUG("WAIT_ABANDONED");
		} else if(ret==WAIT_OBJECT_0) {
			CSDEBUG("WAIT_OBJECT_0");
		} else if(ret==WAIT_TIMEOUT) {
			CSDEBUG("WAIT_TIMEOUT");
		}*/

	if(ret==WAIT_OBJECT_0)
		return true;

	return false; // WAIT_TIMEOUT
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool AWaitEvent::reset(void)
{
	if(ResetEvent(hEvent)) {
		return true;
	}

	return false;
}

}