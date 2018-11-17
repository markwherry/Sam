#ifndef __AWaitEvent_h__
#define __AWaitEvent_h__

#include "APlatform.h"

namespace Atom {

class AWaitEvent
{
public:
	AWaitEvent(bool manualReset = false);
	~AWaitEvent();

	bool set(void);
	bool pulse(void);
	bool wait(int msTime = -1);
	bool reset(void);

protected:
private:
	HANDLE hEvent;
};

}

#endif