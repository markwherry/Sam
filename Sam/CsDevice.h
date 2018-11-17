#ifndef __CsDevice_h__
#define __CsDevice_h__

#include "AString.h"
using namespace Atom;

namespace Cs {

struct CsDeviceInfo
{
	AString name;
	AString uuid;
};

class CsDevice
{
public:
	CsDevice()
		: m_active(false)
	{
	}

	virtual int init(void) { return 1; }
	virtual int exit(void) { return 1; }

	virtual int start(void) { return 1; }
	virtual int stop(void) { return 1; }

	bool isActive(void) { return m_active; }

	CsDeviceInfo* deviceInfo(void) { return &m_deviceInfo; }

protected:
	void setActive(bool active) { m_active = active; }
	CsDeviceInfo m_deviceInfo;

private:
	bool m_active;
	
};

}

#endif