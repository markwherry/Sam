#include "SamGuiChannelSettings.h"

#include "SamEngine.h"
#include "SamProject.h"

#include <QPainter>

SamGuiChannelSettings::SamGuiChannelSettings(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
{
	//ui.setupUi(this);

	QPalette plt = palette();
	plt.setColor(QPalette::Background, QColor(60,60,60));
	setPalette(plt);
	setAutoFillBackground(true);
	setBackgroundRole(QPalette::Background);

	int y = 0;

	m_buttonDownbeatOffset = new UiStateButton(this, 1000, 2);
	m_buttonDownbeatOffset->move(4, y+=4);
	m_buttonDownbeatOffset->setFixedSize(16, 16);
	//m_buttonDownbeatOffset->setBackground();
	m_buttonDownbeatOffset->setStateImage(0, ":/Resources/downbeatoffsetb16.png");
	m_buttonDownbeatOffset->setStateImage(1, ":/Resources/downbeatoffsetb16.png");
	m_buttonDownbeatOffset->setText("Downbeat Offset");
	//m_buttonDownbeatOffset->setBackgroundColor(0,QColor(255,130,0,30));
	m_buttonDownbeatOffset->setStateFillColor(0,QColor(100,75,150));
	m_buttonDownbeatOffset->setStateFillColor(1,QColor(170,70,255));
	m_buttonDownbeatOffset->setStateLineColor(1,QColor(100,75,150));
	//m_buttonDownbeatOffset->setDrawTint();
	//m_buttonDownbeatOffset->setDrawOutline();

	connect(m_buttonDownbeatOffset, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setDownbeatOffset(int, int)));

	m_buttonLayerMutesEnabled = new UiStateButton(this, 1000, 2);
	m_buttonLayerMutesEnabled->move(4, y+=24);
	m_buttonLayerMutesEnabled->setFixedSize(16, 16);
	//m_buttonDownbeatOffset->setBackground();
	m_buttonLayerMutesEnabled->setStateImage(0, ":/Resources/layermutes16.png");
	m_buttonLayerMutesEnabled->setStateImage(1, ":/Resources/layermutes16.png");
	m_buttonLayerMutesEnabled->setText("Layer Mutes");
	//m_buttonLayerMutesEnabled->setBackgroundColor(0,QColor(255,130,0,30));
	m_buttonLayerMutesEnabled->setStateFillColor(0,QColor(100,75,150));
	m_buttonLayerMutesEnabled->setStateFillColor(1,QColor(170,70,255));
	m_buttonLayerMutesEnabled->setStateLineColor(1,QColor(100,75,150));
	//m_buttonLayerMutesEnabled->setDrawTint();
	//m_buttonLayerMutesEnabled->setDrawOutline();
	
	connect(m_buttonLayerMutesEnabled, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setLayerMutesEnabled(int, int)));

	m_buttonLayerMutesEnabled->hide();
	y-=24;

	m_buttonPitchBend = new UiStateButton(this, 1000, 2);
	m_buttonPitchBend->move(4, y+=24);
	m_buttonPitchBend->setFixedSize(16, 16);
	//m_buttonPitchBend->setBackground();
	m_buttonPitchBend->setStateImage(0, ":/Resources/pitchbend16.png");
	m_buttonPitchBend->setStateImage(1, ":/Resources/pitchbend16.png");
	m_buttonPitchBend->setText("Channel Pitch Bend");
	//m_buttonPitchBend->setBackgroundColor(0,QColor(255,130,0,30));
	m_buttonPitchBend->setStateFillColor(0,QColor(100,75,150));
	m_buttonPitchBend->setStateFillColor(1,QColor(170,70,255));
	m_buttonPitchBend->setStateLineColor(1,QColor(100,75,150));
	//m_buttonPitchBend->setDrawTint();
	//m_buttonPitchBend->setDrawOutline();
	
	connect(m_buttonPitchBend, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setPitchBend(int, int)));

	m_labelUp = new UiLabel(this, "Up");
	//m_labelUp->setImage(":/Resources/midi16.png");
	m_labelUp->move(150, y-1); // 32 +=4
	m_labelUp->setTextColor(QColor(180,180,180));

	m_labelDown = new UiLabel(this, "Down");
	//m_labelDown->setImage(":/Resources/midi16.png");
	m_labelDown->move(210, y-1); // 32 +=4
	m_labelDown->setTextColor(QColor(180,180,180));

	m_valueUp = new UiValue(this, 48, 16, "", 0, 1200, 200, 1, 2);
	m_valueUp->move(170, y-1);
	m_valueUp->setValueTextColor(QColor(220,220,220));
	connect(m_valueUp, SIGNAL(valueChanged(float)), this, SLOT(setUp(float)));

	m_valueDown = new UiValue(this, 48, 16, "", 0, 1200, 200, 1, 2);
	m_valueDown->move(246, y-1);
	m_valueDown->setValueTextColor(QColor(220,220,220));
	connect(m_valueDown, SIGNAL(valueChanged(float)), this, SLOT(setDown(float)));

	/*y+=22;
	
	m_audioToolInsert = new SamGuiAudioToolInsert(this, -1);
	m_audioToolInsert->move(2, y);
	//y += (m_insert[i]->height() + 2);*/


	setFixedSize(296, y+20);

	connect(this, SIGNAL(selectedChannelUpdated()), this, SLOT(updateSelectedChannel()));
	connect(this, SIGNAL(downbeatOffsetUpdated()), this, SLOT(updateDownbeatOffset()));
	connect(this, SIGNAL(layerMutesEnabledUpdated()), this, SLOT(updateLayerMutesEnabled()));

	connect(this, SIGNAL(pitchBendUpdated()), this, SLOT(updatePitchBend()));
	connect(this, SIGNAL(pitchBendUpdated()), this, SLOT(updateUp()));
	connect(this, SIGNAL(pitchBendUpdated()), this, SLOT(updateDown()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
	samEngine->registerListener(this, SamEngine::ContextProject);

	m_pixBackground = new QPixmap(":/Resources/black_linen_v2.png");
}

SamGuiChannelSettings::~SamGuiChannelSettings()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::onState(AStateEvent *ev)
{
	//qDebug("SamGuiChannelProperties::onState()");

	if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			case SamChannel::ItemSelected:
				emit selectedChannelUpdated();
				break;
			case SamChannel::ItemDownbeatOffset:
				emit downbeatOffsetUpdated();
				break;
			case SamChannel::ItemLayerMutesEnabled:
				qDebug("SamChannel::ItemLayerMutesEnabled:");
				emit layerMutesEnabledUpdated();
				break;
			case SamChannel::ItemPitchBend:
				qDebug("SamChannel::ItemPitchBend:");
				emit pitchBendUpdated();
				break;
		}
	} else if(ev->context() == SamEngine::ContextProject) {
		switch(ev->item()) {
			case SamProject::ItemChannelRemoved:
				emit selectedChannelUpdated();
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::updateSelectedChannel(void)
{
	//qDebug("SamGuiChannelProperties::updateSelectedChannel()");

	m_channel = samEngine->project()->selectedChannel();

	//m_audioToolInsert->setChannel(m_channel);

	if(!m_channel) {
		if(samEngine->project()->numSelectedChannels() > 1) {
			// Show for multiple
		} else {
			// ??? Should handle this elsewhere

		}
		setEnabled(false);

	} else {
		setEnabled(true);
		m_buttonDownbeatOffset->setState(m_channel->isDownbeatOffset());
		m_buttonLayerMutesEnabled->setState(m_channel->isLayerMutesEnabled());	
		m_buttonPitchBend->setState(m_channel->isPitchBend());
		m_valueUp->setValue(m_channel->pitchBendUp());
		m_valueDown->setValue(m_channel->pitchBendDown());
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::setDownbeatOffset(int id, int flags)
{
	if(m_channel) {
		m_channel->setDownbeatOffset(!m_channel->isDownbeatOffset(), this);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::setLayerMutesEnabled(int id, int flags)
{
	if(m_channel) {
		m_channel->setLayerMutesEnabled(!m_channel->isLayerMutesEnabled(), this);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::updateDownbeatOffset(void)
{
	if(m_channel) { 
		m_buttonDownbeatOffset->setState(m_channel->isDownbeatOffset());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::updateLayerMutesEnabled(void)
{
	if(m_channel) {
		m_buttonLayerMutesEnabled->setState(m_channel->isLayerMutesEnabled());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::setPitchBend(int id, int flags)
{
	if(!m_channel) return;

	m_channel->setPitchBend(!m_channel->isPitchBend(), this);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::setUp(float value)
{
	if(!m_channel) return;

	m_channel->setPitchBendUp((int)value);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::setDown(float value)
{
	if(!m_channel) return;

	m_channel->setPitchBendDown((int)value);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::updatePitchBend(void)
{
	if(!m_channel) return; 

	m_buttonPitchBend->setState(m_channel->isPitchBend());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::updateUp(void)
{
	if(!m_channel) return;

	m_valueUp->setValue(m_channel->pitchBendUp());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::updateDown(void)
{
	if(!m_channel) return;

	m_valueDown->setValue(m_channel->pitchBendDown());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSettings::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.drawPixmap(0,0,*m_pixBackground);
	p.fillRect(0,0,width(),height(),QColor(50,50,50));
}