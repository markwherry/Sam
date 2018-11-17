#ifndef __APrecisionTimer__
#define __APrecisionTimer__

#include "APlatform.h"

//#include "tbb/tick_count.h"
//using namespace tbb;

namespace Atom {

class APrecisionTimer
{
public:
	APrecisionTimer();
	void start(void);
	void stop(void);
	float duration(void);
	//float cycles(void);
	
protected:

private:
	LARGE_INTEGER lpFrequency;
	LARGE_INTEGER lpCounterStart;
	LARGE_INTEGER lpCounterEnd;
	//tick_count tStart, tEnd;
};

}

#endif // __APrecisionTimer__