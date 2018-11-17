// ALock
// A class that makes it easier to enter and leave a critical section without having
// to do so much clean-up, based on 'Multithreading Applications in Win32' (page 216)

#ifndef __ALock_h__
#define __ALock_h__

#include "ACriticalSection.h"

namespace Atom {

class ALock
{
public:
	ALock(ACriticalSection *cs);
	~ALock();

protected:
private:
	ACriticalSection *m_criticalSection;
};

}

#endif // __ALock_h__