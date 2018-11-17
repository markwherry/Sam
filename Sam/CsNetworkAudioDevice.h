#ifndef __CsNetworkAudioDevice_h__
#define __CsNetworkAudioDevice_h__

#include "CsAudioDevice.h"

#include "AThread.h"
#include "APlatform.h"
#include "AUdpSocket.h"
#include "APrecisionTimer.h"
using namespace Atom;

namespace Cs {

class CsNetworkAudioDevice : public CsAudioDevice, public AThread
{
public:
	CsNetworkAudioDevice(CsAudioManager *audioManager);

	AStatus init(void);
	AStatus exit(void);

	AStatus open(AString driverName);
	AStatus close(void);

	int start(void);
	int stop(void);

	AString channelName(int channel, bool isInput);

	virtual int bufferSize(void) { return m_bufferSize; }

	double sampleRate(void);
	AStatus setSampleRate(double sampleRate);

	int numInputs(void) { return m_numInputs; }
	int numOutputs(void) { return m_numOutputs; }

	AString currentAudioDriverName(void) { return "Network Audio"; }

protected:
	void threadProcess(void);

private:
	AUdpSocket *m_udpSocket;
	char *m_sendBuffer;
	int m_sendBufferSize;
	const static int headerSize = 24;

	int m_bufferSize;
	int m_numInputs;
	int m_numOutputs;

	APrecisionTimer m_timer;
};

}

#endif