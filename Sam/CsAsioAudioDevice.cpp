#include "CsAsioAudioDevice.h"
#include "CsAudioManager.h"
#include "ALog.h"

namespace Cs {

// ----------------------------------------------------------------------------------------------------
CsAsioAudioDevice::CsAsioAudioDevice(CsAudioManager *audioManager)
: CsAudioDevice(audioManager)
, m_resetFlag(false)
, m_audioDriverId(-1)
{
	m_deviceInfo.name = AString("ASIO");
	m_deviceInfo.uuid = AString("7212D7D9-63E8-4ef4-BBAD-EB43DC5AD52A");

	currentAsioDevice = this;

	asioInputBuffers = 0;
	asioOutputBuffers = 0;
	asioBufferPreferredSize = 0;

	hEventProcess = CreateEvent(NULL , false , false , NULL);

	startThread();
	setPriority(TimeCritical);

	prevNanoseconds = 0;
}

// ----------------------------------------------------------------------------------------------------
AStatus CsAsioAudioDevice::open(AString driverName)
{
	aLog("CsAsioAudioDevice::open(%s)", driverName.toChar());

	m_audioDriverId = -1;
	for(int i=0; i<asioDrivers.size(); i++) {
		if(driverName == asioDrivers[i].driverName) {
			m_audioDriverId = i;
		}
	}

	if (m_audioDriverId < 0 || m_audioDriverId >= asioDrivers.size()) {
		aLog("audioDriverId(%d) is not valid. (0-%d)", m_audioDriverId, asioDrivers.size()-1);
		return AError;
	}

	CoInitialize(0);

	long ret = CoCreateInstance(asioDrivers[m_audioDriverId].clsid,0,CLSCTX_INPROC_SERVER,asioDrivers[m_audioDriverId].clsid,(void **)&theAsioDriver);
	if (ret == S_OK) {
		aLog("CoCreateInstance successful.");
	} else {
		aLog("CoCreateInstance failed.");
		switch (ret)
		{
		case REGDB_E_CLASSNOTREG:
			aLog("REGDB_E_CLASSNOTREG");
			// A specified class is not registered in the registration database.
			// Also can indicate that the type of server you requested in the CLSCTX
			// enumeration is not registered or the values for the server types in the
			// registry are corrupt.
			break;
		case CLASS_E_NOAGGREGATION:
			aLog("CLASS_E_NOAGGREGATION");
			// This class cannot be created as part of an aggregate.
			break;
		case E_NOINTERFACE:
			aLog("E_NOINTERFACE");
			// The specified class does not implement the requested interface, or the
			// controlling IUnknown does not expose the requested interface. 
			break;
		}
		return AError;
	}

	ret = initAsioDriver();
	if (ret<1) {
		aLog("ASIO initDriver failed.");
		return ret;
	}

	return CsAudioDevice::open(driverName);
}

// ----------------------------------------------------------------------------------------------------
int CsAsioAudioDevice::close(void)
{
	aLog("CsAsioAudioDevice::close()");
	if(asioInputBuffers+asioOutputBuffers > 0) {
		long ret = theAsioDriver->disposeBuffers();
		aLog("ASIODisposeBuffers=%ld",ret);
		asioInputBuffers = asioOutputBuffers = 0;
	}

	if(theAsioDriver) {
		theAsioDriver->Release();
		theAsioDriver = 0;
		m_audioDriverId = -1;
	}

	return CsAudioDevice::close();
}

// ----------------------------------------------------------------------------------------------------
int CsAsioAudioDevice::start()
{
	aLog("CsAsioAudioDevice::start()");

	CsAudioDevice::start();

	if (theAsioDriver->start() != ASE_OK) {
		aLog("The ASIO Driver failed to start.");
		return 0;
	}

	aLog("ASIO Driver started OK");
			
	aLog("The ASIO Driver started successfully.");

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int CsAsioAudioDevice::stop(void)
{
	if(!isActive()) {
		return 1;
	}

	aLog("CsAsioAudioDevice::stop()");
	long ret = theAsioDriver->stop();
	aLog("ASIOStop=%ld", ret);
	
	return CsAudioDevice::stop();
}

// ----------------------------------------------------------------------------------------------------
void CsAsioAudioDevice::threadProcess(void)
{
	int dotimer = 0;

	for (;;) {
		WaitForSingleObject(hEventProcess, INFINITE);

		if(m_resetFlag) {
			m_resetFlag = false;
			audioManager()->switchDeviceDriver(audioManager()->currentAudioDeviceDriver());
		} else {
			if(dotimer++ > 10) {
				m_timer.start();
				process(bufferIndex, nanoseconds, samples);
				m_timer.stop();
				m_lastBufferDuration = m_timer.duration();
				float bufferTime = bufferSize()/sampleRate();
				m_lastBufferUsage = m_lastBufferDuration/bufferTime;
				if(m_lastBufferUsage > m_bufferUsagePeak) {
					m_bufferUsagePeak = m_lastBufferUsage;
				}
				dotimer = 0;
			} else {
				process(bufferIndex, nanoseconds, samples);
			}
		}
	}
}

// ----------------------------------------------------------------------------------------------------
void CsAsioAudioDevice::process(long index, double timestamp, double samples)
{
	//aLog("CsAsioAudioDevice::process()");

//	CsPrecisionTimer precisionTimer;
//	precisionTimer.start();

	long bufferSize = currentAsioDevice->asioBufferPreferredSize;
	int activeChannels = currentAsioDevice->asioOutputChannels;//16;

	int maxChannels;
	if (activeChannels<currentAsioDevice->asioOutputChannels)
		maxChannels = activeChannels;
	else
		maxChannels = currentAsioDevice->asioOutputChannels;

	m_outputBuffer->zeroAll();

	//currentAsioDevice->audioDeviceCallback->processAudio(m_inputBuffer, m_outputBuffer, timestamp, samples);
	audioManager()->callback()->processAudio(m_inputBuffer, m_outputBuffer, timestamp, samples);

	for (int i = 0; i < currentAsioDevice->asioInputBuffers + currentAsioDevice->asioOutputBuffers; i++) {
		//aLog ("i=%d\n ",i);
		if (asioBufferInfos[i].isInput == false)
		{
			//aLog (L"OUTPUT\n");
			// OK do processing for the outputs only
			//aLog("currentASIODevice->asioChannelInfos[i].type = %d", currentASIODevice->asioChannelInfos[i].type);
			switch (currentAsioDevice->asioChannelInfos[i].type) {
				case ASIOSTInt32LSB: // The is what the FireFace needs
				//aLog ("ASIOSTInt32LSB [%d]\n", i);

				//if (i==28) convertFloat32ToInt32 (outputBuffer[0], asioBufferInfos[i].buffers[index], bufferSize);
				//if (i==29) convertFloat32ToInt32 (outputBuffer[1], asioBufferInfos[i].buffers[index], bufferSize);

					if (i<currentAsioDevice->asioInputChannels) { // i is an input
					} else { // i is an output
						int channel = i-currentAsioDevice->asioInputChannels;
						//aLog("channel=%d, maxChannels=%d", channel, maxChannels);
						if (channel>=0 && channel < maxChannels) {
							//convertFloat32ToInt32 (outputBuffer[channel], asioBufferInfos[i].buffers[index], bufferSize);
							float *buffer = m_outputBuffer->stream(channel);
							//for(int x=0; x<bufferSize; x++) {
							//	if(buffer[x]!=0.0f) aLog("%f", buffer[x]);
							//}
							
							memset(asioBufferInfos[i].buffers[index], 0, bufferSize*4);
							convertFloat32ToInt32(buffer, asioBufferInfos[i].buffers[index], bufferSize);
							
							//aLog("Here!");
						}
					}

					break;
			}
		}
	}

	// Signal the driver that the buffers are ready
	theAsioDriver->outputReady();

//	precisionTimer.stop();
//	m_lastBufferDuration = precisionTimer.duration();

	audioManager()->callback()->postProcessAudio(timestamp, samples);
}

// ----------------------------------------------------------------------------------------------------
AString CsAsioAudioDevice::channelName(int channel, bool isInput)
{
	ASIOChannelInfo info;
	info.channel = channel;
	info.isInput = isInput;

	if(theAsioDriver->getChannelInfo(&info) == ASE_OK) {
		return AString(info.name);
	}

	return AString("ASIO Channel");
}

// ----------------------------------------------------------------------------------------------------
double CsAsioAudioDevice::sampleRate(void)
{
	theAsioDriver->getSampleRate(&asioSampleRate);

	return asioSampleRate;
}

// ----------------------------------------------------------------------------------------------------
int CsAsioAudioDevice::setSampleRate(double sampleRate)
{
	//aLog("CsAsioAudioDevice::setSampleRate(%f)", sampleRate);

	if(theAsioDriver->canSampleRate(sampleRate) != ASE_OK) {
		aLog("CsAsioAudioDevice::setSampleRate() %f not supported", sampleRate);
		return 0;
	}

	if (theAsioDriver->setSampleRate(sampleRate) != ASE_OK) {
		aLog("CsAsioAudioDevice::setSampleRate() %f could not be set", sampleRate);
		return 0;
	}

	updateAudioProcessConfiguration();

	return 1;
}



// ----------------------------------------------------------------------------------------------------
void CsAsioAudioDevice::buildDriverList(void)
{
	clearDriverList();

	int ret;
	int returnState = 0;
	char name[256];

	HKEY hKey;

	// Find the ASIO registry key
	ret = RegOpenKeyA (HKEY_LOCAL_MACHINE, "software\\asio", &hKey);
	if (ret == ERROR_SUCCESS) {
		aLog ("Key Open (1)\n");
		int index = 0;
		// Enumerate the ASIO drivers stored in the registry
		while (ret == ERROR_SUCCESS) {
			
			ret = RegEnumKeyA (hKey, index++, name, 256);
			if (ret == ERROR_SUCCESS) {
				aLog ("[%s]","test");
				aLog ("Driver[%d] = '%s'\n", index-1, name);
				AsioDriverData tempAsioDriver;
				strcpy (tempAsioDriver.driverName, name);
				if (getDriverData (hKey, name, &tempAsioDriver)) {
					//aLog (L"----------->ASIODriver.push_back");
					asioDrivers.append(tempAsioDriver);
					returnState = 1;
				}
			}
		}
		RegCloseKey (hKey);
	}
	else aLog ("Fallen at first hurdle!");

	//return returnState;
}

// ----------------------------------------------------------------------------------------------------
void CsAsioAudioDevice::clearDriverList(void)
{
	asioDrivers.clear();
}

// ----------------------------------------------------------------------------------------------------
int CsAsioAudioDevice::initAsioDriver(void)
{
	// ASIOInit ()
	// -----------
	asioDriverInfo.asioVersion = 2;
	asioDriverInfo.sysRef = 0;
	if (!theAsioDriver->init(&asioDriverInfo)) {
		aLog("ASIO Init Failed.");
		return 0;
	}
	aLog ("ASIO Init OK.");


	// Get driver settings
	// -------------------
	theAsioDriver->getDriverName(asioDriverInfo.name);
	aLog("asioDriverInfo.name = %s", asioDriverInfo.name);

	theAsioDriver->getChannels (&asioInputChannels, &asioOutputChannels);
	aLog("I/O = %ld/%ld", asioInputChannels, asioOutputChannels);

	// Buffer Size
	theAsioDriver->getBufferSize (&asioBufferMinSize, &asioBufferMaxSize, &asioBufferPreferredSize, &asioBufferGranularity);
	aLog("asioBufferMinSize=%ld", asioBufferMinSize);
	aLog("asioBufferMaxSize=%ld", asioBufferMaxSize);
	aLog("asioBufferPreferredSize=%ld", asioBufferPreferredSize);
	aLog("asioBufferGranularity=%ld", asioBufferGranularity);


	//m_bufferSize = asioBufferPreferredSize; // Do we need this??? We can get this information based off the buffers once initialised.

	// Build a list containing the supported sample rates
	m_sampleRates.clear();
	const double fs[] = { 44100.0, 48000.0, 88200.0, 96000.0, 176400.0, 192000.0 };
	for(int i=0; i<sizeof(fs)/sizeof(double); i++) {
		if(theAsioDriver->canSampleRate(fs[i]) == ASE_OK) {
			m_sampleRates.append(fs[i]);
			aLog("ASIO can support %f", fs[i]);
		} else {
			aLog("ASIO cannot support %f", fs[i]);
		}
	}

	// If the current sample rate is outside of the supported range, try setting to 44.1, otherwise return an error.
	theAsioDriver->getSampleRate(&asioSampleRate);
	aLog("asioSampleRate = %f", asioSampleRate);
	
	
	if(asioSampleRate < 44100.0 || asioSampleRate > 192000.0) {

		if(theAsioDriver->canSampleRate(44100.0) != ASE_OK) {
			aLog("44100.0 not supported");
			return 0;
		}

		if (theAsioDriver->setSampleRate(44100.0) != ASE_OK) {
			aLog("44100.0 could not be set");
			return 0;
		}
		aLog ("Sample rate set to 44.1kHz");

		//m_sampleRate = 44100.0;
	} /*else {
		m_sampleRate = asioSampleRate;
	}*/

	//m_nanosPerSample = (double)m_sampleRate/1000000000.0;

	// Clock sources
	long numClockSources = 32;
	ASIOClockSource clockSources[32];
	theAsioDriver->getClockSources(clockSources, &numClockSources);

	aLog("%d clock sources", numClockSources);
	for(int i=0; i<numClockSources; i++) {
		aLog("%s", clockSources[i].name);
		aLog("isCurrentSource (%d)", (int)clockSources[i].isCurrentSource);
	}

	if (theAsioDriver->outputReady() == 0) {
		aLog("ASIO postOutput supported.");
		asioPostOutput = true;
	}
	else {
		aLog("ASIO postOutput not supported.");
		asioPostOutput = false;
	}

	

	// Set-up the callbacks
	// --------------------
	asioCallbacks.bufferSwitch = &bufferSwitch;
	asioCallbacks.sampleRateDidChange = &sampleRateChanged;
	asioCallbacks.asioMessage = &asioMessages;
	asioCallbacks.bufferSwitchTimeInfo = &bufferSwitchTimeInfo;

		
	// Create buffers
	// --------------
	ASIOBufferInfo *info = asioBufferInfos;

	// prepare inputs (Though this is not necessaily required, no opened inputs will work, too
	if (asioInputChannels > CsAudioBuffer::maxStreams)
		asioInputBuffers = CsAudioBuffer::maxStreams;
	else
		asioInputBuffers = asioInputChannels;
	for(long i = 0; i < asioInputBuffers; i++, info++)
	{
		info->isInput = ASIOTrue;
		info->channelNum = i;
		info->buffers[0] = info->buffers[1] = 0;
	}

	// prepare outputs
	if (asioOutputChannels > CsAudioBuffer::maxStreams)
		asioOutputBuffers = CsAudioBuffer::maxStreams;
	else
		asioOutputBuffers = asioOutputChannels;
	for(long i = 0; i < asioOutputBuffers; i++, info++)
	{
		info->isInput = ASIOFalse;
		info->channelNum = i;
		info->buffers[0] = info->buffers[1] = 0;
	}

	// create and activate buffers
	if (theAsioDriver->createBuffers(asioBufferInfos, asioInputBuffers + asioOutputBuffers, asioBufferPreferredSize, &asioCallbacks) != ASE_OK) {
		aLog("ASIO CreateBuffers failed.");
		return 0;
	}
	aLog("ASIO CreateBuffers OK.");

	// now get all the buffer details, sample word length, name, word clock group and activation
	for(long i=0; i<asioInputBuffers + asioOutputBuffers; i++) {
		asioChannelInfos[i].channel = asioBufferInfos[i].channelNum;
		asioChannelInfos[i].isInput = asioBufferInfos[i].isInput;
		if (theAsioDriver->getChannelInfo(&asioChannelInfos[i]) != ASE_OK) {
			aLog("ASIO GetChannelInfo failed.");
			return 0;
		}
		aLog("ASIO GetChannelInfo OK [%ld].",i);
	}

	// get the input and output latencies
	// Latencies often are only valid after ASIOCreateBuffers()
	// (input latency is the age of the first sample in the currently returned audio block)
	// (output latency is the time the first sample in the currently returned audio block requires to get to the output)
	if (theAsioDriver->getLatencies(&asioInputLatency, &asioOutputLatency) != ASE_OK) {
		aLog("ASIO GetLatencies failed.");
		return 0;
	}
	aLog("ASIOGetLatencies (input: %d, output: %d)", asioInputLatency, asioOutputLatency);
	
	if(m_inputBuffer!=0) {
		delete m_inputBuffer;
	}
	if(m_outputBuffer!=0) {
		delete m_outputBuffer;
	}
	m_inputBuffer  = new CsAudioBuffer(asioInputBuffers, asioBufferPreferredSize);
	m_outputBuffer = new CsAudioBuffer(asioOutputBuffers, asioBufferPreferredSize);
	
	aLog("Exit: CsAsioAudioDevice::initAsioDriver()");

	return 1;
}

// ----------------------------------------------------------------------------------------------------
void CsAsioAudioDevice::bufferSwitch(long index, ASIOBool processNow)
{
	aLog("CsAsioAudioDevice::bufferSwitch ()");
	// the actual processing callback.
	// Beware that this is normally in a seperate thread, hence be sure that you take care
	// about thread synchronization. This is omitted here for simplicity.

	// as this is a "back door" into the bufferSwitchTimeInfo a timeInfo needs to be created
	// though it will only set the timeInfo.samplePosition and timeInfo.systemTime fields and the according flags
	ASIOTime  timeInfo;
	memset(&timeInfo, 0, sizeof (timeInfo));

	// get the time stamp of the buffer, not necessary if no
	// synchronization to other media is required
	//if(ASIOGetSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == ASE_OK)
	//	timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

	bufferSwitchTimeInfo(&timeInfo, index, processNow);
}

// ----------------------------------------------------------------------------------------------------
ASIOTime* CsAsioAudioDevice::bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow)
{
	// perform the processing
	currentAsioDevice->nanoseconds = ASIO64toDouble(timeInfo->timeInfo.systemTime);
	currentAsioDevice->samples = ASIO64toDouble(timeInfo->timeInfo.samplePosition);
	currentAsioDevice->tcSamples = ASIO64toDouble(timeInfo->timeCode.timeCodeSamples);

	

	currentAsioDevice->bufferIndex = index;


	
	SetEvent(currentAsioDevice->hEventProcess);
	
	/*currentAsioDevice->m_timer.start();
	currentAsioDevice->process(currentAsioDevice->bufferIndex, currentAsioDevice->nanoseconds, currentAsioDevice->samples);
	currentAsioDevice->m_timer.stop();
	currentAsioDevice->m_lastBufferDuration = currentAsioDevice->m_timer.duration();
	float bufferTime = currentAsioDevice->bufferSize()/currentAsioDevice->sampleRate();
	currentAsioDevice->m_lastBufferUsage = currentAsioDevice->m_lastBufferDuration/bufferTime;
	if(currentAsioDevice->m_lastBufferUsage > currentAsioDevice->m_bufferUsagePeak) {
		currentAsioDevice->m_bufferUsagePeak = currentAsioDevice->m_lastBufferUsage;
	}//*/


	currentAsioDevice->prevNanoseconds = currentAsioDevice->nanoseconds;
	return 0;
}

// ----------------------------------------------------------------------------------------------------
void CsAsioAudioDevice::sampleRateChanged(ASIOSampleRate sRate)
{
	aLog("CsAsioAudioDevice::sampleRateChanged(%f)", sRate);

	// Need to test whether this is the right thing to do...
	currentAsioDevice->updateAudioProcessConfiguration();
}

// ----------------------------------------------------------------------------------------------------
long CsAsioAudioDevice::asioMessages (long selector, long value, void* message, double* opt)
{
	aLog ("---------------->asioMessages (%ld)", selector);
	// currently the parameters "value", "message" and "opt" are not used.
	long ret = 0;
	switch(selector) {
		case kAsioSelectorSupported:
			if(value == kAsioResetRequest
			|| value == kAsioEngineVersion
			|| value == kAsioResyncRequest
			|| value == kAsioLatenciesChanged
			// the following three were added for ASIO 2.0, you don't necessarily have to support them
			|| value == kAsioSupportsTimeInfo
			|| value == kAsioSupportsTimeCode
			|| value == kAsioSupportsInputMonitor)
				ret = 1L;
			break;
		case kAsioResetRequest:
			aLog ("kAsioResetRequest");
			// defer the task and perform the reset of the driver during the next "safe" situation
			// You cannot reset the driver right now, as this code is called from the driver.
			// Reset the driver is done by completely destruct is. I.e. ASIOStop(), ASIODisposeBuffers(), Destruction
			// Afterwards you initialize the driver again.


			
			currentAsioDevice->m_resetFlag = true;
			SetEvent (currentAsioDevice->hEventProcess);
			

			ret = 1L;
			break;
		case kAsioResyncRequest:
			aLog ("kAsioResyncRequest");
			// This informs the application, that the driver encountered some non fatal data loss.
			// It is used for synchronization purposes of different media.
			// Added mainly to work around the Win16Mutex problems in Windows 95/98 with the
			// Windows Multimedia system, which could loose data because the Mutex was hold too long
			// by another thread.
			// However a driver can issue it in other situations, too.
			ret = 1L;
			break;
		case kAsioLatenciesChanged:
			aLog ("kAsioLatenciesChanged");
			// This will inform the host application that the drivers were latencies changed.
			// Beware, it this does not mean that the buffer sizes have changed!
			// You might need to update internal delay data.
			ret = 1L;
			break;
		case kAsioEngineVersion:
			//aLog (L"kAsioEngineVersion\n");
			// return the supported ASIO version of the host application
			// If a host applications does not implement this selector, ASIO 1.0 is assumed
			// by the driver
			ret = 2L;
			break;
		case kAsioSupportsTimeInfo:
			//aLog (L"kAsioSupportsTimeInfo\n");
			// informs the driver wether the asioCallbacks.bufferSwitchTimeInfo() callback
			// is supported.
			// For compatibility with ASIO 1.0 drivers the host application should always support
			// the "old" bufferSwitch method, too.
			ret = 1;
			break;
		case kAsioSupportsTimeCode:
			//aLog (L"kAsioSupportsTimeCode\n");
			// informs the driver wether application is interested in time code info.
			// If an application does not need to know about time code, the driver has less work
			// to do.
			ret = 0;
			break;
	}
	aLog("EXIT");
	return ret;
}

// ----------------------------------------------------------------------------------------------------
int CsAsioAudioDevice::getDriverData(HKEY hKey, char *name, AsioDriverData *tempAsioDriver)
{
	aLog("CsAsioAudioDevice::GetDriverData ()");

	int returnState = 0;

	HKEY hKeyValue;

	DWORD dataType = REG_SZ;
	char dataBuffer[256];
	DWORD dataSize = 256;

	CLSID clsid;
	WORD wData[100];

	if (RegOpenKeyA(hKey, name, &hKeyValue) == ERROR_SUCCESS) {
	
		if (RegQueryValueExA (hKeyValue, "clsid", 0, &dataType, (LPBYTE)dataBuffer, &dataSize) == ERROR_SUCCESS) {
			aLog ("clsid = %s", dataBuffer);
			MultiByteToWideChar(CP_ACP,0,(LPCSTR)dataBuffer,-1,(LPWSTR)wData,100);
			if (CLSIDFromString((LPOLESTR)wData,(LPCLSID)&clsid) == S_OK) {
				memcpy(&tempAsioDriver->clsid,&clsid,sizeof(CLSID));
			}
			
			if (getDriverPath (dataBuffer, tempAsioDriver)) {
				returnState = 1;
			}

		}

		RegCloseKey (hKeyValue);

	}
	else
	{
		aLog ("RegOpenKeyA (1) Fail.");
	}

	return returnState;
}

// ----------------------------------------------------------------------------------------------------
int CsAsioAudioDevice::getDriverPath(char *clsidBuffer, AsioDriverData *tempAsioDriver)
{
	aLog("GetDriverPath ()");

	int ret;
	int returnState = 0;

	HKEY hKeyEnum;
	HKEY hKeySub;
	HKEY hKeyPath;

	DWORD dataType = REG_SZ;
	char dataBuffer[256];
	DWORD dataSize = 256;

	char dllPath[128];
	DWORD dllPathSize = 128;

	ret = RegOpenKeyA(HKEY_CLASSES_ROOT, "clsid", &hKeyEnum);
	if (ret == ERROR_SUCCESS) {
		//aLog (L"Key 3 Open.\n");
		bool found = false;
		int index = 0;
		
		while (ret == ERROR_SUCCESS && !found) {
			//aLog (L"Here!\n");
			ret = RegEnumKeyA(hKeyEnum,index++,dataBuffer,512);
			if (ret == ERROR_SUCCESS){
				//aLog (L"ERROR_SUCCESS\n");
				//aLog (L"Here!\n");
				CharLowerBuffW((LPWSTR)dataBuffer,strlen(dataBuffer));
				//aLog (L"db='%s' db2='%s'\n", databuf, databuf2);
				if (!(strcmp(dataBuffer,clsidBuffer))) {
					//aLog (L"Do we ever get here?\n");
					if ((ret = RegOpenKeyExA(hKeyEnum,dataBuffer,0,KEY_READ,&hKeySub)) == ERROR_SUCCESS) {
						if ((ret = RegOpenKeyExA(hKeySub,"InprocServer32",0,KEY_READ,&hKeyPath)) == ERROR_SUCCESS) {
							
							ret = RegQueryValueExA(hKeyPath,0,0,&dataType,(LPBYTE)dllPath,&dllPathSize);
							if (ret == ERROR_SUCCESS) {
								found = true;
								aLog ("dllPath = %s", dllPath);
								strcpy (tempAsioDriver->dllPath, dllPath);
								returnState = 1;
							}

							RegCloseKey (hKeyPath);
						}
						RegCloseKey (hKeySub);
					}
				} else {
					//aLog ("String compare failure.");
				}
			}
			else {
				aLog ("FAIL!");
			}

		}

		RegCloseKey (hKeyEnum);
	}
	else {
		aLog ("Error Opening Key(3)");
	}

	return returnState;
}

AString CsAsioAudioDevice::currentAudioDriverName(void)
{
	if(theAsioDriver) {
		//return asioDriverInfo.name;
		return driverName(m_audioDriverId);
	}

	return "No ASIO Driver";
}

}