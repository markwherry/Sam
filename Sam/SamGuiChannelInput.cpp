#include "SamGuiChannelInput.h"

#include "SamEngine.h"
#include "SamProject.h"

#include <QPainter>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelInput::SamGuiChannelInput(QWidget *parent)
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

	labelMidiPort = new UiLabel(this, "Port");
	labelMidiPort->setImage(":/Resources/midi16.png");
	labelMidiPort->move(4, y+=4); // 32
	labelMidiPort->setTextColor(QColor(180,180,180));

	selectMidiInput = new SamGuiSelectMidiInputBus(this, 0);
	selectMidiInput->setDrawFill(false);
	selectMidiInput->setDrawTint(false);
	selectMidiInput->setDrawLine(false);
	//selectMidiPort->setTextBold(false);
	//selectMidiPort->setTextColor(QColor(200,200,200));
	selectMidiInput->move(110,y);
	

	labelMidiChannel = new UiLabel(this, "Channel");
	labelMidiChannel->setImage(":/Resources/midichannelsolid16.png");
	labelMidiChannel->move(4, y+=24); //56
	labelMidiChannel->setTextColor(QColor(180,180,180));
	

	selectMidiChannel = new UiSelectMidiChannel(this);
	selectMidiChannel->setDrawFill(false);
	selectMidiChannel->setDrawTint(false);
	selectMidiChannel->setDrawLine(false);
	selectMidiChannel->move(110, y); //56
	selectMidiChannel->setFixedWidth(160);

	buttonMidiMuted = new UiStateButton(this, 1000, 2);
	buttonMidiMuted->move(4, y+=24);
	buttonMidiMuted->setFixedSize(16, 16);
	//buttonMidiMuted->setBackground();
	buttonMidiMuted->setStateImage(0, ":/Resources/panic16.png");
	buttonMidiMuted->setStateImage(1, ":/Resources/panic16.png");
	buttonMidiMuted->setText("MIDI Input Mute / Panic");
	//buttonMidiMuted->setBackgroundColor(0,QColor(255,130,0,30));
	buttonMidiMuted->setStateFillColor(0,QColor(150,100,100));
	buttonMidiMuted->setStateFillColor(1,QColor(250,100,100));
	buttonMidiMuted->setStateLineColor(1,QColor(150,100,100));
	//buttonMidiMuted->setDrawTint();
	//buttonMidiMuted->setDrawOutline();

	connect(buttonMidiMuted, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setMidiMuted(int, int)));

	setFixedSize(296, y+20);

	//connect(selectMidiPort, SIGNAL(itemSelected(int)), this, SLOT(setMidiPort(int)));
	connect(selectMidiChannel, SIGNAL(itemSelected(int)), this, SLOT(setMidiChannel(int)));

	connect(this, SIGNAL(selectedChannelUpdated()), this, SLOT(updateSelectedChannel()));
	//connect(this, SIGNAL(midiPortUpdated()), this, SLOT(updateMidiPort()));
	connect(this, SIGNAL(midiChannelUpdated()), this, SLOT(updateMidiChannel()));
	connect(this, SIGNAL(midiMutedUpdated()), this, SLOT(updateMidiMuted()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
	samEngine->registerListener(this, SamEngine::ContextProject);

	m_pixBackground = new QPixmap(":/Resources/black_linen_v2.png");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelInput::~SamGuiChannelInput()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelInput::onState(AStateEvent *ev)
{
	if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			case SamChannel::ItemSelected:
				emit selectedChannelUpdated();
				break;
			case SamChannel::ItemMidiInputChannel:
				if((ev->updatedObject() == m_channel || samEngine->project()->numSelectedChannels() > 1) && ev->fromObject() != this) {
					emit midiChannelUpdated();
				}
				break;
			case SamChannel::ItemMidiMuted:
				if(ev->updatedObject() == m_channel) {
					emit midiMutedUpdated();
				}
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
void SamGuiChannelInput::setMidiChannel(int itemIndex)
{
	//qDebug("SamGuiChannelProperties::setMidiChannel(%d)", itemIndex);

	if(m_channel || samEngine->project()->numSelectedChannels() > 1) {
		for(int i=0; i<samEngine->project()->numChannels(); i++) {
			SamChannel *channel = samEngine->project()->channel(i);
			if(channel->isSelected()) {
				channel->setMidiInputChannel(itemIndex, this);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelInput::updateSelectedChannel(void)
{
	//qDebug("SamGuiChannelProperties::updateSelectedChannel()");

	//if(m_channel

	m_channel = samEngine->project()->selectedChannel();
	selectMidiInput->setParentChannel(m_channel);
	updateMidiChannel();

	if(!m_channel) {
		if(samEngine->project()->numSelectedChannels() > 1) {
			// Show for multiple
		} else {
			// ??? Should handle this elsewhere

		}
	} else {
		
		
		buttonMidiMuted->setState(m_channel->isMidiMuted());	
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelInput::updateMidiChannel(void)
{
	if(m_channel) {
		selectMidiChannel->setSelectedItem(m_channel->midiInputChannel());
	} else {
		SamChannel *channel = samEngine->project()->firstSelectedChannel();
		if(channel) {
			selectMidiChannel->setSelectedItem(channel->midiInputChannel());
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelInput::updateMidiMuted(void)
{
	if(m_channel) {
		buttonMidiMuted->setState(m_channel->isMidiMuted());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelInput::setMidiMuted(int id, int flags)
{
	if(m_channel) {
		m_channel->setMidiMuted(!m_channel->isMidiMuted(), this);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelInput::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.drawPixmap(0,0,*m_pixBackground);
	p.fillRect(0,0,width(),height(),QColor(50,50,50));
}