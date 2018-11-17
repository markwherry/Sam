#ifndef __CsAsioAudioDevice_h__
#define __CsAsioAudioDevice_h__

#include "CsAudioDevice.h"
#include "AThread.h"
#include "APrecisionTimer.h"
//#include "Platform.h"

#include "Asio/iasiodrv.h"

#include <vector>

namespace Cs {

struct AsioDriverData {
	char driverName[256];
	CLSID clsid;
	char dllPath[128];
};

class CsAsioAudioDevice;

static CsAsioAudioDevice* volatile currentAsioDevice = 0;
static IASIO* volatile theAsioDriver = 0;
static ASIOCallbacks asioCallbacks;

class CsAsioAudioDevice : public CsAudioDevice, public AThread
{
public:
	CsAsioAudioDevice(CsAudioManager *audioManager);

	int numDrivers(void) { return asioDrivers.size(); }
	
	//int init(void);
	//int exit(void);

	AStatus open(AString driverName);
	int close(void);

	int start(void);
	int stop(void);

	void threadProcess(void);
	
	void process(long index, double timestamp, double samples);

	AString channelName(int channel, bool isInput);

	int numAudioDrivers(void) { return asioDrivers.size(); }
	AString driverName(int index) { return AString(asioDrivers[index].driverName); }

	int bufferSize(void) { return asioBufferPreferredSize; }

	double sampleRate(void);
	int setSampleRate(double sampleRate);

	int numInputs(void) { return asioInputChannels; }
	int numOutputs(void) { return asioOutputChannels; }

	AString currentAudioDriverName(void);

protected:
	void buildDriverList(void);
	void clearDriverList(void);

private:
	int initAsioDriver(void);

	static void bufferSwitch(long index, ASIOBool processNow);
	static ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);
	static void sampleRateChanged(ASIOSampleRate sRate);
	static long asioMessages (long selector, long value, void* message, double* opt);

	int getDriverData(HKEY hKey, char *name, AsioDriverData *tempAsioDriver);
	int getDriverPath(char *clsidBuffer, AsioDriverData *tempAsioDriver);

	static void convertFloat32ToInt32(float *source, void *destination, long samples)
	{
		long *dst = (long *)destination;
		double scaler = (double)0x7fffffffL + .49999;
		while (--samples >= 0)
			*dst++ = (long)((double)(*source++)*scaler);
	}

	AList<AsioDriverData> asioDrivers;

	HANDLE hEventProcess;
	volatile long bufferIndex;
	volatile double nanoseconds;
	volatile double prevNanoseconds;
	volatile double samples;
	volatile double tcSamples;

	bool m_resetFlag;

	// ASIOInit ()
	ASIODriverInfo asioDriverInfo;

	// ASIOGetChannels()
	long asioInputChannels;
	long asioOutputChannels;

	// ASIOGetBufferSize()
	long asioBufferMinSize;
	long asioBufferMaxSize;
	long asioBufferPreferredSize;
	long asioBufferGranularity;

	// ASIOGetSampleRate()
	ASIOSampleRate asioSampleRate;

	// ASIOOutputReady()
	bool asioPostOutput;

	// ASIOGetLatencies ()
	long asioInputLatency;
	long asioOutputLatency;

	// ASIOCreateBuffers ()
	long asioInputBuffers;	// becomes number of actual created input buffers
	long asioOutputBuffers;	// becomes number of actual created output buffers
	ASIOBufferInfo asioBufferInfos[CsAudioBuffer::maxStreams * 2]; // buffer info's
	

	// bufferSwitchTimeInfo()
	ASIOTime       asioTimeInfo;			// time info state
	unsigned long  asioSysRefTime;      // system reference time, when bufferSwitch() was called

	// ASIOGetChannelInfo()
	ASIOChannelInfo asioChannelInfos[CsAudioBuffer::maxStreams * 2]; // channel info's
	// The above two arrays share the same indexing, as the data in them are linked together

	APrecisionTimer m_timer;

	int m_audioDriverId;
};

const double twoRaisedTo32 = 4294967296.;
#define ASIO64toDouble(a)  ((a).lo + (a).hi * twoRaisedTo32)

}

#endif // __CsAsioAudioDevice_h__