#include "SamGuiChannelTools.h"

#include "SamEngine.h"
#include "SamProject.h"

#include <QPainter>

SamGuiChannelTools::SamGuiChannelTools(QWidget *parent)
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

	for(int i=0; i<SamChannel::maxMusicTools; i++) {
		m_insert[i] = new SamGuiMusicToolInsert(this);
		m_insert[i]->move(2, y);
		y += (m_insert[i]->height() + 2);
	}
	
	setFixedSize(296, y);

	connect(this, SIGNAL(selectedChannelUpdated()), this, SLOT(updateSelectedChannel()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
	samEngine->registerListener(this, SamEngine::ContextProject);

	m_pixBackground = new QPixmap(":/Resources/black_linen_v2.png");
}

// -----
SamGuiChannelTools::~SamGuiChannelTools()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelTools::onState(AStateEvent *ev)
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
void SamGuiChannelTools::updateSelectedChannel(void)
{
	//qDebug("SamGuiChannelProperties::updateSelectedChannel()");

	m_channel = samEngine->project()->selectedChannel();
	for(int i=0; i<SamChannel::maxMusicTools; i++) {
		m_insert[i]->setChannel(m_channel, i);
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
void SamGuiChannelTools::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.drawPixmap(0,0,*m_pixBackground);

	p.fillRect(0,0,width(),height(),QColor(50,50,50));
}