#include "ACriticalSection.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
ACriticalSection::ACriticalSection()
{
	InitializeCriticalSection(&criticalSection);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
ACriticalSection::~ACriticalSection()
{
	DeleteCriticalSection(&criticalSection);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ACriticalSection::enter(void)
{
	EnterCriticalSection(&criticalSection);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ACriticalSection::leave(void)
{
	LeaveCriticalSection(&criticalSection);
}

}