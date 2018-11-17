#include "CsDummyAudioDevice.h"

#include "CsAudioManager.h"
#include "CsTypes.h"

#include "ALog.h"

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsDummyAudioDevice::CsDummyAudioDevice(CsAudioManager *audioManager)
: CsAudioDevice(audioManager)
, m_runDriver(false)
{
	timeBeginPeriod(1);

	m_inputBuffer = new CsAudioBuffer(2, 256);
	m_outputBuffer = new CsAudioBuffer(2, 256);
	m_bufferSize = 256;
	m_numInputs = 4;
	m_numOutputs = 4;

	m_deviceInfo.name = AString("No Audio Device");
	m_deviceInfo.uuid = AString("940D8741-601F-404e-AC36-A61C40EF73E9");

	m_sampleRates.clear();
	const double fs[] = { 44100.0 }; //, 48000.0, 88200.0, 96000.0, 176400.0, 192000.0 };
	for(int i=0; i<sizeof(fs)/sizeof(double); i++) {
		m_sampleRates.append(fs[i]);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsDummyAudioDevice::threadProcess(void)
{
	CsSampleTime samples = 0;

	aLog("Dummy Audio Device starting.");
	while(m_runDriver) {
		CsNanoTime nanos = (CsNanoTime)timeGetTime() * 600000.0f;
		audioManager()->callback()->processAudio(m_inputBuffer, m_outputBuffer, nanos, samples);
		audioManager()->callback()->postProcessAudio(nanos, samples);
		samples += 256;
		Sleep(6);
	}
	aLog("Dummy Audio Device stopping.");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsDummyAudioDevice::start(void)
{
	aLog("CsDummyAudioDevice::start()");

	CsAudioDevice::start();

	m_runDriver = true;
	startThread();

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsDummyAudioDevice::stop(void)
{
	CsAudioDevice::stop();

	m_runDriver = false;

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString CsDummyAudioDevice::channelName(int channel, bool isInput)
{
	if(isInput) {
		return AString("Dummy Input");
	}

	return AString("Dummy Output");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
double CsDummyAudioDevice::sampleRate(void)
{
	return 44100.0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsDummyAudioDevice::setSampleRate(double sampleRate)
{
	if(sampleRate == 44100.0) {
		return AOk;
	}

	return AError;
}

}