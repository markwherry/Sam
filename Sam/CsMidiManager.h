#ifndef __CsMidiManager_h__
#define __CsMidiManager_h__

#include "AThread.h"
#include "AWaitEvent.h"
#include "ARingBuffer.h"
#include "AList.h"
#include "ACriticalSection.h"
using namespace Atom;

#include "CsMidiDevice.h"

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class CsMidiManagerCallback
{
public:
	virtual void processMidiMessage(CsMidiMessage midiMessage) = 0;
	virtual void processTempoMessage(double timestamp, float tempo) { }
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class CsMidiManager : public AThread
{
public:
	CsMidiManager(CsMidiManagerCallback *callback);
	~CsMidiManager();

	int init(int numNetworkMidiInputs = 8);
	int exit(void);

	int numDevices(void) const { return m_midiDevices.size(); }
	CsMidiDevice* device(int index) const { return m_midiDevices[index]; }

	void processMidiEvent(CsMidiMessage midiMessage);
	void sendMidiEvent (CsMidiMessage midiMessage);

	int numPorts(int deviceIndex, CsMidiDevice::CsMidiDevicePortType type) const;
	AString portName(int deviceIndex, int portIndex, CsMidiDevice::CsMidiDevicePortType type) const;
	bool isPortActive(int deviceIndex, int portIndex, CsMidiDevice::CsMidiDevicePortType type) const;
	AStatus setPortActive(int deviceIndex, int portIndex, CsMidiDevice::CsMidiDevicePortType type, bool active);

	CsMidiManagerCallback *callback(void) { return m_callback; }

protected:
	void threadProcess(void);

private:
	CsMidiManagerCallback *m_callback;
	AList<CsMidiDevice*> m_midiDevices;
	AWaitEvent *m_waitForMessage;
	ARingBuffer<CsMidiMessage> *m_midiMessages;
	ACriticalSection m_csMessage;
};

}

#endif