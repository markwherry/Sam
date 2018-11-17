#include "SamGuiSelectMidiInputBus.h"

#include "SamEngine.h"
#include "SamProject.h"

SamGuiSelectMidiInputBus::SamGuiSelectMidiInputBus(QWidget *parent, SamChannel *channel)
	: UiSelectMenu(parent, 160)
	, m_channel(channel)
{
	//ui.setupUi(this);
	
	refresh();

	connect(this, SIGNAL(itemSelected(QAction*)), this, SLOT(selectInput(QAction*)));

	connect(this, SIGNAL(midiPortUpdated()), this, SLOT(updateMidiPort()));

	samEngine->registerListener(this, SamEngine::ContextChannel);
}

SamGuiSelectMidiInputBus::~SamGuiSelectMidiInputBus()
{
	samEngine->unregisterListener(this, SamEngine::ContextChannel);
}

void SamGuiSelectMidiInputBus::onState(AStateEvent *ev)
{
	//qDebug("SamGuiSelectMidiInput::onState()");

	if(ev->item() == SamChannel::ItemMidiInputPort) {
		if((m_channel == 0 || ev->updatedObject() == m_channel) && ev->fromObject() != this) {
			emit midiPortUpdated();
		}
	}
}

void SamGuiSelectMidiInputBus::refresh(void)
{
	qDebug("SamGuiSelectMidiInput::refresh()");

	clear();
	m_inputActions.clear();

	m_actionNoInput = addItem("None");

	addSeparator();

	for(int i=0; i<samEngine->numMidiInputBuses(); i++) {
		QAction *action = addItem(samEngine->midiInputBus(i)->name().toChar());
		m_inputActions.append(action);
	}
}

void SamGuiSelectMidiInputBus::setParentChannel(SamChannel *channel)
{
	m_channel = channel;

	updateMidiPort();

	/*if(m_channel) {
		selectMidiPort->setSelectedItem(m_channel->midiInputPort());
	} else {
		SamChannel *channel = samEngine->project()->firstSelectedChannel();
		if(channel) {
			selectMidiPort->setSelectedItem(channel->midiInputPort());
		}
	}*/
}

void SamGuiSelectMidiInputBus::selectInput(QAction *action)
{
	qDebug("SamGuiSelectMidiInput::selectInput()");
	
	int bus = 0;
	
	for(int i=0; i<m_inputActions.size(); i++) {
		if(action == m_inputActions[i]) {
			bus = samEngine->midiInputBus(i)->id();	
		}
	}

	if(samEngine->project()->numSelectedChannels() > 1) {
		for(int i=0; i<samEngine->project()->numChannels(); i++) {
			SamChannel *channel = samEngine->project()->channel(i);
			if(channel->isSelected()) {
				channel->setMidiInputPort(bus, this);
			}
		}
	} else {
		if(m_channel) {
			m_channel->setMidiInputPort(bus, this);
		}
	}
}

void SamGuiSelectMidiInputBus::updateMidiPort(void)
{
	qDebug("SamGuiSelectMidiInput::updateMidiPort()");

	SamChannel *channel = m_channel;
	if(!channel) {
		channel = samEngine->project()->firstSelectedChannel();
	}
	if(!channel) {
		return;
	}

	if(channel) {
		if(channel->midiInputPort() == 0) {
			setSelectedAction(m_actionNoInput);
		} else {
			for(int i=0; i<samEngine->numMidiInputBuses(); i++) {
				if(samEngine->midiInputBus(i)->id() == channel->midiInputPort()) {
					setSelectedAction(m_inputActions[i]);
				}
			}
		}
	}
	update();
}