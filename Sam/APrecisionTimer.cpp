#include "APrecisionTimer.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
APrecisionTimer::APrecisionTimer()
{
	QueryPerformanceFrequency(&lpFrequency);

	QueryPerformanceCounter (&lpCounterStart);
	lpCounterEnd = lpCounterStart;

	//tStart = tick_count::now();
	//tEnd = tick_count::now();

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void APrecisionTimer::start(void)
{
	QueryPerformanceCounter(&lpCounterStart);
	//tStart = tick_count::now();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void APrecisionTimer::stop(void)
{
	QueryPerformanceCounter(&lpCounterEnd);
	//tEnd = tick_count::now();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float APrecisionTimer::duration(void)
{
	return (float)(lpCounterEnd.QuadPart-lpCounterStart.QuadPart)/lpFrequency.QuadPart;
	//return (tEnd - tStart).seconds();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*float APrecisionTimer::cycles(void)
{
	//return (float)(lpCounterEnd.QuadPart-lpCounterStart.QuadPart);
	return 0;
}*/

}