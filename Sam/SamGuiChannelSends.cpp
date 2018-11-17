#include "SamGuiChannelSends.h"

#include <QPainter>

#include "SamEngine.h"
#include "SamProject.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelSends::SamGuiChannelSends(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
{
	//ui.setupUi(this);

	QPalette plt = palette();
	plt.setColor(QPalette::Background, QColor(60,60,60));
	setPalette(plt);
	setAutoFillBackground(true);
	setBackgroundRole(QPalette::Background);
	
	int y = 2;

	for(int i=0; i<SamAudioBus::maxSends; i++) {
		m_sends[i] = new SamGuiSend(this, i);
		m_sends[i]->move(2, y);
		y += (m_sends[i]->height() + 2);
	}
	
	setFixedSize(296, y);

	connect(this, SIGNAL(selectedChannelUpdated()), this, SLOT(updateSelectedChannel()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
	samEngine->registerListener(this, SamEngine::ContextProject);

	m_pixBackground = new QPixmap(":/Resources/black_linen_v2.png");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiChannelSends::~SamGuiChannelSends()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSends::onState(AStateEvent *ev)
{
	//qDebug("SamGuiChannelProperties::onState()");

	if(ev->context() == SamEngine::ContextChannel) {
		switch(ev->item()) {
			case SamChannel::ItemSelected:
				emit selectedChannelUpdated();
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
void SamGuiChannelSends::updateSelectedChannel(void)
{
	//qDebug("SamGuiChannelProperties::updateSelectedChannel()");

	m_channel = samEngine->project()->selectedChannel();
	for(int i=0; i<SamAudioBus::maxSends; i++) {
		m_sends[i]->setChannel(m_channel, i);
	}

	if(!m_channel) {
		if(samEngine->project()->numSelectedChannels() > 1) {
			// Show for multiple
		} else {
			// ??? Should handle this elsewhere

		}
		setEnabled(false);

	} else {
		setEnabled(true);
		
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelSends::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.drawPixmap(0,0,*m_pixBackground);
	p.fillRect(0,0,width(),height(),QColor(50,50,50));
}