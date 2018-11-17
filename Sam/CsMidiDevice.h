#ifndef __CsMidiDevice_h__
#define __CsMidiDevice_h__

#include "ATypes.h"
#include "ALog.h"
using namespace Atom;

#include "CsDevice.h"
#include "CsMidiMessage.h"

namespace Cs {

class CsMidiManager;

class CsMidiDevice : public CsDevice
{
public:
	CsMidiDevice(CsMidiManager *midiManager, int deviceIndex)
		: m_midiManager(midiManager)
		, m_deviceIndex(deviceIndex)
	{
	}

	virtual AStatus init(void) { return 1; }
	virtual AStatus exit(void) { return 1; }

	virtual AStatus start(bool manuallyActivatePorts = false) { return 1; }
	virtual AStatus stop(void) { return 1; }

	enum CsMidiDevicePortType {
		Input  = 0,
		Output = 1
	};

	virtual int numPorts(CsMidiDevicePortType type) const { return 0; }
	virtual AString portName(int portIndex, CsMidiDevicePortType type) const { return AString(""); }
	virtual bool isPortActive(int portIndex, CsMidiDevicePortType type) const { return false; }
	virtual AStatus setPortActive(int portIndex, CsMidiDevicePortType type, bool active) { return AError; }

	virtual void sendMidiEvent(CsMidiMessage midiMessage) = 0;

	void debugPorts(void)
	{
		for(int i=0; i<numPorts(Input); i++) {
			aLog("[%d] %s", isPortActive(i, Input), portName(i, Input).toChar());
		}
		for(int i=0; i<numPorts(Output); i++) {
			aLog("[%d] %s", isPortActive(i, Output), portName(i, Output).toChar());
		}
	}

protected:
	CsMidiManager* midiManager(void) { return m_midiManager; }
	int deviceIndex(void) { return m_deviceIndex; }

private:
	CsMidiManager *m_midiManager;
	int m_deviceIndex;
};

}

#endif 