#include "CsMidiManager.h"
#include "CsWindowsMmeMidiDevice.h"
#include "CsNetworkMidiDevice.h"

#include "ALog.h"

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsMidiManager::CsMidiManager(CsMidiManagerCallback *callback)
: m_callback(callback)
{
	m_waitForMessage = new AWaitEvent();
	m_midiMessages = new ARingBuffer<CsMidiMessage>(1024);

	startThread();
	setPriority(THREAD_PRIORITY_HIGHEST);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsMidiManager::~CsMidiManager()
{
	stopThread();
	m_waitForMessage->set();
	waitForThreadToExit();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsMidiManager::init(int numNetworkMidiInputs)
{
	aLog("CsMidiManager::init()");

	if(!m_callback) {
		return 0;
	}

	CsMidiDevice *windowsMmeMidiDevice = new CsWindowsMmeMidiDevice(this, numDevices());
	if(windowsMmeMidiDevice) {
		if(windowsMmeMidiDevice->init()) {
			if(windowsMmeMidiDevice->start()) {
				m_midiDevices.append(windowsMmeMidiDevice);
			}
		}
	}

	CsMidiDevice *networkMidiDevice = new CsNetworkMidiDevice(this, numDevices(), 0, numNetworkMidiInputs);
	if(networkMidiDevice) {
		if(networkMidiDevice->init()) {
			if(networkMidiDevice->start()) {
				m_midiDevices.append(networkMidiDevice);
			}
		}
	}

	for(int i=0; i<m_midiDevices.size(); i++) {
		m_midiDevices[i]->debugPorts();
	}


	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsMidiManager::exit(void)
{
	for(int i=0; i<m_midiDevices.size(); i++) {
		if(m_midiDevices[i]->isActive()) {
			m_midiDevices[i]->stop();
		}
		m_midiDevices[i]->exit();
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsMidiManager::processMidiEvent(CsMidiMessage midiMessage)
{
	//m_csMessage.enter();
	m_midiMessages->add(midiMessage);
	//m_csMessage.leave();
	m_waitForMessage->set();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsMidiManager::sendMidiEvent(CsMidiMessage midiMessage)
{
	m_midiDevices[midiMessage.device]->sendMidiEvent(midiMessage);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsMidiManager::numPorts(int deviceIndex, CsMidiDevice::CsMidiDevicePortType type) const
{
	if(deviceIndex < 0 || deviceIndex >= m_midiDevices.size()) {
		return 0;
	}

	return m_midiDevices[deviceIndex]->numPorts(type);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString CsMidiManager::portName(int deviceIndex, int portIndex, CsMidiDevice::CsMidiDevicePortType type) const
{
	if(deviceIndex < 0 || deviceIndex >= m_midiDevices.size()) {
		return "";
	}

	return m_midiDevices[deviceIndex]->portName(portIndex, type);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsMidiManager::isPortActive(int deviceIndex, int portIndex, CsMidiDevice::CsMidiDevicePortType type) const
{
	if(deviceIndex < 0 || deviceIndex >= m_midiDevices.size()) {
		return false;
	}

	return m_midiDevices[deviceIndex]->isPortActive(portIndex, type);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsMidiManager::setPortActive(int deviceIndex, int portIndex, CsMidiDevice::CsMidiDevicePortType type, bool active)
{
	if(deviceIndex < 0 || deviceIndex >= m_midiDevices.size()) {
		return 0;
	}

	return m_midiDevices[deviceIndex]->setPortActive(portIndex, type, active);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsMidiManager::threadProcess(void)
{
	int numMessages = 0;

	while(isThreadRunning()) {
		if(m_midiMessages->size() > 0) {
			//m_csMessage.enter();
			CsMidiMessage midiMessage = m_midiMessages->get();
			//m_csMessage.leave();
			m_callback->processMidiMessage(midiMessage);
		} else {
			m_waitForMessage->wait();
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void CsMidiManager::threadProcess(void)
{
	int numMessages = 0;

	while(isThreadRunning()) {
		m_waitForMessage->wait();
		numMessages = m_midiMessages->size();
		for(int i=0; i<numMessages; i++) {
			CsMidiMessage midiMessage = m_midiMessages->get();
			midiMessage.identify();
			m_callback->processMidiEvent(midiMessage);
		}
	}
}*/


}