#include "SamProject.h"
#include "SamEngine.h"
#include "SamRealTimeCommands.h"
#include "SamCommands.h"

#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamProject::SamProject(AModel *parent)
	: AModel(parent)
	, m_lastCreatedAudioHardwareBusId(100)
	, m_lastCreatedAudioBusId(1000)
	, m_tempo(120.0f)
{
	m_commandManager = new ACommandManager();

	//for(int i=0; i<128; i++) {
	//	for(int j=0; j<16; j++) {
	//		m_numChannelMidiAssignments[i][j] = 0;
	//		m_firstChannelMidiAssignment[i][j] = -1;
	//	}
	//}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamProject::~SamProject()
{
	delete m_commandManager;

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamProject::clear(void)
{
	aLog("SamProject::clear()");

	for(int i=0; i<m_channels.size(); i++) {
		samEngine->executeRealTimeCommand(new RtcRemoveChannel(m_channels[i]));
	}

	for(int i=0; i<m_audioHardwareBuses.size(); i++) {
		samEngine->executeRealTimeCommand(new RtcRemoveAudioHardwareBus(m_audioHardwareBuses[i]));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamProject::addChannel(SamChannel *channel)
{
	aLog("SamProject::addChannel()");

	channel->addRef();
	m_channels.append(channel);

	//updateChannelMidiAssignments();

	updateState(SamEngine::ContextProject, ItemChannelAdded, channel, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamProject::removeChannel(SamChannel *channel)
{
	aLog("SamProject::removeChannel()");

	for(int i=0; i<m_channels.size(); i++) {
		if(m_channels[i] == channel) {
			m_channels.removeAt(i);
			channel->release();
			aLog("Channel removed. (refcount = %d)", channel->refCount());
			break;
		}
	}


	updateState(SamEngine::ContextProject, ItemChannelRemoved, channel, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamChannel* SamProject::selectedChannel(void)
{
	SamChannel *channel = 0;

	for(int i=0; i<m_channels.size(); i++) {
		if(m_channels[i]->isSelected()) {
			if(channel) {
				return 0;
			}
			channel = m_channels[i];
		}
	}

	return channel;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamChannel* SamProject::firstSelectedChannel(void)
{
	for(int i=0; i<m_channels.size(); i++) {
		if(m_channels[i]->isSelected()) {
			return m_channels[i];
		}
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamProject::numSelectedChannels(void)
{
	int selectedChannels = 0;

	for(int i=0; i<m_channels.size(); i++) {
		if(m_channels[i]->isSelected()) {
			selectedChannels++;
		}
	}

	return selectedChannels;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamProject::deselectChannels(void)
{
	for(int i=0; i<m_channels.size(); i++) {
		if(m_channels[i]->isSelected()) {
			m_channels[i]->setSelected(false);
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamProject::addAudioBus(SamAudioBus *audioBus)
{
	m_audioBuses.append(audioBus);

	updateState(SamEngine::ContextProject, ItemAudioBusAdded, audioBus, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamProject::removeAudioBus(SamAudioBus *audioBus)
{
	aLog("SamProject::removeAudioBus()");

	for(int i=0; i<m_audioBuses.size(); i++) {
		if(m_audioBuses[i] == audioBus) {
			m_audioBuses.removeAt(i);
			aLog("Audio Bus removed.");
			break;
		}
	}


	updateState(SamEngine::ContextProject, ItemAudioBusRemoved, audioBus, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamAudioHardwareBus* SamProject::audioHardwareBusFromId(int id)
{
	for(int i=0; i<m_audioHardwareBuses.size(); i++) {
		if(m_audioHardwareBuses[i]->id() == id) {
			return m_audioHardwareBuses[i];
		}
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamProject::audioHardwareBusIndexFromId(int id)
{
	for(int i=0; i<m_audioHardwareBuses.size(); i++) {
		if(m_audioHardwareBuses[i]->id() == id) {
			return i;
		}
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamProject::addAudioHardwareBus(SamAudioHardwareBus *audioHardwareBus)
{
	m_audioHardwareBuses.append(audioHardwareBus);

	updateState(SamEngine::ContextProject, ItemAudioHardwareBusAdded, audioHardwareBus, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamProject::removeAudioHardwareBus(SamAudioHardwareBus *audioHardwareBus)
{
	aLog("SamProject::removeAudioHardwareBus()");

	for(int i=0; i<m_audioHardwareBuses.size(); i++) {
		if(m_audioHardwareBuses[i] == audioHardwareBus) {
			m_audioHardwareBuses.removeAt(i);
			aLog("Audio Hardware Bus removed.");
			break;
		}
	}


	updateState(SamEngine::ContextProject, ItemAudioHardwareBusRemoved, audioHardwareBus, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamProject::nextAudioHardwareBusStartStream(void)
{
	//CsEngine *engine = (CsEngine*)parent();

	int stream = 0;

	if(m_audioHardwareBuses.size()>0) {
		int ns = m_audioHardwareBuses[m_audioHardwareBuses.size()-1]->numStreams();
		stream = m_audioHardwareBuses[m_audioHardwareBuses.size()-1]->streamHardwareChannel(ns-1)+1; 
		//CSDEBUG("stream=%d", stream);
//		if(stream >= samEngine->audioManager()->numOutputs())
//			stream = 0;
		//CSDEBUG("stream=%d", stream);
	}

	return stream;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamProject::nextAudioHardwareBusId(void)
{
	m_lastCreatedAudioHardwareBusId++;
	
	bool unique = false;
	while(!unique) {
		aLog("!");
		unique = true;
		for(int i=0; i<m_audioHardwareBuses.size(); i++) {
			if(m_lastCreatedAudioHardwareBusId == m_audioHardwareBuses[i]->id()) {
				unique = false;
			}
		}
		if(!unique) {
			m_lastCreatedAudioHardwareBusId++;
		}
	}

	aLog("m_lastCreatedAudioHardwareBusId = %d", m_lastCreatedAudioHardwareBusId);

	return m_lastCreatedAudioHardwareBusId;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamProject::nextAudioBusId(void)
{
	m_lastCreatedAudioBusId++;
	
	bool unique = false;
	while(!unique) {
		aLog("!");
		unique = true;
		for(int i=0; i<m_audioBuses.size(); i++) {
			if(m_lastCreatedAudioBusId == m_audioBuses[i]->id()) {
				unique = false;
			}
		}
		if(!unique) {
			m_lastCreatedAudioBusId++;
		}
	}

	aLog("m_lastCreatedAudioBusId = %d", m_lastCreatedAudioBusId);

	return m_lastCreatedAudioBusId;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamProject::calculateAudioBusDependencies(void)
{
	aLog("SamProject::calculateAudioBusDependencies()");
/*
	// Calculate Output dependencies where Channels are routed to outputs.
	for(int i=0; i<numAudioHardwareBuses(); i++) {
		//csDebug("output [%d/%d]", i, m_project->numAudioOutputBuses());
//		audioHardwareBus(i)->clearDependencies();
		for(int j=0; j<numChannels(); j++) {
			//csDebug("channel [%d/%d]", j, m_project->numChannels());
			
				//SamChannel *channel = (SamChannel*)channel(j);
				for(int k=0; k<channel(j)->numAudioOutputs(); k++) {
					aLog("bus[%d] = %d [%d]", k, channel(j)->audioOutputBus(k)->audioOutputBusId(), audioHardwareBus(i)->id());
					if(channel(j)->audioOutputBus(k)->audioOutputBusId()==audioHardwareBus(i)->id() || channel(j)->audioOutputBus(k)->doesBusSendToAudioBusId(audioHardwareBus(i)->id())) {
						audioHardwareBus(i)->addDependency(channel(j));
						aLog("Bus %d is dependent on a channel.", i);
					}
				}
			
		}

		//for(int j=0; j<numAudioBuses(); j++) {
		//	if(audioBus(j)->audioOutputBusId() == hardwareAudioBus(i)->id() || audioBus(j)->doesBusSendToAudioBusId(hardwareAudioBus(i)->id())) {
		//		hardwareAudioBus(i)->addDependency(audioBus(j));
		//	}
		//}
	}*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamProject::numChannelMidiAssignemnts(int inputBusIndex, int midiChannel)
{
	int assignments = 0; 

	for(int i=0; i<m_channels.size(); i++) {
		int inputBus = m_channels[i]->midiInputPort();
		if(samEngine->midiInputBusIndex(inputBus) == inputBusIndex && m_channels[i]->midiInputChannel() == midiChannel+1) {
			assignments++;
		}
	}

	return assignments;
}

/*void SamProject::updateChannelMidiAssignments(void)
{
	// Reset
	for(int i=0; i<128; i++) {
		for(int j=0; j<16; j++) {
			m_numChannelMidiAssignments[i][j] = 0;
			m_firstChannelMidiAssignment[i][j] = -1;
		}
	}

	// Calculate
	for(int i=0; i<m_channels.size(); i++) {
		int inputBus        = m_channels[i]->midiInputPort();
		int inputIndex     = samEngine->midiInputBusIndex(inputBus);
		int inputChannel = m_channels[i]->midiInputChannel() - 1;

		if(inputChannel > -1) {
			m_numChannelMidiAssignments[inputIndex][inputChannel]++;
			if(m_numChannelMidiAssignments[inputIndex][inputChannel] == 1) {
				m_firstChannelMidiAssignment[inputIndex][inputChannel] = i;
				//aLog("***** ASSIGNED [%d] %d, %d", i, inputIndex, inputChannel);
			}
		}
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamProject::isEmpty(void)
{
	if(m_channels.size() == 0 && m_audioHardwareBuses.size() == 0) {
		return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamProject::setTempo(float tempo, void *from, bool shouldUpdate)
{
	m_tempo = tempo;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamProject::updateSolo(void)
{

}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamProject::numChannelsSolo(void)
{
	int nc = 0;

	for(int i=0; i<m_channels.size(); i++) {
		if(m_channels[i]->isSolo()) {
			nc++;
		}
	}

	return nc;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamProject::readElement(AXmlDocument *document, AXmlElement *element, ACommand *command)
{
	aLog("---------- SamProject::readElement() ----------");

	

	for(int i=0; i<element->numChildElements(); i++) {
		AXmlElement *el = element->childElement(i);

		// Read Tempo
		if(el->tag() == "Tempo") {
			m_tempo = el->attribute("bpm", "120").toFloat();
			aLog("Tempo = %f", m_tempo);

		// Read Instruments
		// ----------------
		} else if(el->tag() == "Instruments") {
			aLog("---------- Loading instruments ----------");
			AList<AString> *filenames = new AList<AString>;
			for(int i=0; i<el->numChildElements(); i++) {
				aLog("Instrument %d/%d", i+1, el->numChildElements());
				AXmlElement *elIns = el->childElement(i);
				if(elIns->hasAttribute("filename")) {
					aLog("Has filename attribute [%s]", elIns->attribute("filename").toChar());
					filenames->append(elIns->attribute("filename"));
					
				}
			}
			LoadInstruments *loadInstruments = new LoadInstruments(filenames, filenames->size(), false);
			//aLog("here");
			loadInstruments->execute(command->commandManager());

			if(loadInstruments->isFeedback()) {
				command->setFeedback(true);
				command->setMessage(loadInstruments->message());
				command->setMessageType(loadInstruments->messageType());
			}
			//aLog("here2");

		// Read Hardware Buses
		// -------------------
		} else if(el->tag() == "HardwareBuses") {
			aLog("---------- Loading Hardware Buses ----------");
			aLog("childElements = %d", el->numChildElements());
			for(int i=0; i<el->numChildElements(); i++) {
				AXmlElement *elAhb = el->childElement(i);
				aLog("%d/%d = [%s]", i+1, el->numChildElements(), elAhb->tag().toChar());
				SamAudioHardwareBus *audioHardwareBus = new SamAudioHardwareBus(this, elAhb->attribute("id").toInt(), elAhb->attribute("name"), elAhb->attribute("streams").toInt(), CsSpeakerConfig(elAhb->attribute("speakerConfig")));
				audioHardwareBus->setAudioOutputBusId(elAhb->attribute("audioOutputBusId").toInt());
				audioHardwareBus->readElement(document, el->childElement(i));
				audioHardwareBus->setAudioProcessInfo(samEngine->audioProcessInfo());
				samEngine->executeRealTimeCommand(new RtcAddAudioHardwareBus(audioHardwareBus));
			}

		// Read Channels
		// -------------
		} else if(el->tag() == "Channels") {
			aLog("---------- Loading Channels ----------");
			for(int i=0; i<el->numChildElements(); i++) {
				//SamChannel *channel = new SamChannel(samEngine->project());
				//samEngine->executeRealTimeCommand(new RtcAddChannel(channel));

				AXmlElement *elCh = el->childElement(i);
				if(elCh->hasAttribute("instrumentUuid")) {
					SamInstrument *instrument = samEngine->instrumentFromUuid(elCh->attribute("instrumentUuid"));
					if(instrument) {
						SamChannel *channel = new SamChannel(this);
						if(elCh->hasAttribute("midiInputChannel")) {
							channel->setMidiInputChannel(elCh->attribute("midiInputChannel").toInt(), 0, false);
						}
						if(elCh->hasAttribute("midiInputPort")) {
							channel->setMidiInputPort(elCh->attribute("midiInputPort").toInt(), 0, false);
						}

						channel->setMidiMuted((bool)elCh->attribute("midiMuted", "0").toInt(), 0, false);

						int voices = -1;
						if(elCh->hasAttribute("voices")) {
							voices = elCh->attribute("voices").toInt();
						}
						channel->setAudioProcessInfo(samEngine->audioProcessInfo());
						channel->assignInstrument(instrument, 0, voices);
						channel->setArticulationMode((bool)elCh->attribute("articulationMode", "0").toInt());
						//if(elCh->hasAttribute("currentArticulation")) {
							channel->setCurrentArticulationIndex(elCh->attribute("currentArticulation", "0").toInt());
						//}
						
						
						channel->setCurrentDefaultArticulationIndex(elCh->attribute("currentDefaultArticulation", "0").toInt());
						channel->setDefaultCfArticulations(elCh->attribute("defaultCfArticulationIndex", "defaultCfDefaultArticulationIndex").toInt(), elCh->attribute("", "0").toInt());

						if(elCh->hasAttribute("downbeatOffset")) {
							channel->setDownbeatOffset(elCh->attribute("downbeatOffset").toInt());
						}

						if(elCh->hasAttribute("label")) channel->setLabel(elCh->attribute("label"));

						
						channel->setArticulationLock((bool)elCh->attribute("articulationLock", "0").toInt());

						for(int j=0; j<elCh->numChildElements(); j++) {
							if(elCh->childElement(j)->tag() == "PitchBend") {
								aLog("Reading PitchBend");
								AXmlElement *elPb = elCh->childElement(j);
								channel->setPitchBend((bool)elPb->attribute("enabled", "0").toInt());
								channel->setPitchBendUp(elPb->attribute("up", "200").toInt());
								channel->setPitchBendDown(elPb->attribute("down", "200").toInt());
							} else if(elCh->childElement(j)->tag() == "MidiNoteAttenuation") {
								aLog("*** Reading <MidiNoteAttenuation>");
								AXmlElement* eAtt = elCh->childElement(j);
								channel->setAttenuationStart(eAtt->attribute("attenuationStart", "21").toInt());
								channel->setAttenuationEnd(eAtt->attribute("attenuationEnd", "109").toInt());
								channel->setAttenuationFadeIn(eAtt->attribute("attenuationFadeIn", "0").toInt());
								channel->setAttenuationFadeOut(eAtt->attribute("attenuationFadeOut", "0").toInt());
								channel->setAttenuationFadeInQ(eAtt->attribute("attenuationFadeInQ", "0.5").toFloat());
								channel->setAttenuationFadeOutQ(eAtt->attribute("attenuationFadeOutQ", "0.5").toFloat());
								channel->updateAttenuationsForMidiNotes();
							} else if(elCh->childElement(j)->tag() == "StreamMix") {
								SamStreamMix *streamMix = new SamStreamMix(0);
								streamMix->readElement(document, elCh->childElement(j));
								channel->setStreamMix(streamMix);
								delete streamMix;
							} else if(elCh->childElement(j)->tag() == "OutputBuses") {
								for(int k=0; k<elCh->childElement(j)->numChildElements(); k++) {
									aLog("Reading OutputBus");
									AXmlElement *elBus = elCh->childElement(j)->childElement(k);
									if(elBus->hasAttribute("audioOutputBusId")) {
										channel->audioOutputBus(k)->setAudioOutputBusId(elBus->attribute("audioOutputBusId").toInt());
									}
									for(int l=0; l<elBus->numChildElements(); l++) {
										if(elBus->childElement(l)->tag() == "Insert") {
											aLog("Reading Insert");
											AXmlElement *elInsert = elBus->childElement(l);
											int insertId = elInsert->attribute("id", "-1").toInt();
											if(insertId > -1) {
												channel->audioOutputBus(k)->audioInsert(insertId)->readElement(document, elInsert);
											}
										} else if(elBus->childElement(l)->tag() == "Send") {
											aLog("Reading Send %d", l);
											AXmlElement *elSend = elBus->childElement(l);
											int sendId = elSend->attribute("id", "-1").toInt();
											if(sendId > -1) {
												channel->audioOutputBus(k)->audioSend(sendId)->readElement(document, elSend);
											}
										}
									}
								}
							} else if(elCh->childElement(j)->tag() == "MusicToolInserts") {
								for(int k=0; k<elCh->childElement(j)->numChildElements(); k++) {
									aLog("Reading MusicToolInsert");
									AXmlElement *elMti = elCh->childElement(j)->childElement(k);
									AString uuid = elMti->attribute("uuid");
									SamMusicTool *tool = (SamMusicTool*)samEngine->musicToolManager()->createInstance(uuid, channel->musicToolInsert(k));
									channel->musicToolInsert(k)->setTool(tool);
									AXmlElement *elMt = elMti->childElement(0);
									tool->readState(document, elMt);

									channel->musicToolInsert(k)->setActiveController(elMti->attribute("activeController", "-1").toInt());
									channel->musicToolInsert(k)->setActive(elMti->attribute("active", "1").toInt());
								}
							} else if(elCh->childElement(j)->tag() == "ArticulationCrossfadeCurve") {
								AXmlElement *elArtCf = elCh->childElement(j);
								channel->curveArticulationCrossfade()->setShape(elArtCf->attribute("shape").toInt());
								channel->curveArticulationCrossfade()->setDepth(elArtCf->attribute("q").toFloat());
								channel->curveArticulationCrossfade()->setStart(elArtCf->attribute("start").toFloat());
								channel->curveArticulationCrossfade()->setEnd(elArtCf->attribute("end").toFloat());
							} /*else if(elCh->childElement(j)->tag() == "PerVoiceFx") {
								aLog("===========================================================================");
								aLog("***************************************************************************");
								aLog("===========================================================================");
								AXmlElement *elPerVoiceFx = elCh->childElement(j);
								channel->setPerVoiceFxTool(elPerVoiceFx->attribute("audioToolUuid"));
								channel->setPerVoiceVelocitySplit(elPerVoiceFx->attribute("velocitySplit", "81").toInt());
								for(int k=0; k<elPerVoiceFx->numChildElements(); k++) {
									for(int l=0; l<8; l++) {
										if(elPerVoiceFx->childElement(k)->tag() == AString("PresetGroup")+AString::number(l)) {
											AXmlElement *elPr = elPerVoiceFx->childElement(k);
											for(int m=0; m<elPr->numChildElements(); m++) {
												if(elPr->childElement(m)->tag() == "Parameter") {
													AXmlElement *elP = elPr->childElement(m);
													channel->setPerVoiceParameter(l, elP->attribute("id").toInt(), elP->attribute("value").toFloat());
												}
											}
											//break;
										}
									}
									for(int l=0; l<2; l++) {
										if(elPerVoiceFx->childElement(k)->tag() == AString("VelocityPresetMap")+AString::number(l)) {
											AXmlElement *elV = elPerVoiceFx->childElement(k);
											for(int m=0; m<128; m++) {
												channel->setPerVoiceGroup(m, l, elV->attribute(AString("p")+AString::number(m)).toInt());
											}
											//break;
										}
									}
								}
								aLog("===========================================================================");
								aLog("***************************************************************************");
								aLog("===========================================================================");
							}*/
						}

						samEngine->executeRealTimeCommand(new RtcAddChannel(channel));
					}
				}

				/**/
			}
		}
	}

	

	// Purge unused instruments - Engine - use this command every time a channel is deleted

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamProject::writeElement(AXmlDocument *document, AXmlElement *element)
{
	AXmlElement *elTempo = document->createElement("Tempo");
	element->addChildElement(elTempo);
	elTempo->setAttribute("bpm", m_tempo);

	AXmlElement *elHardwareBuses = document->createElement("HardwareBuses");
	element->addChildElement(elHardwareBuses);
	for(int i=0; i<m_audioHardwareBuses.size(); i++) {
		AXmlElement *elHardwareBus = document->createElement("HardwareBus");
		m_audioHardwareBuses[i]->writeElement(document, elHardwareBus);
		elHardwareBuses->addChildElement(elHardwareBus);
	}

	//elBuses = document->createElement("Buses");
	//element->addChildElement(elBuses);

	AXmlElement *elChannels = document->createElement("Channels");
	element->addChildElement(elChannels);

	for(int i=0; i<m_channels.size(); i++) {
		for(int j=0; j<m_channels.size(); j++) {
			if(m_channels[j]->positionInList() == i) {
				AXmlElement *elChannel = document->createElement("Channel");
				m_channels[j]->writeElement(document, elChannel);
				elChannels->addChildElement(elChannel);
				break;
			}
		}
	}

	return 1;
}