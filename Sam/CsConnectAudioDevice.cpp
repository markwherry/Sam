#include "CsConnectAudioDevice.h"

#include "CsAudioManager.h"
#include "CsTypes.h"

#include "ipps.h"

#include "ALog.h"

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsConnectAudioDevice::CsConnectAudioDevice(CsAudioManager *audioManager)
: CsAudioDevice(audioManager)
, m_sendBuffer(0)
, m_sendBufferSize(0)
{
	timeBeginPeriod(1);

	m_bufferSize = 256;
	m_numInputs = 0;
	m_numOutputs = 16;

	m_deviceInfo.name = AString("Connect Audio");
	m_deviceInfo.uuid = AString("0576E879-03A4-4B82-9BB9-0F6EBB5B8D63");

	m_sampleRates.clear();
	const double fs[] = { 44100.0 }; //, 48000.0, 88200.0, 96000.0, 176400.0, 192000.0 };
	for(int i=0; i<sizeof(fs)/sizeof(double); i++) {
		m_sampleRates.append(fs[i]);
	}

	m_socketSend = new AUdpSocket();
	m_socketReceive = new AUdpSocket();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsConnectAudioDevice::init(void)
{
	aLog("CsNetworkAudioDevice::init()");

	CsAudioDevice::init();

	//m_nanosPerSample = (double)44100.0/1000000000.0;

	if(!m_socketSend->open(9932)) {
		aLog("Error opening socket on port 9932.");
		return AError;
	}
	aLog("Socket open on port 9932.");

	if(!m_socketReceive->open(9933)) {
		aLog("Error opening socket on port 9933.");
		return AError;
	}
	aLog("Socket open on port 9932.");

	return AOk;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsConnectAudioDevice::exit(void)
{
	return CsAudioDevice::exit();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsConnectAudioDevice::open(AString driverName)
{
	if(m_inputBuffer) {
		delete m_inputBuffer;
		m_inputBuffer = 0;
	}
	if(m_outputBuffer) {
		delete m_outputBuffer;
		m_outputBuffer = 0;
	}

	if(m_sendBuffer) {
		free(m_sendBuffer);
		m_sendBuffer = 0;
	}

	if(m_numInputs > 0) {
		m_inputBuffer = new CsAudioBuffer(m_numInputs, m_bufferSize);
	}
	
	if(m_numOutputs > 0) {
		m_outputBuffer  = new CsAudioBuffer(m_numOutputs, m_bufferSize);
	}

	m_sendBufferSize = (sizeof(float)*(m_bufferSize*m_numOutputs))+headerSize;
	m_sendBuffer = (char*)malloc(m_sendBufferSize);
	memset(m_sendBuffer, 0, m_sendBufferSize);

	return CsAudioDevice::open(driverName);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsConnectAudioDevice::close(void)
{
	return CsAudioDevice::close();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsConnectAudioDevice::start(void)
{
	aLog("CsNetworkAudioDevice::start()");

	CsAudioDevice::start();

	startThread();
	setPriority(THREAD_PRIORITY_TIME_CRITICAL);

	return AOk;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsConnectAudioDevice::stop(void)
{
	CsAudioDevice::stop();

	stopThread();

	char buffer[headerSize];
	memset(buffer, 0, headerSize);
	m_socketReceive->sendBuffer("127.0.0.1", buffer, headerSize);

	waitForThreadToExit();

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString CsConnectAudioDevice::channelName(int channel, bool isInput)
{
	if(isInput) {
		return AString("Input");
	}

	return AString(AString("Output ")+AString::number(channel));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
double CsConnectAudioDevice::sampleRate(void)
{
	return 44100.0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsConnectAudioDevice::setSampleRate(double sampleRate)
{
	if(sampleRate == 44100.0) {
		return AOk;
	}

	return AError;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsConnectAudioDevice::threadProcess(void)
{
	char buffer[24];
	unsigned long address;
	int dotimer = 0;

	bool resetReceived = false;

	double samples = 0;

	aLog("****** Network Audio Device starting.");
	while(isThreadRunning()) {
		if(m_sendBuffer) {
			//aLog("Waiting for network command...");
			m_socketReceive->receiveBuffer(buffer, headerSize, &address);
			//CSDEBUG("Received buffer. address=[%s]", udpSocket->lastReceivedAddress());
			
			if(buffer[0]==100) { // Reset
				aLog("Reset Received from %s.", m_socketReceive->lastReceivedAddress());
				//audioDeviceCallback->processAudio(inputsBuffer, outputsBuffer, 0, 0);
				//startReceived = true;
				
				short bs;
				AUdpSocket::charToShort(buffer, 2, &bs, 11);
				m_bufferSize = bs;
				aLog("Buffer Size = %d", m_bufferSize);
				
				short fs;
				AUdpSocket::charToShort(buffer, 2, &fs, 13);
				m_numOutputs = fs;
				aLog("Number of outputs = %d", m_numOutputs);

				resetReceived = true;
				//resetAudioDevice(m_driverId, audioDeviceCallback);
				//audioManager()->switchDeviceDriver(audioManager()->currentAudioDeviceDriver());
				open("");

				m_timer.start();

				m_outputBuffer->zeroAll();
				double timestamp = (double)timeGetTime()*1000000.0;
				audioManager()->callback()->processAudio(m_inputBuffer, m_outputBuffer, timestamp, samples);
				audioManager()->callback()->postProcessAudio(timestamp, samples);
				samples += (double)m_bufferSize;

				m_timer.stop();
				m_lastBufferDuration = m_timer.duration();
				float bufferTime = bufferSize()/sampleRate();
				m_lastBufferUsage = m_lastBufferDuration/bufferTime;
				if(m_lastBufferUsage > m_bufferUsagePeak) {
					m_bufferUsagePeak = m_lastBufferUsage;
				}

				

			} else if(buffer[0]==101) { // Send next block
				//aLog("Send next buffer from/to %s.", m_udpSocket->lastReceivedAddress());
				if(resetReceived) {
					//memset(sendBuffer, 0, sendBufferSize);
					
					//for(int i=0; i<headerSize; i++) {
					//	m_sendBuffer[i] = buffer[i];
					//}
					memcpy(m_sendBuffer, buffer, headerSize);
					//ippsMemcpy



					//m_udpSocket->floatToChar(m_outputBuffer->data(), m_bufferSize*m_numOutputs, m_sendBuffer, headerSize);
			//		memcpy(&m_sendBuffer[24], m_outputBuffer->data(), sizeof(float)*m_bufferSize*m_numOutputs);


					//CSDEBUG("Converted Float->Char.");
					m_socketSend->sendBuffer(m_socketReceive->lastReceivedAddress(), m_sendBuffer, m_sendBufferSize);
					//CSDEBUG("Sent packet.");
					
					if(dotimer++ > 10) {
						m_timer.start();

						m_outputBuffer->zeroAll();
						double timestamp = (double)timeGetTime()*1000000.0;
						audioManager()->callback()->processAudio(m_inputBuffer, m_outputBuffer, timestamp, samples);
						audioManager()->callback()->postProcessAudio(timestamp, samples);
						samples += (double)m_bufferSize;

						m_timer.stop();
						m_lastBufferDuration = m_timer.duration();
						float bufferTime = bufferSize()/sampleRate();
						m_lastBufferUsage = m_lastBufferDuration/bufferTime;
						if(m_lastBufferUsage > m_bufferUsagePeak) {
							m_bufferUsagePeak = m_lastBufferUsage;
						}

						memcpy(&m_sendBuffer[headerSize], m_outputBuffer->data(), sizeof(float)*m_bufferSize*m_numOutputs);

						dotimer = 0;
					} else {
						m_outputBuffer->zeroAll();
						double timestamp = (double)timeGetTime()*1000000.0;
						audioManager()->callback()->processAudio(m_inputBuffer, m_outputBuffer, timestamp, samples);
						audioManager()->callback()->postProcessAudio(timestamp, samples);
						samples += (double)m_bufferSize;

						memcpy(&m_sendBuffer[headerSize], m_outputBuffer->data(), sizeof(float)*m_bufferSize*m_numOutputs);
					}
				}
				
			}
		} else {
			//aLog("Sleeping."); // What was this for again???
			Sleep(1);
		}
	}

	aLog("******Network Audio Device stopping...");
}

}