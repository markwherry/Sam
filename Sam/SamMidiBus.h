#ifndef __SamMidiBus_h__
#define __SamMidiBus_h__

#include "SamBus.h"

#include "CsMidiManager.h"
using namespace Cs;

class SamMidiBus : public SamBus
{
public:
	SamMidiBus(AModel *parent, CsMidiManager *midiManager, int id, AString name, int deviceIndex, int portIndex);

	int deviceIndex(void) { return m_deviceIndex; }
	void setDeviceIndex(int deviceIndex) { m_deviceIndex = deviceIndex; }

	int portIndex(void) { return m_portIndex; }
	void setPortIndex(int portIndex) { m_portIndex = portIndex; }

protected:
private:
	CsMidiManager *m_midiManager;
	int m_deviceIndex;
	int m_portIndex;
};

#endif // __SamMidiBus_h__