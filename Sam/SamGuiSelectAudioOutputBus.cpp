#include "SamGuiSelectAudioOutputBus.h"

#include "SamEngine.h"
#include "SamProject.h"
#include "SamRealTimeCommands.h"

SamGuiSelectAudioOutputBus::SamGuiSelectAudioOutputBus(QWidget *parent, SamAudioBus *audioBus, SamAudioSend *audioSend)
	: UiSelectMenu(parent, 160)
	, m_audioBus(audioBus)
	, m_audioSend(audioSend)
{
	//ui.setupUi(this);

	refresh();

	samEngine->registerListener(this, SamEngine::ContextProject);
	samEngine->registerListener(this, SamEngine::ContextAudioBus);

	//connect(this, SIGNAL(audioOutputUpdated()), this, SLOT(refresh()));
	connect(this, SIGNAL(audioOutputsUpdated()), this, SLOT(refresh()));
	connect(this, SIGNAL(itemSelected(QAction*)), this, SLOT(selectBus(QAction*)));
}

SamGuiSelectAudioOutputBus::~SamGuiSelectAudioOutputBus()
{
	samEngine->unregisterListener(this, SamEngine::ContextProject);
	samEngine->unregisterListener(this, SamEngine::ContextAudioBus);
}

void SamGuiSelectAudioOutputBus::onState(AStateEvent *ev)
{
	//qDebug("SamGuiSelectAudioBus::onState()");

	if(ev->context() == SamEngine::ContextProject) {
		//qDebug("SamEngine::ContextProject");
		switch(ev->item()) {
			case SamProject::ItemAudioHardwareBusAdded:
			case SamProject::ItemAudioHardwareBusRemoved:
			case SamProject::ItemAudioBusAdded:
			case SamProject::ItemAudioBusRemoved:
				//qDebug("SamProject::ItemAudioHardwareBusAdded:");
				emit audioOutputsUpdated();
				break;
		}
	} else if(ev->context() == SamEngine::ContextAudioBus) {
		//qDebug("SamEngine::ContextAudioBus");
		switch(ev->item()) {
			case SamAudioBus::ItemOutputBusId:
				qDebug("SamGuiSelectAudioOutputBus::ItemOutputBusId:");
				if(ev->fromObject() != this && ev->updatedObject() == m_audioBus) {
					qDebug("-->audioOutputUpdated()");
					emit audioOutputsUpdated();
				}
				break;
		}
	}

	//ev->fromObject() != this)
}

void SamGuiSelectAudioOutputBus::setParentAudioBus(SamAudioBus *audioBus, SamAudioSend *audioSend)
{
	m_audioBus = audioBus;
	m_audioSend = audioSend;
	
	updateAudioOutput();
}

void SamGuiSelectAudioOutputBus::refresh(void)
{
	qDebug("SamGuiSelectAudioBus::refresh()");

	clear();
	m_busActions.clear();
	m_outActions.clear();

	actionNoOutput = addItem("None");

	SamProject *project = samEngine->project(); 

	if(samEngine->project()->numAudioHardwareBuses() > 0) {
		addItem("----- Outputs -----", false);
		for(int i=0; i<samEngine->project()->numAudioHardwareBuses(); i++) {
			//qDebug("Adding hardware bus %d/%d", i, samEngine->project()->numAudioHardwareBuses());
			QAction *action = addItem(samEngine->project()->audioHardwareBus(i)->name().toChar());
			m_outActions.append(action);
			if(samEngine->project()->audioHardwareBus(i)==m_audioBus) {
				action->setEnabled(false);
			}
			if(m_audioBus) {
				if(!m_audioSend) {
					if(m_audioBus->audioOutputBusId() == samEngine->project()->audioHardwareBus(i)->id()) {
						qDebug("*****> m_audioBus->audioOutputBusId() %d == samEngine->project()->audioHardwareBus(i)->id()", m_audioBus->audioOutputBusId(), samEngine->project()->audioHardwareBus(i)->id());
						setSelectedAction(action);
					}
				} else {
					if(m_audioSend->audioBusId() == samEngine->project()->audioHardwareBus(i)->id()) {
						//qDebug("*****> m_audioSend->audioBusId() == samEngine->project()->audioHardwareBus(i)->id()");
						setSelectedAction(action);
					}
				}
			}
		}
	}

	if(m_audioSend) {
		if(samEngine->project()->numAudioBuses() > 0) {
			addItem("----- Auxs -----", false);
			for(int i=0; i<samEngine->project()->numAudioBuses(); i++) {
				QAction *action = addItem(project->audioBus(i)->name().toChar());
				m_busActions.append(action);

				// Don't select yourself
				if(samEngine->project()->audioBus(i)==m_audioBus) {
					action->setEnabled(false);
				}

				// Set selected
				if(m_audioBus) {
					if(!m_audioSend) {
						if(m_audioBus->audioOutputBusId() == samEngine->project()->audioBus(i)->id()) {
							//qDebug("*****> m_audioBus->audioOutputBusId() %d == samEngine->project()->audioHardwareBus(i)->id()", m_audioBus->audioOutputBusId(), samEngine->project()->audioHardwareBus(i)->id());
							setSelectedAction(action);
						}
					} else {
						if(m_audioSend->audioBusId() == samEngine->project()->audioBus(i)->id()) {
							//qDebug("*****> m_audioSend->audioBusId() == samEngine->project()->audioHardwareBus(i)->id()");
							setSelectedAction(action);
						}
					}
				}
			}
		}
	}

	update();
}

void SamGuiSelectAudioOutputBus::selectBus(QAction *action)
{
	//qDebug("SamGuiSelectAudioBus::selectBus()");
	
	if(action == actionNoOutput) {
		if(!m_audioSend) {
			samEngine->executeRealTimeCommand(new RtcSetAudioBusOutputId(m_audioBus, 0, this));
		} else {
			// This is a kludge and should be removed as soon as possible!
			SamChannel *channel = (SamChannel*)m_audioBus->parent();
			for(int j = 0; j<channel->numAudioOutputs(); j++) {
				samEngine->executeRealTimeCommand(new RtcSetSendOutput(channel->audioOutputBus(j), m_audioSend->id(), 0));
			}
			// End kludge

			//samEngine->executeRealTimeCommand(new RtcSetSendOutput(m_audioBus, m_audioSend->id(), 0));
		}

		return;
	}

	/*for(int i=0; i<m_busActions.size(); i++) {
		if(action == m_busActions[i]) {
			if(!m_audioSend) {
				csEngine->executeRealTimeCommand(new CsRtcSetAudioBusOutputId(csEngine->project(), m_audioBus, csEngine->project()->audioBus(i)->id()));
			} else {
				csEngine->executeRealTimeCommand(new CsRtcSetSendOutput(csEngine->project(), m_audioBus, m_audioSend->id(), csEngine->project()->audioBus(i)));
			}
		}
	}*/

	for(int i=0; i<m_outActions.size(); i++) {
			if(action == m_outActions[i]) {
				if(!m_audioSend) {
					samEngine->executeRealTimeCommand(new RtcSetAudioBusOutputId(m_audioBus, samEngine->project()->audioHardwareBus(i)->id(), this));
				} else {
					// This is a kludge and should be removed as soon as possible!
					SamChannel *channel = (SamChannel*)m_audioBus->parent();
					for(int j = 0; j<channel->numAudioOutputs(); j++) {
						samEngine->executeRealTimeCommand(new RtcSetSendOutput(channel->audioOutputBus(j), m_audioSend->id(), samEngine->project()->audioHardwareBus(i)));
					}
					// End kludge

					// samEngine->executeRealTimeCommand(new RtcSetSendOutput(m_audioBus, m_audioSend->id(), samEngine->project()->audioHardwareBus(i)));
				} 
			
			}
		} 

	for(int i=0; i<m_busActions.size(); i++) {
		if(action == m_busActions[i]) { aLog("AUX MATCH");
			if(!m_audioSend) {
				samEngine->executeRealTimeCommand(new RtcSetAudioBusOutputId(m_audioBus, samEngine->project()->audioBus(i)->id(), this));
			} else {
				// This is a kludge and should be removed as soon as possible!
				SamChannel *channel = (SamChannel*)m_audioBus->parent();
				for(int j = 0; j<channel->numAudioOutputs(); j++) {
					samEngine->executeRealTimeCommand(new RtcSetSendOutput(channel->audioOutputBus(j), m_audioSend->id(), samEngine->project()->audioBus(i)));
				}
				// End kludge
			} 
			
		}
	}
}

void SamGuiSelectAudioOutputBus::updateAudioOutput(void)
{
	//qDebug("SamGuiSelectAudioOutputBus::updateAudioOutput()");

	if(!m_audioBus) {
		return;
	}

	if(!m_audioSend) { //qDebug("1");
		if(m_audioBus->audioOutputBusId()==0) { //qDebug("2");
			setSelectedAction(actionNoOutput); //qDebug("3");
		} else { //qDebug("4");
			for(int i=0; i<m_outActions.size(); i++) { //qDebug("5");		
				if(m_audioBus->audioOutputBusId() == samEngine->project()->audioHardwareBus(i)->id()) { //qDebug("6");
					setSelectedAction(m_outActions[i]); //qDebug("7");
				}
			}
			for(int i=0; i<m_busActions.size(); i++) { //qDebug("5");		
				if(m_audioBus->audioOutputBusId() == samEngine->project()->audioBus(i)->id()) { //qDebug("6");
					setSelectedAction(m_busActions[i]); //qDebug("7");
				}
			}
		}
	} else {
		if(m_audioSend->audioBusId()==0) {
			setSelectedAction(actionNoOutput);
		} else {
			for(int i=0; i<m_outActions.size(); i++) {	
				if(m_audioSend->audioBusId() == samEngine->project()->audioHardwareBus(i)->id()) {
					setSelectedAction(m_outActions[i]);
				}
			}
			for(int i=0; i<m_busActions.size(); i++) {	
				if(m_audioSend->audioBusId() == samEngine->project()->audioBus(i)->id()) {
					setSelectedAction(m_busActions[i]);
				}
			}
		}
	}
}