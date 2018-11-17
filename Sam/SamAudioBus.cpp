#include "SamAudioBus.h"

#include "SamEngine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamAudioBus::SamAudioBus(AModel *parent, int id, AString name, int streams, CsSpeakerConfig speakerConfig)
	: SamBus(parent, id, name)
	, m_project(samEngine->project())
	, m_streams(streams)
	, m_speakerConfig(speakerConfig)
	, m_hardware(false)
	, m_audioBuffer(0)
	, m_audioBufferPreFader(0)
	, m_audioOutputBusId(0)
	, m_testTone(false)
	, m_filterActive(false)
{
	aLog("===== SamAudioBus::SamAudioBus() speakerConfig.numSpeakers() = %d", speakerConfig.numSpeakers());
	//m_osc = new CsOscillator(44100.0);
	//m_oscBuffer = new CsSampleBuffer();
	//m_oscBuffer->buffer32 = (float**)malloc(sizeof(float*)*streams);
	//m_oscBuffer->numFrames = 0;
	//m_oscBuffer->numStreams = streams;

	for(int i=0; i<maxInserts; i++) {
		m_audioToolInserts[i] = new SamAudioToolInsert(this, i, speakerConfig);
	}

	for(int i=0; i<maxSends; i++) {
		m_audioSends[i] = new SamAudioSend(this, this, i);
	}

	m_filter = new CsSynthFilter(44100.0, streams);
	m_filter->setCutOff(22050.0);

	m_processBuffer = new CsSampleBuffer();
	m_processBuffer->numStreams = m_streams;

	m_volumeModulator = new CsModulator(44100.0, samEngine->audioProcessInfo().bufferSize, 32, true, 256, 1.0f);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamAudioBus::~SamAudioBus()
{
	//free(m_oscBuffer->buffer32);
	//delete m_oscBuffer;
	//delete m_osc;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioBus::main(void)
{
	//aLog("SamAudioBus::main() m_streams = %d", m_streams);

	if(m_audioBuffer) {
		if(m_filterActive) {
			//aLog("Filter Active");
			m_filter->renderBuffer(m_processBuffer);
		}

		for(int i=0; i<maxInserts; i++) {
			if(m_audioToolInserts[i]->isActive()) {
				m_audioToolInserts[i]->process(m_audioBuffer, m_audioBuffer,1, 1, 0, 0);
			}
		}

		if(!m_hardware && numPreFadeSends() > 0) {
			m_audioBufferPreFader->copyAll(m_audioBuffer);
		}

		//
		m_volumeModulator->update();
		int sampleIndex = 0;
		for(int i=0; i<m_processBuffer->numFrames/m_volumeModulator->period(); i++) {
			for(int j=0; j<m_processBuffer->numStreams; j++) {
				float *stream = m_processBuffer->buffer32[j];
				for(int k=0; k<m_volumeModulator->period(); k++) {
					stream[sampleIndex+k] *= m_volumeModulator->buffer()[i];//m_modVolume->modulationBuffer()[i];
					//aLog("sample[%d][%d] * %f", j, sampleIndex+k, m_modVolume->modulationBuffer()[i]);
				}
			}
			sampleIndex += m_volumeModulator->period();
		}
	}
	//cDebug("SamAudioBus::main()");

	/*if(m_testTone) {
		for(int i=0; i<numStreams(); i++) {
			m_oscBuffer->buffer32[i] = m_audioBuffer->stream(i);
		}
		
		m_osc->renderBuffer(m_oscBuffer);
	}*/

	//aLog("EXIT: SamAudioBus::main()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioBus::setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo)
{
	//aLog("SamAudioBus::setAudioProcessInfo() %d", audioProcessInfo.bufferSize);

	if(m_streams < 1) {
		return;
	}

	m_audioProcessInfo = audioProcessInfo; 

	// Allocate the process buffer
	if(m_processBuffer->buffer32) {
		free(m_processBuffer->buffer32);
	}
	m_processBuffer->numFrames = audioProcessInfo.bufferSize;
	m_processBuffer->buffer32 = (float**)malloc(sizeof(float*)*m_streams);

	// Create an audio buffer if this isn't a hardware bus
	if(!m_hardware) {
		if(m_audioBuffer) delete m_audioBuffer;
		if(m_audioBufferPreFader) delete m_audioBufferPreFader;
		
		setAudioBuffer(new CsAudioBuffer(m_streams, m_audioProcessInfo.bufferSize));
		m_audioBufferPreFader = new CsAudioBuffer(m_streams, m_audioProcessInfo.bufferSize);
		/*m_audioBuffer = new CsAudioBuffer(m_streams, m_audioProcessInfo.bufferSize);
		for(int i=0; i<m_streams; i++) {
			m_processBuffer->buffer32[i] = m_audioBuffer->stream(i);
		} */
	}
	
	

	m_volumeModulator->setBufferSize(m_audioProcessInfo.bufferSize);  aLog("5");

	//for(int i=0; i<MAX_SENDS; i++) {
	//	m_audioSend[i]->setAudioProcessInfo(audioProcessInfo);
	//}

	//m_oscBuffer->numFrames = audioProcessInfo.bufferSize;
	//m_osc->setSampleRate(audioProcessInfo.sampleRate);

	//aLog("EXIT: SamAudioBus::setAudioProcessInfo()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioBus::setSpeakerConfig(CsSpeakerConfig config)
{
	// Reassign
	m_speakerConfig = config;
	if(m_speakerConfig.numSpeakers() == m_streams || m_speakerConfig.numSpeakers() < 1) {
		return;
	}

	m_streams = m_speakerConfig.numSpeakers();

	float oldCutOff = m_filter->cutOff();
	if(m_filter) {
		delete m_filter;
	}
	m_filter = new CsSynthFilter(44100.0, m_streams);
	m_filter->setCutOff(oldCutOff);

	m_processBuffer->numStreams = m_streams;

	setAudioProcessInfo(m_audioProcessInfo);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioBus::setAudioBuffer(CsAudioBuffer *audioBuffer)
{
	m_audioBuffer = audioBuffer;
	for(int i=0; i<m_streams; i++) {
		m_processBuffer->buffer32[i] = m_audioBuffer->stream(i);
	} 
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioBus::setAudioOutputBusId(int id, void *from)
{
	m_audioOutputBusId = id;

	updateState(SamEngine::ContextAudioBus, ItemOutputBusId, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioBus::setTestTone(bool testTone, void *from)
{
	m_testTone = testTone;

	//stateManager()->updateState(SamEngine::ContextEngine, ItemTestTone, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamAudioBus::doesBusSendToAudioBusId(int audioBusId)
{
	for(int i=0; i<maxSends; i++) {
		if(m_audioSends[i]) {
			if(m_audioSends[i]->audioBusId() == audioBusId) {
				return true;
			}
		}
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamAudioBus::numPreFadeSends(void)
{
	int num = 0;

	for(int i=0; i<maxSends; i++) {
		if(m_audioSends[i]->isPreFader()) {
			num++;
		}
	}

	return num;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamAudioBus::readElement(AXmlDocument *document, AXmlElement *element)
{
	/*m_name = element->attribute("name");
	m_id = element->attribute("id").toInt();
	
	m_streams = element->attribute("streams").toInt();
	m_speakerConfig = CsSpeakerConfig(element->attribute("speakerConfig"));
	m_audioOutputBusId = element->attribute("audioOutputBusId").toInt();*/

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamAudioBus::writeElement(AXmlDocument *document, AXmlElement *element)
{
	if(SamBus::writeElement(document, element)) {
		element->setAttribute("streams", m_streams);
		element->setAttribute("speakerConfig", CsSpeakerConfig::speakerConfigName(m_speakerConfig.config()));
		element->setAttribute("audioOutputBusId", m_audioOutputBusId);

		for(int i=0; i<numAudioInserts(); i++) {
			if(m_audioToolInserts[i]->isAssigned()) {
				AXmlElement *elInsert = document->createElement("Insert");
				m_audioToolInserts[i]->writeElement(document, elInsert);
				element->addChildElement(elInsert);
			}
		}

		for(int i=0; i<numAudioSends(); i++) {
			AXmlElement *elSend = document->createElement("Send");
			m_audioSends[i]->writeElement(document, elSend);
			element->addChildElement(elSend);
		}
	} else {
		return 0;
	}

	return 1;
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioBus::allocateBuffer(void)
{
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioBus::freeBuffer(void)
{
}