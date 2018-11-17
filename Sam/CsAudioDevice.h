#ifndef __CsAudioDevice_h__
#define __CsAudioDevice_h__

#include "CsDevice.h"
#include "CsAudioBuffer.h"

#include "AList.h"
using namespace Atom;

#include "ATypes.h"

namespace Cs {

class CsAudioManager;

class CsAudioDevice : public CsDevice
{
friend CsAudioManager;

public:
	CsAudioDevice(CsAudioManager *audioManager);

	virtual AStatus init(void);
	virtual AStatus exit(void);

	virtual AStatus open(AString driverName);
	virtual AStatus close(void);

	virtual AStatus start(void);
	virtual AStatus stop(void);

	virtual AString currentAudioDriverName(void) { return ""; }
	virtual int numAudioDrivers(void) { return 0; }
	virtual AString driverName(int index) { return ""; }

	virtual int bufferSize(void) = 0;

	virtual double sampleRate(void) = 0;
	virtual int setSampleRate(double sampleRate) = 0;

	virtual int numInputs(void) = 0;
	virtual int numOutputs(void) = 0;

	virtual AString channelName(int channel, bool isInput) { return AString(""); }

	float lastBufferDuration(void) { return m_lastBufferDuration; }
	float lastBufferUsage(void) { return m_lastBufferUsage; }
	float bufferUsagePeak(void) { return m_bufferUsagePeak; }
	void resetBufferUsagePeak(void) { m_bufferUsagePeak = 0.0f; }

protected:
	CsAudioManager *audioManager(void) { return m_audioManager; }
	virtual void buildDriverList(void) { }
	virtual void clearDriverList(void) { }

	void updateAudioProcessConfiguration(void);

	AList<double> m_sampleRates;

	CsAudioBuffer *m_inputBuffer;
	CsAudioBuffer *m_outputBuffer;

	float m_lastBufferDuration;
	float m_lastBufferUsage;
	float m_bufferUsagePeak;

	

private:
	CsAudioManager *m_audioManager;
};

}

#endif