#include "SamGuiMixer.h"

#include "SamEngine.h"
#include "SamProject.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMixer::SamGuiMixer(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	samEngine->registerListener(this, SamEngine::ContextProject);
	samEngine->registerListener(this, SamEngine::ContextChannel);

	//connect(this, SIGNAL(channelAdded(SamChannel*)), this, SLOT(addChannel(SamChannel*)));
	//connect(this, SIGNAL(channelRemoved(SamChannel*)), this, SLOT(removeChannel(SamChannel*)));
	connect(this, SIGNAL(updated(void)), this, SLOT(update()));

	setFixedSize(0, 300);
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMixer::~SamGuiMixer()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixer::onState(AStateEvent *ev)
{
	//qDebug("***** SamGuiMixer::onState() *****");

	if(ev->context() == SamEngine::ContextProject) {

		switch(ev->item()) {
			case SamProject::ItemChannelAdded:
				//qDebug("SamProject::ItemChannelAdded:");
				emit channelAdded((SamChannel*)ev->updatedObject());
				break;
			case SamProject::ItemChannelRemoved:
				emit channelRemoved((SamChannel*)ev->updatedObject());
				break;
		}

	} else if(ev->context() == SamEngine::ContextChannel) {

		switch(ev->item()) {
			case SamChannel::ItemSelected:
				//update();
				//emit updated();
				break;
		}

	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixer::addChannel(SamChannel *channel, bool exclusive)
{
	qDebug("  *** SamGuiMixer::addChannel() width = %d", width());

	if(exclusive) {
		for(int i=0; i<m_strips.size(); i++) {
			m_strips[i]->hide();
			delete m_strips[i];
			m_strips[i] = 0;
		}
		m_strips.clear();
	}

	if(!channel) {
		return;
	}

	int x = 0;
	if(m_strips.size() > 0) {
		x = m_strips[m_strips.size()-1]->x() + m_strips[m_strips.size()-1]->width();
	}

	for(int i = 0; i < channel->numAudioOutputs(); i++) { aLog("   ** Bus %d/%d x = %d", i+1, channel->numAudioOutputs(), x);
		SamGuiMixerStrip *strip = new SamGuiMixerStrip(this, channel, i);
		//connect(strip, SIGNAL(selected()), this, SLOT(updateSelection()));
		strip->move(x, 0);
		strip->setVisible(true);
		m_strips.append(strip);

		x += strip->width();
	}

	x+=10;

	SamProject *project = (SamProject*)channel->parent();
	for(int i=0; i<project->numAudioBuses(); i++) {
		SamGuiMixerStrip *strip = new SamGuiMixerStrip(this, 0, i);
		strip->move(x,0);
		strip->setVisible(true);
		m_strips.append(strip);
		x += strip->width();
	}

	setFixedWidth(x); aLog("    * width = %d", width());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixer::removeChannel(SamChannel *channel)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMixer::updateSelection(void)
{
	qDebug("SamGuiMixer::updateSelection()");
	QObject *object = sender();

	for(int i=0; i<m_strips.size(); i++) { qDebug("%d/%d %d == %d", i+1, m_strips.size(), object, m_strips[i]);
		m_strips[i]->setSelected(object == m_strips[i]);
		if(object == m_strips[i]) {
			emit audioBusSelected(m_strips[i]->channel(), m_strips[i]->audioBusIndex());
		}
	}
}