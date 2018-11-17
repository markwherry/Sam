#ifndef __CsDummyAudioDevice_h__
#define __CsDummyAudioDevice_h__

#include "CsAudioDevice.h"

#include "AThread.h"
#include "APlatform.h"
using namespace Atom;

namespace Cs {

class CsDummyAudioDevice : public CsAudioDevice, public AThread
{
public:
	CsDummyAudioDevice(CsAudioManager *audioManager);
	
	void threadProcess(void);

	int start(void);
	int stop(void);

	AString channelName(int channel, bool isInput);

	virtual int bufferSize(void) { return m_bufferSize; }

	double sampleRate(void);
	AStatus setSampleRate(double sampleRate);

	int numInputs(void) { return m_numInputs; }
	int numOutputs(void) { return m_numOutputs; }

	AString currentAudioDriverName(void) { return "No Audio Driver"; }

protected:
private:
	bool m_runDriver;
	int m_bufferSize;
	int m_numInputs;
	int m_numOutputs;
};

}

#endif // __ADummyAudioDevice_h__