#ifndef __CsWindowsMmeMidiDevice_h__
#define __CsWindowsMmeMidiDevice_h__

#include "CsMidiDevice.h"

#include "APlatform.h"
#include "AThread.h"
#include "AWaitEvent.h"
#include "ARingBuffer.h"
using namespace Atom;

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------
class CsWindowsMmeMidiInputThread : public AThread
{
public:
	CsWindowsMmeMidiInputThread(CsMidiManager *midiManager, int deviceIndex, int portIndex);
	~CsWindowsMmeMidiInputThread();

	//void start(void);
	//void stop(void);

	void handleData(DWORD dwParam1, DWORD dwParam2);

	void setStartTime(DWORD startTime) { m_startTime = startTime; }

protected:
	void threadProcess(void);

private:
	CsMidiManager *m_midiManager;
	int m_deviceIndex;
	int m_portIndex;
	DWORD m_startTime;
	AWaitEvent *m_waitForMessage;
	ARingBuffer<CsMidiMessage> *m_midiMessages;
};

// ------------------------------------------------------------------------------------------------------------------------
class CsWindowsMmeMidiDevice : public CsMidiDevice
{
public:
	CsWindowsMmeMidiDevice(CsMidiManager *midiManager, int deviceIndex);

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
private:
	static void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	static void CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
	void debugMidiResult(MMRESULT result);

	HMIDIOUT hMidiOut[64];
	HMIDIIN hMidiIn[64];
	HANDLE hTimerQueue;

	AThread *m_inputThreads[64];
	
	
};

}

#endif