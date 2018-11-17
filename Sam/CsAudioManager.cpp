#include "CsAudioManager.h"

#include "CsDummyAudioDevice.h"
#include "CsAsioAudioDevice.h"
#include "CsNetworkAudioDevice.h"
//#include "CsConnectAudioDevice.h"
#include "ALog.h"

namespace Cs {

// --------------------------------------------------------------------------------------------------------------------------------------------
CsAudioManager::CsAudioManager(CsAudioManagerCallback *callback)
: m_callback(callback)
, m_currentAudioDeviceId(-1)
{
	CsAudioDevice *dummyAudioDevice = new CsDummyAudioDevice(this);
	if(dummyAudioDevice) {
		m_audioDevices.append(dummyAudioDevice);
	}

	CsAudioDevice *asioAudioDevice = new CsAsioAudioDevice(this);
	if(asioAudioDevice) {
		m_audioDevices.append(asioAudioDevice);
	}

	CsAudioDevice *networkAudioDevice = new CsNetworkAudioDevice(this);
	if(networkAudioDevice) {
		m_audioDevices.append(networkAudioDevice);
	}

	/*CsAudioDevice *connectAudioDevice = new CsConnectAudioDevice(this);
	if(networkAudioDevice) {
		m_audioDevices.append(connectAudioDevice);
	}*/
}

// --------------------------------------------------------------------------------------------------------------------------------------------
CsAudioManager::~CsAudioManager()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsAudioManager::init(AString deviceuuid)
{
	aLog("AudioServer::init(%s)", deviceuuid.toChar());

	int audioDeviceId = -1;

	for(int i=0; i<m_audioDevices.size(); i++) {
		aLog("[%s] [%s]", deviceuuid.toChar(), m_audioDevices[i]->deviceInfo()->uuid.toChar());
		if(deviceuuid == m_audioDevices[i]->deviceInfo()->uuid) {
			audioDeviceId = i;
		}
	}

	aLog("audioDeviceId = %d", audioDeviceId);

	if(audioDeviceId < 0 || audioDeviceId >= m_audioDevices.size()) {
		aLog("Error: Audio Device not found.");
		if(m_audioDevices.size() >= 1) {
			aLog("Using dummy device instead.");
			audioDeviceId = 0;
		} else {
			aLog("Dummy device not found.");
			return AError;
		}
	}

	if(!m_audioDevices[audioDeviceId]->init()) {
		aLog("Error: device->init()");
		return AError;
	}

	m_currentAudioDeviceId = audioDeviceId;

	return AOk;
}

// --------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsAudioManager::exit(void)
{
	aLog("AudioManager::exit()");

	if(m_currentAudioDeviceId > -1) {
		m_audioDevices[m_currentAudioDeviceId]->exit();
		m_currentAudioDeviceId = -1;
		return AOk;
	}
	
	return AError;
}

// --------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsAudioManager::open(AString driverName)
{
	return m_audioDevices[m_currentAudioDeviceId]->open(driverName);
}

// --------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsAudioManager::close(void)
{
	aLog("AudioManager::close()");

	if(m_audioDevices[m_currentAudioDeviceId]->isActive()) {
		m_audioDevices[m_currentAudioDeviceId]->stop();
	} else {
		aLog("Device wasn't started.");
	}

	aLog("Closing...");
	m_audioDevices[m_currentAudioDeviceId]->close();
	aLog("Closed.");
	
	return AOk;
}

// --------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsAudioManager::start(void)
{
	aLog("AudioManager::start()");

	if(m_currentAudioDeviceId<0) {
		aLog("AudioManager::start() m_currentAudioDeviceId<0");
		return AError;
	}

	m_audioDevices[m_currentAudioDeviceId]->updateAudioProcessConfiguration();

	m_audioDevices[m_currentAudioDeviceId]->start();

	return AOk;
}

// --------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsAudioManager::stop(void)
{
	if(m_currentAudioDeviceId<0) {
		return AError;
	}

	return m_audioDevices[m_currentAudioDeviceId]->stop();

}

// --------------------------------------------------------------------------------------------------------------------------------------------
AString CsAudioManager::currentAudioDeviceDriver(void)
{
	if(m_currentAudioDeviceId < 0) {
		return ""; //aLog("HERE!");
	}
	//aLog("%s",m_audioDevices[m_currentAudioDeviceId]->currentAudioDriverName().toChar());
	return m_audioDevices[m_currentAudioDeviceId]->currentAudioDriverName();
}

AString CsAudioManager::currentAudioDevice(void)
{
	if(m_currentAudioDeviceId < 0) {
		return "";
	}

	return m_audioDevices[m_currentAudioDeviceId]->deviceInfo()->uuid;
}

// --------------------------------------------------------------------------------------------------------------------------------------------
AStatus CsAudioManager::switchDeviceDriver(AString driverName)
{
	aLog("CsAudioManager::switchDeviceDriver(%s)", driverName.toChar());

	if(m_audioDevices[m_currentAudioDeviceId]->isActive()) {
		stop();
	} else {
		aLog("Device wasn't started.");
	}
	aLog("Closing...");
	close();
	aLog("Closed.");

	
	aLog("Opening...");
	if(open(driverName)) {
		aLog("Starting...");
		start();
		aLog("Started.");
	} else {
		aLog("Could not open.");
		return AError;
	}
	
	return AOk;
}

// --------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioManager::numDeviceDrivers(void)
{
	if(m_currentAudioDeviceId < 0) {
		return 0;
	}

	return m_audioDevices[m_currentAudioDeviceId]->numAudioDrivers();
}
// --------------------------------------------------------------------------------------------------------------------------------------------
AString CsAudioManager::deviceDriverName(int index)
{
	if(m_currentAudioDeviceId < 0) {
		return "";
	}

	return m_audioDevices[m_currentAudioDeviceId]->driverName(index);
}
// --------------------------------------------------------------------------------------------------------------------------------------------
float CsAudioManager::lastBufferDuration(void)
{
	if(m_currentAudioDeviceId < 0) {
		return 0.0f;
	}

	return m_audioDevices[m_currentAudioDeviceId]->lastBufferDuration();
}

// --------------------------------------------------------------------------------------------------------------------------------------------
float CsAudioManager::lastBufferUsage(void)
{
	if(m_currentAudioDeviceId < 0) {
		return 0.0f;
	}

	return m_audioDevices[m_currentAudioDeviceId]->lastBufferUsage();
}

// --------------------------------------------------------------------------------------------------------------------------------------------
float CsAudioManager::bufferUsagePeak(void)
{
	if(m_currentAudioDeviceId < 0) {
		return 0.0f;
	}

	return m_audioDevices[m_currentAudioDeviceId]->bufferUsagePeak();
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioManager::resetBufferUsagePeak(void)
{
	if(m_currentAudioDeviceId < 0) {
		return;
	}

	return m_audioDevices[m_currentAudioDeviceId]->resetBufferUsagePeak();
}

// --------------------------------------------------------------------------------------------------------------------------------------------
int CsAudioManager::bufferSize(void)
{
	if(m_currentAudioDeviceId < 0) {
		return 0;
	}

	return m_audioDevices[m_currentAudioDeviceId]->bufferSize();
}

// --------------------------------------------------------------------------------------------------------------------------------------------
double CsAudioManager::sampleRate(void)
{
	if(m_currentAudioDeviceId < 0) {
		return 0.0f;
	}

	return m_audioDevices[m_currentAudioDeviceId]->sampleRate();
}

// --------------------------------------------------------------------------------------------------------------------------------------------
AString  CsAudioManager::channelName(int id, bool isInput)
{
	if(m_currentAudioDeviceId < 0) {
		return AString("");
	}

	return m_audioDevices[m_currentAudioDeviceId]->channelName(id, isInput);
}


}