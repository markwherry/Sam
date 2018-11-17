#include "UiAudioHardware.h"

#include "SamPreferences.h"

#include <QmessageBox>

UiAudioHardware::UiAudioHardware(QWidget *parent, CsAudioManager *audioManager)
	: QWidget(parent)
	, m_audioManager(audioManager)
{
	//ui.setupUi(this);

	int margin = 10;

	setFixedSize(285 + (margin*2) + 30,50 + (margin*2) + 30);


	// Init the labels
	// ---------------
	m_labelAudioDevice = new UiLabel(this);
	m_labelAudioDevice->setText("Audio System");
	m_labelAudioDevice->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
	m_labelAudioDevice->setFixedSize(120,20);
	m_labelAudioDevice->move(margin+0,margin+0);

	m_labelAudioDriver = new UiLabel(this);
	m_labelAudioDriver->setText("Audio Driver");
	m_labelAudioDriver->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
	m_labelAudioDriver->setFixedSize(120,20);
	m_labelAudioDriver->move(margin+0,margin+30);

	m_labelNumInputs = new UiLabel(this);
	m_labelNumInputs->setText("Network MIDI Inputs");
	m_labelNumInputs->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
	m_labelNumInputs->setFixedSize(120,20);
	m_labelNumInputs->move(margin+0,margin+60);


	// Init the menu for selecting devices
	// -----------------------------------
	m_selectAudioDevice = new UiSelectMenu(this, 200);
	m_selectAudioDevice->move(margin+85+50,margin+0);
	connect(m_selectAudioDevice, SIGNAL(itemSelected(int)), this, SLOT(updateDevice(int)));
	
	if(m_audioManager) {
		//qDebug("numAudioDevices = %d", m_audioServer->numAudioDevices());
		for(int i=0; i<m_audioManager->numAudioDevices(); i++) {
			//qDebug("name = %s", m_audioServer->audioDeviceInfo(i)->name.toChar());
			m_selectAudioDevice->addItem(QString(m_audioManager->deviceInfo(i)->name.toChar()));
			if(i==0) {
				m_selectAudioDevice->addSeparator();
			}
		}

		int currentAudioDevice = m_audioManager->currentAudioDeviceIndex();
		qDebug("m_audioServer->currentAudioDeviceId() = %d", m_audioManager->currentAudioDeviceIndex());
		if(currentAudioDevice<0) {
			currentAudioDevice = 0;
		}
		m_selectAudioDevice->setSelectedItem(currentAudioDevice);//*/
	}

	
	// Init the menu for selecting drivers
	// -----------------------------------
	m_selectAudioDriver = new UiSelectMenu(this, 200);
	m_selectAudioDriver->move(margin+85+50,margin+30);

	updateDriverList();

	connect(m_selectAudioDriver, SIGNAL(itemSelected(int)), this, SLOT(updateDriver(int)));

	m_selectAudioDriver->setSelectedItem(QString(m_audioManager->currentAudioDeviceDriver().toChar()));


	m_originalPorts = samPreferences->engine()->preference("NumNetworkMidiInputs", "8").toInt();
	m_valueNumInputs = new UiValue(this, 50, 20, "", 1, 32, 0, 1, 2);
	m_valueNumInputs->move(margin+85+50+19, margin+30+30);
	m_valueNumInputs->setValueTextColor(QColor(255,255,255));
	m_valueNumInputs->setValueTextFont(QFont("Calibri", 10, QFont::Bold));
	m_valueNumInputs->setValue(m_originalPorts);
	connect(m_valueNumInputs, SIGNAL(valueChanged(float)), this, SLOT(updateInputs(float)));
}

UiAudioHardware::~UiAudioHardware()
{

}

void UiAudioHardware::updateDevice(int deviceIndex)
{
	qDebug("UiAudioHardware::updateDevice(%d)", deviceIndex);

	if(!m_audioManager) {
		return;
	}

	AString deviceId = m_audioManager->deviceInfo(deviceIndex)->uuid;

	m_audioManager->close();
	m_audioManager->exit();

	if(!m_audioManager->init(deviceId)) {
		qDebug("Device could not be initialised.");
		return;
	}

	//if(m_audioServer->numDeviceDrivers() < 1) {
		if(m_audioManager->open("")) {
			m_audioManager->start();
		}
	//}

	updateDriverList();
}

void UiAudioHardware::updateDriver(int offsetDriverId)
{
	qDebug("UiAudioHardware::updateDriver(%d)", offsetDriverId);

	if(!m_audioManager) {
		return;
	}

	m_audioManager->switchDeviceDriver(m_audioManager->deviceDriverName(offsetDriverId-1));
}

void UiAudioHardware::updateInputs(float inputs)
{
	samPreferences->engine()->setPreference("NumNetworkMidiInputs", AString::number((int)inputs));
}

void UiAudioHardware::closeEvent(QCloseEvent *ev)
{
	qDebug("UiAudioHardware::closeEvent()");

	if(samPreferences->engine()->preference("NumNetworkMidiInputs", "8").toInt() != m_originalPorts) {
		QMessageBox::warning(this, "Sam", "<b>You will need to restart the program</b><br><br>in order for the number of MIDI inputs to be adjusted.");
	}
}

void UiAudioHardware::updateDriverList(void)
{
	m_selectAudioDriver->clear();
	m_selectAudioDriver->setEnabled(false);
	m_selectAudioDriver->addItem("No Audio Driver");

	if(!m_audioManager) {
		return;
	}

	if(m_audioManager->numDeviceDrivers()>0) {
		m_selectAudioDriver->setEnabled(true);
		for(int i=0; i<m_audioManager->numDeviceDrivers(); i++) {
			m_selectAudioDriver->addItem(QString(m_audioManager->deviceDriverName(i).toChar()));
		}
	}
}