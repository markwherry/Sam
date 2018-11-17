#include "CsWindowsMmeMidiDevice.h"

#include "CsMidiManager.h"
#include "ALog.h"

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsWindowsMmeMidiInputThread::CsWindowsMmeMidiInputThread(CsMidiManager *midiManager, int deviceIndex, int portIndex)
: m_midiManager(midiManager)
, m_deviceIndex(deviceIndex)
, m_portIndex(portIndex)
{
	m_waitForMessage = new AWaitEvent();
	m_midiMessages = new ARingBuffer<CsMidiMessage>(1024);

	startThread();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsWindowsMmeMidiInputThread::~CsWindowsMmeMidiInputThread()
{
	stopThread();
	m_waitForMessage->set();
	waitForThreadToExit();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsWindowsMmeMidiInputThread::handleData(DWORD dwParam1, DWORD dwParam2)
{
	CsMidiMessage midiMessage;
	midiMessage.device    = m_deviceIndex;
	midiMessage.port      = m_portIndex;
	midiMessage.timestamp = (double)(m_startTime+dwParam2)*1000000.0f;
	midiMessage.data[0]   = dwParam1 & 0x000000FF;
	midiMessage.data[1]   = (dwParam1>>8) & 0x000000FF;
	midiMessage.data[2]   = (dwParam1>>16) & 0x000000FF;
	
	m_midiMessages->add(midiMessage);
	m_waitForMessage->set();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsWindowsMmeMidiInputThread::threadProcess(void)
{
	int numMessages = 0;
	while(isThreadRunning()) {
		m_waitForMessage->wait();
		numMessages = m_midiMessages->size();
		for(int i=0; i<numMessages; i++) {
			CsMidiMessage midiMessage = m_midiMessages->get();
			midiMessage.identify();
			midiMessage.defineLength();
			m_midiManager->processMidiEvent(midiMessage);
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsWindowsMmeMidiDevice::CsWindowsMmeMidiDevice(CsMidiManager *midiManager, int deviceIndex)
: CsMidiDevice(midiManager, deviceIndex)
{
	m_deviceInfo.name = AString("Windows Multimedia MIDI");
	m_deviceInfo.uuid = AString("F3B828AB-0E3E-437E-9505-60C22C9D7A1D");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsWindowsMmeMidiDevice::init()
{
	for(int i=0; i<64; i++) {
		hMidiOut[i] = 0;
		hMidiIn[i]  = 0;
		m_inputThreads[i] = 0;
	}

	timeBeginPeriod(1);

	hTimerQueue = CreateTimerQueue();
    if(hTimerQueue == NULL) {
        aLog("CreateTimerQueue failed (%d)\n", GetLastError());
        return 0;
    } 
	
	//aLog("TimerQueue created OK.");

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsWindowsMmeMidiDevice::exit(void)
{
	//aLog("AWindowsMidi::exit()");

	if(hTimerQueue) {
		DeleteTimerQueue(hTimerQueue);
	}

	timeEndPeriod(1);

	//aLog("EXIT: CsWindowsMidi::exit()");

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsWindowsMmeMidiDevice::start(bool manuallyActivatePorts)
{
	//aLog("CsWindowsMidi::start()");

	if(!manuallyActivatePorts) {
		for(int i=0; i<numPorts(CsMidiDevice::Input); i++) {
			setPortActive(i, CsMidiDevice::Input, true);
		}
		for(int i=0; i<numPorts(CsMidiDevice::Output); i++) {
			if(portName(i, CsMidiDevice::Output) != AString("Microsoft GS Wavetable Synth")) {
				setPortActive(i, CsMidiDevice::Output, true);
			} else {
				aLog("Did not active Microsoft GS Wavetable Synth.");
			}
		}
	}

	setActive(true);

	return 1; 
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsWindowsMmeMidiDevice::stop(void)
{
	//aLog("CsWindowsMidi::stop()");

	for(int i=0; i<numPorts(CsMidiDevice::Input); i++) {
		setPortActive(i, CsMidiDevice::Input, false);
	}
	
	for(int i=0; i<numPorts(CsMidiDevice::Output); i++) {
		setPortActive(i, CsMidiDevice::Output, false);
	}

	setActive(false);

	//aLog("EXIT: CsWindowsMidi::stop()");

	return AOk;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CALLBACK CsWindowsMmeMidiDevice::MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	//aLog("CsWindowsMmeMidiDevice::MidiInProc");

	CsWindowsMmeMidiInputThread *thread = (CsWindowsMmeMidiInputThread*)dwInstance;

	if(wMsg == MIM_DATA) {
		//aLog("MIM_DATA");
		thread->handleData(dwParam1, dwParam2);
	} else if(wMsg == MIM_LONGDATA) {
	} else {
	}

	//CsWindowsMmeMidiInputCallbackData *callbackData = (CsWindowsMmeMidiInputCallbackData*)dwInstance;
	//CsWindowsMmeMidiDevice *instance = (CsWindowsMmeMidiDevice*)callbackData->instance;

	/*LPMIDIHDR lpMIDIHeader;
	unsigned char *	ptr;
	TCHAR buffer[80];
	unsigned char bytes;*/

	/*midiEvent.port = callbackData->port;
	midiEvent.timestamp = (double)(instance->m_driverStartTime+dwParam2)*1000000;// (CsUInt64)timeGetTime()*1000000;//(cswmm->m_driverStartTime+dwParam2)*1000000;
	//aLog("%lld",cswmm->m_driverStartTime);

	if(wMsg == MIM_DATA) {
		CsMidiMessage midiMessage;
		midiMessage.timestamp = (CsNanoTime)(instance->m_driverStartTime+dwParam2)*1000000.0f;
		midiMessage.data[0] = dwParam1 & 0x000000FF;
		midiMessage.data[1] = (dwParam1>>8) & 0x000000FF;
		midiMessage.data[2] = (dwParam1>>16) & 0x000000FF;
		instance->midiManager()->processMidiEvent(midiEvent);
	} else if(wMsg == MIM_LONGDATA) {
	} else {
	}
	switch (wMsg) {
		case MIM_DATA:
		{
			CsMidiMessage midiMessage
		//CsMIDIPacketAllocate (&packet, 3);
		//aLog("0x%08X 0x%02X 0x%02X 0x%02X\r\n", dwParam2, dwParam1 & 0x000000FF, (dwParam1>>8) & 0x000000FF, (dwParam1>>16) & 0x000000FF);
		midiEvent.data[0] = dwParam1 & 0x000000FF;
		midiEvent.data[1] = (dwParam1>>8) & 0x000000FF;
		midiEvent.data[2] = (dwParam1>>16) & 0x000000FF;
		//cswmm->csm->onMIDIIn (&packet);

		if(midiEvent.data[0]>=0x80 && midiEvent.data[0]<=0xBF)
			midiEvent.length = 3;
		else if(midiEvent.data[0]>=0xC0 && midiEvent.data[0]<=0xDF)
			midiEvent.length = 2;
		else if(midiEvent.data[0]>=0xE0 && midiEvent.data[0]<=0xEF)
			midiEvent.length = 3;

		instance->midiManager()->processMidiEvent(midiEvent);

		

		break;*/
	/*case MIM_LONGDATA:
		packet.length = 0;
		aLog ( "Long data!\n");
			if (!(SysXFlag & 0x80))
			{
				lpMIDIHeader = (LPMIDIHDR)dwParam1; //
				ptr = (unsigned char *)(lpMIDIHeader->lpData);
				
				if (!SysXFlag)
				{
					aLog("*************** System Exclusive **************\r\n0x%08X ", dwParam2);
					SysXFlag |= 0x01;
				}

				if (*(ptr + (lpMIDIHeader->dwBytesRecorded - 1)) == 0xF7)
					SysXFlag &= (~0x01);


				//aLog ("%s ",lpMIDIHeader->lpData);

				while((lpMIDIHeader->dwBytesRecorded--))
				{
					packet.data[packet.length] = *(ptr)++;
					//cout << packet.data[packet.length] << " ";
					aLog ("0x%02X ", packet.data[packet.length]);
					packet.length++;
				}

				midiInAddBuffer(inHandle, lpMIDIHeader, sizeof(MIDIHDR));

				//MackieControlC4.Driver (packet);
			}
		break; */
	//}
	/*case MIM_OPEN:
		aLog ("MIM_OPEN\n");
		break;
	case MIM_CLOSE:
		aLog ("MIM_CLOSE\n");
		break;
	case MIM_ERROR:
		aLog ("MIM_ERROR\n");
		break;
	case MIM_LONGERROR:
		aLog ("MIM_LONGERROR\n");
		break;
	}*/

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsWindowsMmeMidiDevice::sendMidiEvent(CsMidiMessage midiMessage)
{
	//aLog("CsWindowsMmeMidiDevice::sendMidiEvent()");
	
	//aLog("CsWindowsMidi::sendMidiEvent(%d,%lldd) [%d]",midiEvent.data[0],midiEvent.timestamp, timeGetTime());

	if (midiMessage.length>3) {
		// System Exclusive
		//aLog("System Exclusive");
	} else {
		if(midiMessage.timestamp==0) {
			if (midiMessage.length==3) {
				//aLog("3 [%02x] [%d] [%d]", midiMessage.data[0], midiMessage.data[1], midiMessage.data[2]);
				midiOutShortMsg(hMidiOut[midiMessage.port], (midiMessage.data[2]*65536)+(midiMessage.data[1]*256)+(midiMessage.data[0]));
			} else if (midiMessage.length==2) {
				//aLog("2 [%02x] [%d]", midiMessage.data[0], midiMessage.data[1]);
				midiOutShortMsg(hMidiOut[midiMessage.port], (0*65536)+(midiMessage.data[1]*256)+(midiMessage.data[0]));
			}
		} else {
			/*midiMessage.handle = hMidiOut[midiMessage.port];
			MidiEvent *midiEventToSend;
			midiEventToSend = (MidiEvent*)malloc(sizeof(MidiEvent));
			*midiEventToSend = midiEvent;
			HANDLE hTimer = 0;
			if (!CreateTimerQueueTimer(&hTimer, hTimerQueue,(WAITORTIMERCALLBACK)WaitOrTimerCallback, midiEventToSend , midiEvent.timestamp-timeGetTime(), 0, 0)) {
				aLog("CreateTimerQueueTimer failed (%d)\n", GetLastError());
			}*/
		}

	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsWindowsMmeMidiDevice::numPorts(CsMidiDevicePortType type) const
{
	if(type == CsMidiDevice::Input) {
		return midiInGetNumDevs();
	}

	return midiOutGetNumDevs();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString CsWindowsMmeMidiDevice::portName(int portIndex, CsMidiDevicePortType type) const
{
	if(type == CsMidiDevice::Input) {
		MIDIINCAPS mic;
		if (midiInGetDevCaps(portIndex, &mic, sizeof(MIDIINCAPS)) == MMSYSERR_NOERROR) {
			return AString(mic.szPname);
		} else {
			return AString("Input Port Name Not Recognised");
		}
	}

	MIDIOUTCAPS moc;
	if (midiOutGetDevCaps(portIndex, &moc, sizeof(MIDIINCAPS)) == MMSYSERR_NOERROR) {
		return AString(moc.szPname);
	}

	return AString("Output Port Name Not Recognised");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsWindowsMmeMidiDevice::isPortActive(int portIndex, CsMidiDevicePortType type) const
{
	if(type == CsMidiDevice::Input) {
		if(hMidiIn[portIndex]) {
			return true;
		} else {
			return false;
		}
	}

	if(hMidiOut[portIndex]) {
		return true;
	} else {
		return false;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsWindowsMmeMidiDevice::setPortActive(int portIndex, CsMidiDevicePortType type, bool active)
{
	aLog("CsWindowsMmeMidiDevice::setPortActive(%d, %d, %d)", portIndex, (int)type, (int)active);

	MMRESULT result;

	if(type == CsMidiDevice::Input) {

		if(active) {

			if(hMidiIn[portIndex]) {
				return AOk;
			}

			CsWindowsMmeMidiInputThread *thread = new CsWindowsMmeMidiInputThread(midiManager(), deviceIndex(), portIndex);

			result = midiInOpen(&hMidiIn[portIndex], portIndex, (DWORD_PTR)MidiInProc, (DWORD_PTR)thread, CALLBACK_FUNCTION);
			if(result == MMSYSERR_NOERROR) {
				thread->setStartTime(timeGetTime());
				midiInStart(hMidiIn[portIndex]);
				m_inputThreads[portIndex] = thread;
				return AOk;
			} else {
				hMidiIn[portIndex] = 0;
				delete thread;
				aLog ("Error creating input %d (%d)",portIndex, result);
				debugMidiResult(result);
				return AError;
			}

		} else {

			if(!hMidiIn[portIndex]) {
				return AOk;
			}

			midiInReset(hMidiIn[portIndex]);
			midiInClose(hMidiIn[portIndex]);
			hMidiIn[portIndex] = 0;
			delete m_inputThreads[portIndex];
			m_inputThreads[portIndex] = 0;

			return AOk;

		}

	} else if(type == CsMidiDevice::Output) {

		if(active) {

			if(hMidiOut[portIndex]) {
				return AOk;
			}

			result = midiOutOpen(&hMidiOut[portIndex], portIndex, 0, 0, CALLBACK_NULL);
			if(result == MMSYSERR_NOERROR) {
				return AOk;
			} else {
				hMidiOut[portIndex] = 0;
				aLog("Error creating output %d (%d), %d", portIndex, result, GetLastError());
				debugMidiResult(result);
				return AError;
			}

		} else {

			if(!hMidiOut[portIndex]) {
				return AOk;
			}

			midiOutClose(hMidiOut[portIndex]);
			hMidiOut[portIndex] = 0;

			return AOk;
		}

	}

	return AError;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsWindowsMmeMidiDevice::WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{/*
	//aLog("CsWindowsMidi::WaitOrTimerCallback()");
	
	CsMidiMessage *midiMessage = (MidiEvent*)lpParameter;
	
	//aLog("--play--> %d %d %d (%d)\n", midiEvent->data[0], midiEvent->data[1], midiEvent->data[2], timeGetTime());
	
	midiOutShortMsg((HMIDIOUT)midiMessage->handle, (midiMessage->data[2]*65536)+(midiMessage->data[1]*256)+(midiMessage->data[0]));
	free(midiEvent);*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsWindowsMmeMidiDevice::debugMidiResult(MMRESULT result)
{
	switch(result) {
		case MIDIERR_NODEVICE:
			aLog("MIDIERR_NODEVICE");
			break;
		case MMSYSERR_ALLOCATED: // The specified resource is already allocated.
			aLog("MMSYSERR_ALLOCATED\n");
			break;
		case MMSYSERR_BADDEVICEID: // The specified device identifier is out of range.
			aLog("MMSYSERR_BADDEVICEID\n");
			break;
		case MMSYSERR_INVALFLAG: // The flags specified by dwFlags are invalid.
			aLog("MMSYSERR_INVALFLAG\n");
			break;
		case MMSYSERR_INVALPARAM: //The specified pointer or structure is invalid.
			aLog("MMSYSERR_INVALPARAM\n");
			break;
		case MMSYSERR_NOMEM: // The system is unable to allocate or lock memory.
			aLog("MMSYSERR_NOMEM\n");
			break;
		case MMSYSERR_NOTSUPPORTED: // Function isn't supported
			aLog("MMSYSERR_NOTSUPPORTED");
			break;
	}
}

}