#include "SamAudioHardwareBus.h"

#include "SamEngine.h"
#include "SamProject.h"

SamAudioHardwareBus::SamAudioHardwareBus(AModel *parent, int id, AString name, int streams, CsSpeakerConfig config)
: SamAudioBus(parent, id, name, streams, config)
{
	aLog("SamAudioHardwareBus::SamAudioHardwareBus(streams = %d)", streams);

	setHardware(true);
	setTestTone(false);

	for(int i=0; i<16; i++) {
		m_streamHardwareChannel[i] = 0;
	}
}

SamAudioHardwareBus::~SamAudioHardwareBus()
{
	
}

/*void SamAudioHardwareBus::setAudioProcessInfo(Core::AudioProcessInfo audioProcessInfo)
{
	//cDebug("SamAudioHardwareBus::setAudioProcessInfo() %d", audioProcessInfo.bufferSize);
	SamAudioBus::setAudioProcessInfo(audioProcessInfo);
}*/

void SamAudioHardwareBus::main(void)
{
	//cDebug("SamAudioHardwareBus::main()");

	if(isTestTone()) {
		for(int i=0; i<numStreams(); i++) {
			m_oscBuffer->buffer32[i] = m_audioBuffer->stream(m_streamHardwareChannel[i]);
		}
		
		m_osc->renderBuffer(m_oscBuffer);
		return;
	}

	//Iterate number of channels in project
	for(int i=0; i<project()->numChannels(); i++){

		SamChannel *channel = (SamChannel*)project()->channel(i);

		// Iterate number of outputs (audio buses) in a channel
		for(int out=0; out<channel->numAudioOutputs(); out++) {

			SamAudioBus *output = channel->audioOutputBus(out);

			// Is the current bus of the current channel routed to this audio output bus?
			if(output->audioOutputBusId() == m_id){

			//aLog("m_project->channel(%d)->audioOutputBusId(%d) == %d", i, out, m_id);
					
			// Does the output configuration of the channel's audio output bus match the speaker config of this bus?
				//if(output->speakerConfig().config() == speakerConfig().config()) {
				//aLog("output->numStreams() = %d, numStreams() = %d", output->numStreams(), numStreams());
				//if(output->numStreams() <= numStreams()) {

					//aLog("output->speakerConfig().config() %d == speakerConfig().config() %d", output->speakerConfig().numS, speakerConfig().name().toChar());
						
					if(!project()->channel(i)->isMute()) {// && ((!m_project->isSoloSet()) || (samEngine->project()->isSoloSet()&&samEngine->project()->channel(i)->isSolo()))) {
						//aLog("speakerConfig().numSpeakers() = %d", speakerConfig().numSpeakers());
						for(int j=0; j<speakerConfig().numSpeakers(); j++){
							//CSDEBUG("[%d, %d] %d, %d (out=%d) m_streamHardwareChannel[%d]=%d", m_id, m_speakerConfig, j, i, out, j, m_streamHardwareChannel[j]);
							//if(samEngine->project()->channel(i)->audioBuffer())

							//m_audioBuffer->addStream(m_streamHardwareChannel[j], m_project->channel(i)->outputBuffer(out),j); // Original Line before AudioBuses were added to Channels
							if(j < output->numStreams()) {
								if(streamHardwareChannel(j) < m_audioBuffer->numStreams()) {
									m_audioBuffer->addStream(streamHardwareChannel(j), channel->audioOutputBus(out)->audioBuffer(), j);
								} /*else {
									aLog("CRASH!");
								}*/
							}
							//csDebug("Not here");
						}
					}
				//} else {
					//cDebug("Output config does not match.");
				//}

					
			} //else {
				//aLog("Checking sends... %d (%d)", id(), i);
				for(int send=0; send<SamAudioBus::maxSends; send++) {
					SamAudioSend *s = project()->channel(i)->audioOutputBus(out)->audioSend(send);
					//aLog("  %d, %d = %d", s->isActive(), s->audioBusId(), m_id);
					if(s->isActive() && s->audioBusId() == m_id) { //&& s->gain() > 0.000001f
						
						//aLog("%d, %d = %d    Here gain = %f", s->isActive(), s->audioBusId(), m_id, s->gain());
						//m_audioBuffer->addAll(m_project->channel(i)->audioOutputBus(out)->audioSend(send)->audioBuffer());
						for(int j=0; j<speakerConfig().numSpeakers(); j++) {
							//m_audioBuffer->addStream(m_streamHardwareChannel(j), project()->channel(i)->audioOutputBus(out)->audioSend(send)->audioBuffer(), j);
							if(j < output->numStreams()) { //aLog("%d, %d = %d    a", s->isActive(), s->audioBusId(), m_id);
								if(streamHardwareChannel(j) < m_audioBuffer->numStreams()) { //aLog("%d, %d = %d    b", s->isActive(), s->audioBusId(), m_id);
									if(s->isPreFader()) { //aLog("%d, %d = %d    c pre", s->isActive(), s->audioBusId(), m_id);
										m_audioBuffer->addStreamWithGain(streamHardwareChannel(j), channel->audioOutputBus(out)->audioBufferPreFader(), j, s->gain());
									} else { //aLog("%d, %d = %d    c", s->isActive(), s->audioBusId(), m_id);
										m_audioBuffer->addStreamWithGain(streamHardwareChannel(j), channel->audioOutputBus(out)->audioBuffer(), j, s->gain());
									}
								} /*else {
									aLog("CRASH!");
								}*/
							}
						}
					}
				}//*/

			//}
		}
	}
}

int SamAudioHardwareBus::readElement(AXmlDocument *document, AXmlElement *element)
{
	for(int i=0; i<element->numChildElements(); i++) {
		AXmlElement *elStream = element->childElement(i);
		if(elStream->tag() == "Stream") { // We need to make sure we only look at Streams, otherwise the Sends can fuck things up.
			int index = elStream->attribute("id").toInt();
			if(index > -1 && index < 16) {
				m_streamHardwareChannel[index] = elStream->attribute("hardwareChannel").toInt();
			}
		}
	}

	return SamAudioBus::readElement(document, element);
}

int SamAudioHardwareBus::writeElement(AXmlDocument *document, AXmlElement *element)
{
	for(int i=0; i<numStreams(); i++) {
		AXmlElement *elStream = document->createElement("Stream");
		elStream->setAttribute("id", i);
		elStream->setAttribute("hardwareChannel", m_streamHardwareChannel[i]);
		element->addChildElement(elStream);
	}

	return SamAudioBus::writeElement(document, element);
}