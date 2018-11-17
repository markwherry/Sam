#include "SamAudioToolInsert.h"

#include "SamEngine.h"
#include "SamChannel.h"

#include "AUtility.h"

#include <strsafe.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamAudioToolInsert::SamAudioToolInsert(AModel *parent, int index, CsSpeakerConfig speakerConfig)
	: SamToolInsert(parent, 0, index)
	, m_speakerConfig(speakerConfig)
	, m_numInstances(0)
	, m_inputBuffer(0)
	, m_outputBuffer(0)
	, m_link(true)
	, m_currentSpeaker(0)
	, m_mode(0)
	, m_midiInput(false)
	
{
	for(int i=0; i<8; i++) {
		m_audioTool[i] = 0;
		m_vstPlugin[i] = 0;
	}

	m_vstEvents = (VstEvents*)malloc(sizeof(VstEvent) + (1024 * sizeof(VstEvent)));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioToolInsert::parameterChanged(int id, float value, void *from)
{
	//aLog("SamAudioToolInsert::parameterChanged()");

	//if(m_index < 0) {
	//	SamChannel *channel = (SamChannel*)parent();
	//	/*for(int i=0; i<channel->numVoices(); i++) {
	//		for(int j=0; j<channel->voice(i)->audioToolInsert()->numInstances(); j++) {
	//			channel->voice(i)->audioToolInsert()->audioTool(j)->setParameterValueNormalised(id, value);
	//		}
	//	}*/
	//	
	//	channel->setPerVoiceParameter(channel->currentPerVoiceGroup(), id, value);
	//} else {
		if(m_audioTool[0]) {
			if(m_numInstances == 1) {
				m_audioTool[0]->setParameterValueNormalised(id, value);
			} else if(m_numInstances > 1) {
				if(m_link) { //aLog("Link");
					for(int i=0; i<m_numInstances; i++) {
						m_audioTool[i]->setParameterValueNormalised(id, value);
					}
				} else { //aLog("Speaker: %d", m_currentSpeaker);
					m_audioTool[m_currentSpeaker]->setParameterValueNormalised(id, value);;
				}
			}
		} else if(m_vstPlugin[0]) {
			if(m_link) { 
				for(int i=0; i<m_numInstances; i++) { 
					if(i != m_currentSpeaker) { 
					//if((AEffect*)from != m_vstPlugin[i]->effect()) {
						if(!m_vstPlugin[i]) aLog("!m_vstPlugin[%d]", i);
						if(!m_vstPlugin[i]->effect()) aLog("!m_vstPlugin[%d]->effect()", i);
						m_vstPlugin[i]->effect()->setParameter(m_vstPlugin[i]->effect(), id, value);
					} //else aLog("From the source!");
				}
			} 
		}
	//}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioToolInsert::clear(void)
{
	m_active = false;

	for(int i=0; i<8; i++) {
		if(m_audioTool[i]) {
			delete m_audioTool[i];
			m_audioTool[i] = 0;
		}

		if(m_vstPlugin[i]) {
			m_vstPlugin[i]->close();
			delete m_vstPlugin[i];
			m_vstPlugin[i] = 0;
		}
	}

	m_numInstances = 0;
	m_link = true;
	m_currentSpeaker = 0;
	m_mode = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioToolInsert::setAudioTool(AString toolUuid, int mode)
{
	aLog("SamAudioToolInsert::setAudioTool(%s, %s, %d) %d", toolUuid.toChar(), m_speakerConfig.name().toChar(), mode, m_speakerConfig.numSpeakers());

	clear();

	m_mode = mode;

	if(toolUuid.length() < 1) {
		SamToolInsert::setTool(0);
		return;
	}

	m_audioTool[0] = (SamAudioTool*)samEngine->audioToolManager()->createInstance(toolUuid, this);

	//if(!m_audioTool) {
	//}

	m_audioTool[0]->init();
	m_audioTool[0]->setAudioProcessInfo(samEngine->audioProcessInfo());
	m_audioTool[0]->setSpeakerConfig(m_speakerConfig, m_speakerConfig);

	if(m_audioTool[0]->speakerConfig() == m_speakerConfig && mode == 0) {
		m_numInstances = 1;
	} else if((m_audioTool[0]->speakerConfig() != m_speakerConfig && mode == 0) || mode == 1) {
		aLog("Can't do speaker config (or mode = 1), so attempting multi-mono");
		m_audioTool[0]->setSpeakerConfig(CsSpeakerConfig(CsSpeakerConfig::Mono), CsSpeakerConfig(CsSpeakerConfig::Mono));
		if(m_audioTool[0]->speakerConfig() == CsSpeakerConfig(CsSpeakerConfig::Mono)) {
			// Sucess
			aLog("%d = %d", m_numInstances, m_speakerConfig.numSpeakers());
			m_numInstances = m_speakerConfig.numSpeakers();
			aLog("%d = %d", m_numInstances, m_speakerConfig.numSpeakers());
			for(int i=1; i<m_numInstances; i++) {
				m_audioTool[i] = (SamAudioTool*)samEngine->audioToolManager()->createInstance(toolUuid, 0);;

				m_audioTool[i]->init();
				m_audioTool[i]->setAudioProcessInfo(samEngine->audioProcessInfo());
				m_audioTool[i]->setSpeakerConfig(CsSpeakerConfig(CsSpeakerConfig::Mono), CsSpeakerConfig(CsSpeakerConfig::Mono));
				// And this time we don't check if can do mono, since we already know!
			}
		} else {
			aLog("ERROR: can't do multi-mono");
		}
	}

	aLog("m_numInstances = %d", m_numInstances);

	// Set-up Buffers
	// --------------
	
	if(m_inputBuffer) {
		if(m_inputBuffer->buffer32) {
			free(m_inputBuffer->buffer32);
		}
		delete m_inputBuffer;
	}

	if(m_outputBuffer) {
		if(m_outputBuffer->buffer32) {
			free(m_outputBuffer->buffer32);
		}
		delete m_outputBuffer;
	}

	m_inputBuffer = new CsSampleBuffer;
	m_inputBuffer->buffer32 = (float**)malloc(sizeof(float*)*m_numInstances);
	m_outputBuffer = new CsSampleBuffer;
	m_outputBuffer->buffer32 = (float**)malloc(sizeof(float*)*m_numInstances);


	


	//setActive(true);

	SamToolInsert::setTool((SamTool*)m_audioTool[0]);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioToolInsert::setVstPlugin(TCHAR *filename, int mode)
{
	AString fn(filename);
	aLog("SamAudioToolInsert::setVstPlugin(%s) mode = %d", fn.toChar(), mode);

	clear();

	m_mode = mode;

	// Initialise the plug-in
	m_vstPlugin[0] = new CsVstPlugin(samEngine, filename);
	
	if(!m_vstPlugin[0]->open()) {
		delete m_vstPlugin[0];
		m_vstPlugin[0] = 0;
		aLog("Couldn't open plug-in");
		return;
	}

	// Figure out if we can do MIDI
	if(m_vstPlugin[0]->canDo("receiveVstEvents") == 1) {
		aLog(">>> Plug-in CAN receive Vst Events on %d channels.", m_vstPlugin[0]->getNumMidiInputChannels());
		m_midiInput = true;
	} else {
		aLog(">>> Plug-in CANNOT receive Vst Events on %d channels.", m_vstPlugin[0]->getNumMidiInputChannels());
	}

	// Sort out speaker configuration
	/*VstSpeakerArrangement *plugInSpeakerArrangement;
	VstSpeakerArrangement *plugOutSpeakerArrangement;
	
	if(m_vstPlugin[0]->getSpeakerArrangement(&plugInSpeakerArrangement, &plugOutSpeakerArrangement)) { 
		aLog("plugInSpeakerArrangement->type        = %d", plugInSpeakerArrangement->type);
		aLog("plugInSpeakerArrangement->numChannels = %d", plugInSpeakerArrangement->numChannels);
		aLog("plugOutSpeakerArrangement->type        = %d", plugInSpeakerArrangement->type);
		aLog("plugOutSpeakerArrangement->numChannels = %d", plugInSpeakerArrangement->numChannels);
	} else aLog("Could not get the speaker arrangement %d", m_vstPlugin[0]->getVstVersion());*/

	VstSpeakerArrangement speakerArrangement;
	speakerArrangement.numChannels = m_speakerConfig.numSpeakers(); aLog("speakerArrangement.numChannels = %d", speakerArrangement.numChannels);
	switch(m_speakerConfig.config()) {
		case CsSpeakerConfig::Undefined:
			speakerArrangement.type = kSpeakerArrUserDefined;
			break;
		case CsSpeakerConfig::Mono:
			speakerArrangement.type = kSpeakerArrMono;
			break;
		case CsSpeakerConfig::Stereo: aLog("CsSpeakerConfig::Stereo:");
			speakerArrangement.type = kSpeakerArrStereo;
			speakerArrangement.speakers[0].type = kSpeakerL;
			speakerArrangement.speakers[1].type = kSpeakerR;
			break;
		case CsSpeakerConfig::Surround40:
			speakerArrangement.type = kSpeakerArr40Music;
			break;
		case CsSpeakerConfig::Surround50:
			speakerArrangement.type = kSpeakerArr50;
			break;
		case CsSpeakerConfig::Surround51:
			speakerArrangement.type = kSpeakerArr51;
			break;
	}

	VstSpeakerArrangement sai, sao;
	memset(&sai,0,sizeof(VstSpeakerArrangement));
	memset(&sao,0,sizeof(VstSpeakerArrangement));
	sai.type = kSpeakerArrStereo;
	sai.numChannels = 2;
	sai.speakers[0].type = kSpeakerL;
	sai.speakers[1].type = kSpeakerR;
	sao.type = kSpeakerArrStereo;
	sao.numChannels = 2;
	sao.speakers[0].type = kSpeakerL;
	sao.speakers[1].type = kSpeakerR;

	aLog("m_vstPlugin[0]->setSpeakerArrangement(&sai, &sao) = %d", (int)m_vstPlugin[0]->setSpeakerArrangement(&sai, &sao));

	if(m_vstPlugin[0]->setSpeakerArrangement(&speakerArrangement, &speakerArrangement) && m_mode == 0) {
		aLog("Can do speaker config");
		m_numInstances = 1;
		samEngine->addPluginToInsert(m_vstPlugin[0]->effect(), this);
	} else {
		aLog("Cannot do speaker config.");
		bool multiStereo = false;
		if(m_mode == 2 || (m_speakerConfig.numSpeakers() % 2 == 0 && m_mode != 1)) {
			speakerArrangement.numChannels = 2;
			speakerArrangement.type = kSpeakerArrStereo;
			if(m_vstPlugin[0]->setSpeakerArrangement(&speakerArrangement, &speakerArrangement)) {
				aLog("Can do multi-stereo");
				multiStereo = true;
				
				for(int i = 1; i < (m_speakerConfig.numSpeakers() / 2); i++) {
					m_vstPlugin[i] = new CsVstPlugin(samEngine, filename);
					m_vstPlugin[i]->open();
					m_vstPlugin[i]->setSpeakerArrangement(&speakerArrangement, &speakerArrangement);
					samEngine->addPluginToInsert(m_vstPlugin[i]->effect(), this);
				}
				m_numInstances = m_speakerConfig.numSpeakers() / 2;
				samEngine->addPluginToInsert(m_vstPlugin[0]->effect(), this);
				m_mode = 2;
				
			} else aLog("Can't do multi-stereo, try mono...");
		}

		if(!multiStereo) {
			speakerArrangement.numChannels = 1;
			speakerArrangement.type = kSpeakerArrMono;
			if(m_vstPlugin[0]->setSpeakerArrangement(&speakerArrangement, &speakerArrangement)) {
				aLog("Can do multi-mono");
				for(int i = 1; i < m_speakerConfig.numSpeakers(); i++) {
					m_vstPlugin[i] = new CsVstPlugin(samEngine, filename);
					m_vstPlugin[i]->open();
					m_vstPlugin[i]->setSpeakerArrangement(&speakerArrangement, &speakerArrangement);
					samEngine->addPluginToInsert(m_vstPlugin[i]->effect(), this);
				}
				m_numInstances = m_speakerConfig.numSpeakers();
				samEngine->addPluginToInsert(m_vstPlugin[0]->effect(), this);
				m_mode = 1;
				
			} else {
				aLog("Give up!");
				clear();
			}
		}
	}
	

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioToolInsert::process(CsAudioBuffer *inputs, CsAudioBuffer *outputs, int numInputs, int numOutputs, double timestamp, double samples)
{
	//aLog("SamAudioToolInsert::process()");

	if(!m_active) return; // Just incase!

	if(m_audioTool[0]) {
		m_inputBuffer->numStreams = inputs->numStreams();
		m_inputBuffer->numFrames  = inputs->numFrames();

		m_outputBuffer->numStreams = outputs->numStreams();
		m_outputBuffer->numFrames  = outputs->numFrames();

		if(m_numInstances == 1) {
			for(int i=0; i<m_inputBuffer->numStreams; i++) {
				m_inputBuffer->buffer32[i]  = inputs->stream(i);
				m_outputBuffer->buffer32[i] = outputs->stream(i);
			}
			m_audioTool[0]->process(m_inputBuffer, m_outputBuffer, 1, 1, 0, 0);
		} else if(m_numInstances > 1) {
			for(int i=0; i<m_numInstances; i++) {
				//csDebug("*%d/%d-%d", i, inputs->numStreams(), outputs->numStreams());
				m_inputBuffer->buffer32[0] = inputs->stream(i); //csDebug("*A");
				m_outputBuffer->buffer32[0] = outputs->stream(i);   //csDebug("*B");
				m_audioTool[i]->process(m_inputBuffer, m_outputBuffer, 1, 1, 0, 0); //csDebug("*C");
			}
		}
	} else {
		if(m_numInstances == 1) {
			m_vstPlugin[0]->processReplacing(inputs->streams(), outputs->streams(), inputs->numFrames());
		} else if(m_numInstances > 1) {
			if(m_mode == 2) { // Multi-Stereo
				int c = 0;
				for(int i=0; i<m_numInstances; i++) {
					m_multiInput[0] = inputs->stream(c);
					m_multiOutput[0] = outputs->stream(c++);
					m_multiInput[1] = inputs->stream(c);
					m_multiOutput[1] = outputs->stream(c++);
					m_vstPlugin[i]->processReplacing(m_multiInput, m_multiOutput, inputs->numFrames());
				}
			} else if(m_mode == 1) { // Multi-mono
				for(int i=0; i<m_numInstances; i++) {
					m_multiInput[0] = inputs->stream(i);
					m_multiOutput[0] = outputs->stream(i);
					m_vstPlugin[i]->processReplacing(m_multiInput, m_multiOutput, inputs->numFrames());
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioToolInsert::processEvents(SamEvent *events, int numEvents)
{
	//if(!m_active) return; // Just incase!

	if(m_vstPlugin[0]) {

		

		// Convert SamEvents to VstEvents

		//VstEvents *vstEvents = (VstEvents*)malloc(sizeof(VstEvent) + (numEvents * sizeof(VstEvent)));
		m_vstEvents->numEvents = numEvents;

		int count = 0;
		for(int i=0; i<numEvents; i++) {

			if(events[i].type == SamEvent::Controller) {
				
				if(isActiveLearn()) {
					setActiveController(events[i].midiValue1);
					setActiveLearn(false);
				}
				
				if(events[i].midiValue1 == activeController()) { 
					//aLog("ev->midiValue2 = %d", ev->midiValue2); 
					if(events[i].midiValue2 == 0) { 
						//m_bypassed = true;
						setActive(true);
					} else { 
						//m_bypassed = false;
						setActive(false);
					}
				}
			}

			m_vstMidiEventBuffer[count].type            = kVstMidiType;
			m_vstMidiEventBuffer[count].byteSize        = sizeof(VstMidiEvent);
			m_vstMidiEventBuffer[count].deltaFrames     = events[i].delta;
			m_vstMidiEventBuffer[count].flags           = 0;
			m_vstMidiEventBuffer[count].noteLength      = 0;
			m_vstMidiEventBuffer[count].noteOffset      = 0;
			m_vstMidiEventBuffer[count].midiData[0]     = events[i].type;
			m_vstMidiEventBuffer[count].midiData[1]     = events[i].midiValue1;
			m_vstMidiEventBuffer[count].midiData[2]     = events[i].midiValue2;
			m_vstMidiEventBuffer[count].midiData[3]     = 0;
			m_vstMidiEventBuffer[count].detune          = 0;
			m_vstMidiEventBuffer[count].noteOffVelocity = 0;

			m_vstEvents->events[count] = (VstEvent*) &m_vstMidiEventBuffer[count];

			count++;
		}

		if(m_active) {
			for(int i=0; i<m_numInstances; i++) {
				m_vstPlugin[i]->processEvents(m_vstEvents);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioToolInsert::process(CsSampleBuffer *inputs, CsSampleBuffer *outputs, int numInputs, int numOutputs, double timestamp, double samples)
{
	//aLog("SamAudioToolInsert::process() %d [%d]", (int)m_active, m_numInstances);

	if(!m_active) return; // Just incase!

	if(m_numInstances == 1) {
	
	} else if(m_numInstances > 1) {
		m_inputBuffer->numStreams = 1;
		m_inputBuffer->numFrames  = inputs->numFrames;

		m_outputBuffer->numStreams = 1;
		m_outputBuffer->numFrames  = outputs->numFrames;

		for(int i=0; i<m_numInstances; i++) {
			m_inputBuffer->buffer32[0] = inputs->buffer32[i];
			m_outputBuffer->buffer32[0] = outputs->buffer32[i];
			m_audioTool[i]->process(m_inputBuffer, m_outputBuffer, 1, 1, 0, 0); //csDebug("*C");
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioToolInsert::resetTool(void)
{
	for(int i=0; i<m_numInstances; i++) {
		m_audioTool[i]->stop();
		m_audioTool[i]->start();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*CsSpeakerConfig SamAudioToolInsert::speakerConfig(void)
{
	if(m_audioTool[0]) {
		return m_audioTool[0]->speakerConfig();
	}

	return CsSpeakerConfig(CsSpeakerConfig::Undefined);
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString SamAudioToolInsert::name(void)
{
	if(m_audioTool[0]) {
		return m_audioTool[0]->name();
	} else if(m_vstPlugin[0]) {
		char text[256]; 
		m_vstPlugin[0]->getHostProductString(text); 
		return AString(text);
	}

	return AString("");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioToolInsert::setLink(bool link)
{
	m_link = link;
	//m_currentSpeaker = 0; // We need to do this, otherwise the VSTs don't work correctly when relinked if something other than the first editor is visible

	if(m_vstPlugin[0] && m_numInstances > 1) {
		if(m_vstPlugin[0]->programsAreChunks()) {
			void *data = 0;
			VstInt32 size = m_vstPlugin[m_currentSpeaker]->getChunk(&data);
			aLog("*** CHUNK SIZE = %d", size);

			if(size > 0) {
				for(int i=0; i<m_numInstances; i++) {
					if(i != m_currentSpeaker) {
						m_vstPlugin[i]->setChunk(data, size);
					}
				}
			}
		} else {
			// TODO: Copy parameters between instances (if we find a plug-in where this is necessary...)
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioToolInsert::setActive(bool active, void* from)
{
	//aLog("SamAudioToolInsert::setActive()");

	if(m_audioTool[0]) { //aLog("Is Autio Tool");
		m_active = active;

	} else if(m_vstPlugin[0]) { //aLog("Is VST");
		if(m_active != active) {
			m_active = active;
			if(m_active) {
				for(int i=0; i<m_numInstances; i++) {
					m_vstPlugin[i]->resume();
					m_vstPlugin[i]->startProcess();
				}
			} else {
				for(int i=0; i<m_numInstances; i++) {
					m_vstPlugin[i]->stopProcess();
					m_vstPlugin[i]->suspend();
				}
			}
		}

	} else { //aLog("Is Nothing!");
		m_active = false;
	}

	//aLog("->SamMusicToolInsert::setActive(%d)", (int)active);

	updateState(SamEngine::ContextAudioTool, ItemActive, this, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamAudioToolInsert::isAssigned(void)
{
	if(m_audioTool[0]) {
		return SamToolInsert::isAssigned();
	} else if(m_vstPlugin[0]) {
		return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamAudioToolInsert::readElement(AXmlDocument *document, AXmlElement *element)
{
	//m_active = (bool)element->attribute("active", "0").toInt();
	//m_link = (bool)element->attribute("link", "1").toInt();

	if(element->numChildElements() > 0) {
		if(element->childElement(0)->tag() == "AudioTool") {
		} else if (element->childElement(0)->tag() == "VST") {
			TCHAR *filename = samEngine->vstPluginManager()->filenameFromUniqueId((VstInt32)element->attribute("uniqueId", "0").toInt());
			if(filename) {
				// Get the filename and set the plug-in
				TCHAR fn[256];
				StringCchCopy(fn, 256, filename);
				setVstPlugin(fn, element->attribute("mode", "0").toInt());

				// Set Link mode
				setLink((bool)element->attribute("link", "1").toInt());

				// Recall the presets
				if(m_numInstances == element->numChildElements()) {
					for(int i = 0; i<element->numChildElements(); i++) {
						AXmlElement *elVst = element->childElement(i);

						if((bool)elVst->attribute("chunk").toInt()) {

							if(elVst->hasInnerText()) {
								aLog("VST hasInnerText! %d", elVst->innerText().length());
								aLog("inner[%s]", elVst->innerText().toChar());
								void *data = (void*)AUtility::decodeBase16(elVst->innerText().toChar(), elVst->innerText().length());
								QByteArray ba((char*)data, strlen((char*)data));
								aLog("[%s]", ba.data());
							
								int size = AUtility::decodeBase16Size(elVst->innerText().toChar(), elVst->innerText().length());

								VstInt32 ret = m_vstPlugin[i]->setChunk(data, size);
								aLog("ret = %d, size = %d", ret, sizeof(data));
							}
						} else {
							// TODO: Implement reading of parameters
						}
					}
				} else {
					aLog("ERRROR: not enough preset data.");
				}


				// Set active
				setActive((bool)element->attribute("active", "0").toInt());
			}

			
		}
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamAudioToolInsert::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("id", index());

	element->setAttribute("active", (int)m_active);
	element->setAttribute("link", (int)m_link);
	//element->setAttribute("speakerConfig", 
	element->setAttribute("mode", (int)m_mode);

	if(m_audioTool[0]) {
		element->setAttribute("name", m_audioTool[0]->name());
		element->setAttribute("uuid", m_audioTool[0]->uuid());
		
		//element->setAttribute("activeController", m_activeController);
		AXmlElement *elAudioTool = document->createElement("AudioTool");
		m_audioTool[0]->writeState(document, elAudioTool);
		element->addChildElement(elAudioTool);
	} else if(m_vstPlugin[0]) {
		char text[256]; 
		m_vstPlugin[0]->getHostProductString(text); 
		element->setAttribute("name", text);
		element->setAttribute("uniqueId", AString::number(m_vstPlugin[0]->uniqueId()));

		for(int i=0; i<m_numInstances; i++) {
			AXmlElement *elVst = document->createElement("VST");
			
			elVst->setAttribute("instance", AString::number(i));

			if(m_vstPlugin[0]->programsAreChunks()) {
				elVst->setAttribute("chunk", "1");

				void *data = 0;
				VstInt32 size = m_vstPlugin[i]->getChunk(&data);
				if(size > 0) {
					AString data16(AUtility::encodeBase16((unsigned char*)data, size, 64));
					elVst->setInnerText(data16);
				}

				elVst->setAttribute("size", AString::number(size));
			} else {
				elVst->setAttribute("chunk", "0");

				// TODO: Implement saving of parameters
			}

			element->addChildElement(elVst);
		}
	}

	return 1;
}