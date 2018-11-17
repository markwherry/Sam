#include "CsNetworkMidiDevice.h"
#include "CsMidiManager.h"

#include "ALog.h"

namespace Cs {

/*CsNetworkMidiInputThread::CsNetworkMidiInputThread(CsMidiManager *midiManager, int deviceIndex, int portIndex)
: m_midiManager(midiManager)
, m_deviceIndex(deviceIndex)
, m_portIndex(portIndex)
{
	m_waitForMessage = new AWaitEvent();
	m_midiMessages = new ARingBuffer<CsMidiMessage>(1024);

	//startThread();
	//setPriority(THREAD_PRIORITY_HIGHEST);
}

CsNetworkMidiInputThread::~CsNetworkMidiInputThread()
{
	stopThread();
	m_waitForMessage->set();
	waitForThreadToExit();
}

void CsNetworkMidiInputThread::handleBuffer(char *buffer)
{
	if (buffer[0] == 1) {
		buffer[3]--;
		buffer[2]--;
		
		//aLog("1[%d] 2[%d] 3[%d]", buffer[1], buffer[2], buffer[3]);
		
		CsMidiMessage midiMessage;
		midiMessage.device = m_deviceIndex;
		midiMessage.port = buffer[4]-1;
		midiMessage.timestamp = (double)timeGetTime()*1000000.0f;
		midiMessage.data[0] = buffer[1]+256;
		midiMessage.data[1] = buffer[2];
		midiMessage.data[2] = buffer[3];

		//m_midiMessages->add(midiMessage);
		//m_waitForMessage->set();
		

		midiMessage.identify();
			midiMessage.defineLength();
			m_midiManager->processMidiEvent(midiMessage);

	} else if (buffer[0] == 2) {
		//CSDEBUG("Clock!");
	} else if(buffer[0] == 3) {
		////csDebug("Tempo packet received!");
		////for(int i=0; i<BUFLEN; i++) {
		////	csDebug("[%d] %d", i, inputBuffer[i]);
		////}
		//floatchar fc;
		//for(int l=0; l<4; l++) {
		//	//csDebug("fc.c = %d", fc.c[l]);
		//	fc.c[l] = inputBuffer[l+1];
		//}
		////csDebug("fc.f = %f", fc.f);
		//tempo = fc.f;
		////csDebug("Tempo = %f", tempo);

		//midiEvent.timestamp = (double)timeGetTime () *1000000.0;
		//midiEvent.length = 0;
		//midiEvent.type = CsMidiEvent::meTempo;
		//midiEvent.tempo = tempo;
		//m_midiServer->processMidiEvent(midiEvent); 
	}	
}

/*void CsNetworkMidiInputThread::threadProcess(void)
{
	int numMessages = 0;
	while(isThreadRunning()) {
		m_waitForMessage->wait();
		numMessages = m_midiMessages->size();
		for(int i=0; i<numMessages; i++) {
			CsMidiMessage midiMessage = m_midiMessages->get();
			
			m_midiManager->processMidiEvent(midiMessage);
		}
	}
}*/
/*
void CsNetworkMidiInputThread::threadProcess(void)
{
	while(isThreadRunning()) {
		//aLog("CsNetworkMidiInputThread::threadProcess() Loop");
		if(m_midiMessages->size() > 0) {
			CsMidiMessage midiMessage = m_midiMessages->get();
			midiMessage.identify();
			midiMessage.defineLength();
			m_midiManager->processMidiEvent(midiMessage);
		} else {
			m_waitForMessage->wait();
		}
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsNetworkMidiDevice::CsNetworkMidiDevice(CsMidiManager *midiManager, int deviceIndex, char *midiIpAddress, int numPorts)
	: CsMidiDevice(midiManager, deviceIndex)
	, m_numPorts(numPorts)
{
	m_deviceInfo.name = AString("Network MIDI");
	m_deviceInfo.uuid = AString("17336EEC-6488-46F1-A4C4-7E80280A32A6");

	m_midiIpAddress = midiIpAddress;

	//for(int i=0; i<4; i++) {
	//	m_inputThreads[i] = 0;
	//}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsNetworkMidiDevice::~CsNetworkMidiDevice()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsNetworkMidiDevice::init(void)
{
	timeBeginPeriod(1);

	if (!udpSocket.open (9930, m_midiIpAddress)) {
		aLog("CsNetworkMidiServer: Error opening socket on port 9930.\n");
		return 0;
	}
	
	aLog("CsNetworkMidiServer: Socket open on port 9930.\n");

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsNetworkMidiDevice::exit(void)
{
	aLog("CsNetworkMidiDevice::exit()");

	udpSocket.close();

	timeEndPeriod(1);

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsNetworkMidiDevice::start(bool manuallyActivatePorts)
{
	//for(int i=0; i<4; i++) {
	//	m_inputThreads[i] = new CsNetworkMidiInputThread(midiManager(), deviceIndex(), i);
	//}

	startThread();
	setPriority(THREAD_PRIORITY_HIGHEST); // Tried AThread::TimeCritical, but this might have caused some big issues.

	setActive(true);

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsNetworkMidiDevice::stop(void)
{
	aLog("CsNetworkMidiDevice::stop()");

	stopThread();

	char buffer[18];
	memset(buffer, 0, 18);
	udpSocket.sendBuffer("127.0.0.1", buffer, 18);

	waitForThreadToExit();

	//for(int i=0; i<4; i++) {
	//	delete m_inputThreads[i];
	//}

	setActive(false);

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsNetworkMidiDevice::sendMidiEvent(CsMidiMessage midiMessage)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsNetworkMidiDevice::numPorts(CsMidiDevicePortType type) const
{
	if(type == CsMidiDevice::Input) {
		return m_numPorts;
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString CsNetworkMidiDevice::portName(int portIndex, CsMidiDevicePortType type) const
{
	if(type == CsMidiDevice::Input) {
		return AString("Network MIDI Port ") + AString::number(portIndex + 1);
	} else {
	}

	return AString("");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsNetworkMidiDevice::isPortActive(int portIndex, CsMidiDevicePortType type) const
{
	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsNetworkMidiDevice::setPortActive(int portIndex, CsMidiDevicePortType type, bool active)
{
	return AOk;
}

// Length of UDP MIDI packet.
#define BUFLEN 18

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsNetworkMidiDevice::threadProcess(void)
{
	char buffer[BUFLEN];

	int port = 0;

	while(isThreadRunning()) {
		memset(buffer, 0, BUFLEN);
		
		//if (udpSocket.receive (inputBuffer, 18) > 0) {
		unsigned long address = 0;
					
		if(udpSocket.receiveBuffer(buffer, BUFLEN, &address)) {
			//aLog("Packet received.");
			//Sleep(1);
			/*port = (int)buffer[4]-1;
			if(port >= 0 && port < 4) {
				m_inputThreads[port]->handleBuffer(buffer);
			}*/
			if (buffer[0] == 1) {
				buffer[3]--;
				buffer[2]--;
				
				//aLog("1[%d] 2[%d] 3[%d]", buffer[1], buffer[2], buffer[3]);
				
				CsMidiMessage midiMessage;
				midiMessage.device = deviceIndex();
				midiMessage.port = buffer[4]-1;
				midiMessage.timestamp = (double)timeGetTime()*1000000.0f;
				//aLog("midiMessage.timestamp = %f", midiMessage.timestamp);
				midiMessage.data[0] = buffer[1]+256;
				midiMessage.data[1] = buffer[2];
				midiMessage.data[2] = buffer[3];

				//m_midiMessages->add(midiMessage);
				//m_waitForMessage->set();
				

				midiMessage.identify();
				midiMessage.defineLength();
				midiManager()->callback()->processMidiMessage(midiMessage);
			} else if (buffer[0] == 2) {
					//CSDEBUG("Clock!");
			} else if(buffer[0] == 3) {
				//csDebug("Tempo packet received!");
				//for(int i=0; i<BUFLEN; i++) {
				//	csDebug("[%d] %d", i, inputBuffer[i]);
				//}
				floatchar fc;
				for(int l=0; l<4; l++) {
					//csDebug("fc.c = %d", fc.c[l]);
					fc.c[l] = buffer[l+1];
				}
				//csDebug("fc.f = %f", fc.f);
				//tempo = fc.f;
				//csDebug("Tempo = %f", tempo);
				midiManager()->callback()->processTempoMessage((double)timeGetTime() * 1000000.0f, fc.f);

				//midiEvent.timestamp = (double)timeGetTime () *1000000.0;
				//midiEvent.length = 0;
				//midiEvent.type = CsMidiEvent::meTempo;
				//midiEvent.tempo = tempo;
				//m_midiServer->processMidiEvent(midiEvent);
			}	
		}
	}

	aLog("***** EXIT: CsNetworkMidiDevice::threadProcess() *****");
}

}