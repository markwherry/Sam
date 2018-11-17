#ifndef __CsAudioManager_h__
#define __CsAudioManager_h__

#include "CsAudioDevice.h"
#include "CsAudioProcessInfo.h"

#include "AList.h"
#include "ATypes.h"
using namespace Atom;

namespace Cs {

// --------------------------------------------------------------------------------------------------------------------------------------------
class CsAudioManagerCallback
{
public:
	CsAudioManagerCallback()
		: m_inputs(0)
		, m_outputs(0)
	{
	}

	virtual void audioStopped(void) { }
	virtual void processAudio(CsAudioBuffer *inputs, CsAudioBuffer *outputs, double nanos, double samples) = 0;
	virtual void postProcessAudio(double nanos, double samples) { }

	CsAudioProcessInfo audioProcessInfo(void){ return m_audioProcessInfo; }
	virtual void setAudioProcessConfiguration(CsAudioProcessInfo audioProcessInfo, int inputs, int outputs)
	{
		m_audioProcessInfo = audioProcessInfo;
		m_inputs = inputs;
		m_outputs = outputs;
	}

	int numInputs(void) { return m_inputs; }
	int numOutputs(void) { return m_outputs; }

private:
	CsAudioProcessInfo m_audioProcessInfo;
	int m_inputs;
	int m_outputs;
};

// --------------------------------------------------------------------------------------------------------------------------------------------
class CsAudioManager
{
public:
	CsAudioManager(CsAudioManagerCallback *callback);
	~CsAudioManager();

	AStatus init(AString deviceuuid);
	AStatus exit(void);

	AStatus open(AString driverName);
	AStatus close(void);

	AStatus start(void);
	AStatus stop(void);

	int numAudioDevices(void) { return m_audioDevices.size(); }
	CsDeviceInfo* deviceInfo(int index) { return m_audioDevices[index]->deviceInfo(); }
	int currentAudioDeviceIndex(void) { return m_currentAudioDeviceId; }
	AString currentAudioDevice(void);
	
	AString currentAudioDeviceDriver(void);
	AStatus switchDeviceDriver(AString driverName);
	int numDeviceDrivers(void);
	AString deviceDriverName(int index);

	float lastBufferDuration(void);
	float lastBufferUsage(void);
	float bufferUsagePeak(void);
	void resetBufferUsagePeak(void);

	int bufferSize(void);
	double sampleRate(void);



	//int currentAudioDeviceId(void) { return m_currentAudioDeviceId; }
	

	/*int numAudioDevices(void) { return audioDeviceList.size(); }
	AAudioDeviceInfo* audioDeviceInfo(int id);
	
	int numDeviceDrivers(void);
	AString deviceDriverName(int id);
	int switchDeviceDriver(int id);
	int numSupportedSampleRates(void);
	double supportedSampleRate(int index);
	int setSupportSampleRate(int index);
	int supportedSampleRateIndex(double fs);
	int deviceIndexFromuuid(AString uuid);*/

	/*int bufferSize(void);
	double sampleRate(void);
	int numInputs(void);
	int numOutputs(void);
	AString channelName(int id, bool isInput);*/

	AString channelName(int id, bool isInput);

	CsAudioManagerCallback* callback(void) { return m_callback; }

	//bool isDeviceStarted(void);

protected:
	
private:
	CsAudioManagerCallback *m_callback;
	AList<CsAudioDevice*> m_audioDevices;
	int m_currentAudioDeviceId;
};

}

#endif // __AudioManager_h__