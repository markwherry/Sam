#include "SamGuiChannelPitchBendRemoteSetup.h"

#include <qpainter.h>

#include "SamEngine.h"
#include "SamPreferences.h"

#include "CsMidiName.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelPitchBendRemoteSetup::SamGuiChannelPitchBendRemoteSetup(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	int x = 8;
	int y = 8;

	setWindowTitle("Channel Pitch Bend Remote Setup");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);

	m_checkActive = new UiCheckBox(this);
	m_checkActive->move(x, y);
	m_checkActive->setText("Remote Active");
	m_checkActive->setChecked(true);
	connect(m_checkActive, SIGNAL(changed(bool)), this, SLOT(setActive(bool)));

	y+=36;

	m_labelController = new UiLabel(this, "Controller # for Remote");
	m_labelController->move(x, y);

	m_valueController = new UiValue(this, 50, 16, "", 0, 127, 0, 1, 2);
	m_valueController->move(x+140, y);
	m_valueController->setValueTextColor(QColor(255,255,255));
	m_valueController->setLiveUpdate();

	connect(m_valueController, SIGNAL(valueChanged(float)), this, SLOT(setController(float)));

	m_labelControllerName = new UiLabel(this, "");
	m_labelControllerName->move(x+140+40, y);
	m_labelControllerName->setTextColor(QColor(150,150,150));
	
	
	y+=24;


	m_labelTrigger = new UiLabel(this, "Value # triggers");
	m_labelTrigger->move(x, y);

	m_labelUp = new UiLabel(this, "Up");
	m_labelUp->move(x+100, y);
	m_labelDown = new UiLabel(this, "Down");
	m_labelDown->move(x+150, y);
	
	y+=24;

	for(int i=0; i<NRC; i++) {
		m_labelValue[i] = new UiLabel(this, QString::number(i));
		m_labelValue[i]->move(x, y);
		
		m_valueValueUp[i] = new UiValue(this, 50, 16, "", 0, 1200, 0, 1, 2);
		m_valueValueUp[i]->move(x+100, y);

		m_valueValueDown[i] = new UiValue(this, 50, 16, "", 0, 1200, 0, 1, 2);
		m_valueValueDown[i]->move(x+150, y);

		m_valueValueUp[i]->setValueTextColor(QColor(255,255,255));
		m_valueValueDown[i]->setValueTextColor(QColor(255,255,255));

		connect(m_valueValueUp[i], SIGNAL(valueChanged(float)), this, SLOT(setUp(float)));
		connect(m_valueValueDown[i], SIGNAL(valueChanged(float)), this, SLOT(setDown(float)));

		y+=24;
	}

	y+= 12;

	m_checkActivateOnMidi = new UiCheckBox(this);
	m_checkActivateOnMidi->move(x, y);
	m_checkActivateOnMidi->setText("Activate Channel Pitch Bend on MIDI");
	m_checkActivateOnMidi->setChecked(true);
	connect(m_checkActivateOnMidi, SIGNAL(changed(bool)), this, SLOT(setActivateOnMidi(bool)));

	setFixedSize(300,y+20);
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelPitchBendRemoteSetup::~SamGuiChannelPitchBendRemoteSetup()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelPitchBendRemoteSetup::setActive(bool state)
{
	samEngine->setChannelPitchBendRemoteActive(state);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelPitchBendRemoteSetup::setActivateOnMidi(bool state)
{
	samEngine->setChannelPitchBendRemoteActiveOnMidi(state);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelPitchBendRemoteSetup::setController(float value)
{
	samEngine->setChannelPitchBendRemoteController((int)value);
	m_labelControllerName->setText(QString("(")+CsMidiName::controllerToName(samEngine->channelPitchBendRemoteController(), 1).toChar()+QString(")"));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelPitchBendRemoteSetup::setUp(float value)
{
	for(int i=0; i<NRC; i++) {
		samEngine->setChannelPitchBendRemoteUp(i, (int)m_valueValueUp[i]->value());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelPitchBendRemoteSetup::setDown(float value)
{
	for(int i=0; i<NRC; i++) {
		samEngine->setChannelPitchBendRemoteDown(i, (int)m_valueValueDown[i]->value());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelPitchBendRemoteSetup::closeEvent(QCloseEvent *ev)
{
	samPreferences->engine()->setPreference("ChannelPitchBendRemoteActive", AString::number((int)samEngine->isChannelPitchBendRemoteActive()));
	samPreferences->engine()->setPreference("ChannelPitchBendRemoteActivateOnMidi", AString::number((int)samEngine->isChannelPitchBendRemoteActiveOnMidi()));
	samPreferences->engine()->setPreference("ChannelPitchBendRemoteController", AString::number(samEngine->channelPitchBendRemoteController()));
	for(int i=0; i<128; i++) {
		samPreferences->engine()->setPreference(AString("ChannelPitchBendRemoteUp")+AString::number(i), AString::number(samEngine->channelPitchBendRemoteUp(i)));
		samPreferences->engine()->setPreference(AString("ChannelPitchBendRemoteDown")+AString::number(i), AString::number(samEngine->channelPitchBendRemoteDown(i)));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelPitchBendRemoteSetup::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0, 0, width(), height(), QColor(70,70,70));
	p.fillRect(0, 0, width(), 36, QColor(50,50,50));

	QPen pen;
	pen.setBrush(QColor(90,90,90));
	p.setPen(pen);
	//p.drawLine(8, m_labelController->y()-12, width()-8, m_labelController->y()-12);

	QColor col[2] = {
		QColor(0,0,0,20),
		QColor(255,255,255,20)
	};

	for(int i=0; i<NRC; i++) {
		p.fillRect(0, m_labelValue[i]->y()-4, width(), m_labelValue[i]->height()+8, col[i%2]);
	}

	p.fillRect(0, m_labelTrigger->y()-4, 100, (NRC+1)*24, QColor(0,0,0,30));

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelPitchBendRemoteSetup::showEvent(QShowEvent *ev)
{
	m_checkActive->setChecked(samEngine->isChannelPitchBendRemoteActive());
	m_valueController->setValue(samEngine->channelPitchBendRemoteController());
	m_labelControllerName->setText(QString("(")+CsMidiName::controllerToName(samEngine->channelPitchBendRemoteController(), 1).toChar()+QString(")"));
	for(int i=0; i<NRC; i++) {
		m_valueValueUp[i]->setValue(samEngine->channelPitchBendRemoteUp(i));
		m_valueValueDown[i]->setValue(samEngine->channelPitchBendRemoteDown(i));
	}
	m_checkActivateOnMidi->setChecked(samEngine->isChannelPitchBendRemoteActiveOnMidi());
}