#include "ALock.h"

namespace Atom {

ALock::ALock(ACriticalSection *cs)
: m_criticalSection(cs)
{
	m_criticalSection->enter();
}

ALock::~ALock()
{
	m_criticalSection->leave();
}

}