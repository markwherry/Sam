#ifndef __CsNetworkMidiDevice_h__
#define __CsNetworkMidiDevice_h__

#include "CsMidiDevice.h"
#include "CsMidiMessage.h"

#include "AThread.h"
#include "AUdpSocket.h"
#include "ARingBuffer.h"
using namespace Atom;

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*class CsNetworkMidiInputThread : public AThread
{
public:
	CsNetworkMidiInputThread(CsMidiManager *midiManager, int deviceIndex, int portIndex);
	~CsNetworkMidiInputThread();

	void handleBuffer(char *buffer);

protected:
	void threadProcess(void);

private:
	CsMidiManager *m_midiManager;
	int m_deviceIndex;
	int m_portIndex;
	AWaitEvent *m_waitForMessage;
	ARingBuffer<CsMidiMessage> *m_midiMessages;
};*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class CsNetworkMidiDevice : public CsMidiDevice, public AThread
{
public:

	CsNetworkMidiDevice(CsMidiManager *midiManager, int deviceIndex, char *midiIpAddress = 0, int numPorts = 8);
	~CsNetworkMidiDevice();

	int init(void);
	int exit(void);

	int start(bool manuallyActivatePorts = false);
	int stop(void);

	void sendMidiEvent(CsMidiMessage midiMessage);

	int numPorts(CsMidiDevicePortType type) const;
	AString portName(int portIndex, CsMidiDevicePortType type) const;
	bool isPortActive(int portIndex, CsMidiDevicePortType type) const;
	AStatus setPortActive(int portIndex, CsMidiDevicePortType type, bool active);

protected:
	void threadProcess(void);

private:
	char *m_midiIpAddress;
	AUdpSocket udpSocket;
	int m_numPorts;
	//CsNetworkMidiInputThread *m_inputThreads[4];
};

}

#endif // __CsNetworkMidiDevice_h__