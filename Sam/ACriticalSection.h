// ACriticalSection
// A nice class to encapsulate a critical section, as described in 'Multithreading Applications in Win32'

#ifndef __ACriticalSection_h__
#define __ACriticalSection_h__

#include "APlatform.h"

namespace Atom {

class ACriticalSection
{
public:
	ACriticalSection();
	~ACriticalSection();

	void enter(void);
	void leave(void);

protected:
private:
	CRITICAL_SECTION criticalSection;
};

}

#endif // __ACriticalSection_h__