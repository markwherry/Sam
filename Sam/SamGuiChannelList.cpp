#include "SamGuiChannelList.h"

#include "SamEngine.h"
#include "SamProject.h"

#include "SamGuiChannel.h"

SamGuiChannelList::SamGuiChannelList(QWidget *parent)
	: UiList(parent)
{
	//ui.setupUi(this);

	samEngine->registerListener(this, SamEngine::ContextProject);
	samEngine->registerListener(this, SamEngine::ContextChannel);

	connect(this, SIGNAL(channelAdded(SamChannel*)), this, SLOT(addChannel(SamChannel*)));
	connect(this, SIGNAL(channelRemoved(SamChannel*)), this, SLOT(removeChannel(SamChannel*)));
	connect(this, SIGNAL(updated(void)), this, SLOT(update()));

	startTimer(100);
}

SamGuiChannelList::~SamGuiChannelList()
{

}

void SamGuiChannelList::onState(AStateEvent *ev)
{
	//qDebug("SamGuiChannelList::onState() ev=%d", ev);

	if(ev->context() == SamEngine::ContextProject) {

		switch(ev->item()) {
			case SamProject::ItemChannelAdded:
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
				emit updated();
				break;
		}

	}
}

void SamGuiChannelList::updateLabels(void)
{
	for(int i=0; i<numItems(); i++) {
		SamGuiChannel* item = (SamGuiChannel*)itemFromIndex(i);
		item->refreshLabel();
	}
}

void SamGuiChannelList::updateArticulations(void)
{
	for(int i=0; i<numItems(); i++) {
		SamGuiChannel* item = (SamGuiChannel*)itemFromIndex(i);
		item->updateArticulationList();
	}
}

void SamGuiChannelList::addChannel(SamChannel *channel)
{
	//qDebug("SamGuiChannelList::addChannel()");

	SamGuiChannel *item = new SamGuiChannel(this, channel);
	addItem(item);
	update();
}

void SamGuiChannelList::removeChannel(SamChannel *channel)
{
	qDebug("SamGuiChannelList::removeChannel()");
	for(int i=0; i<numItems(); i++) {
		SamGuiChannel *c = (SamGuiChannel*)itemFromIndex(i);
		if(c->channel() == channel) {
			removeItem(c);
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiChannelList::timerEvent(QTimerEvent *ev)
{
	for(int i=0; i<numItems(); i++) {
		SamGuiChannel* item = (SamGuiChannel*)itemFromIndex(i);
		item->updateNextFrame();
	}
}