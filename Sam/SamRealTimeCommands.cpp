#include "SamRealTimeCommands.h"

#include "SamEngine.h"
#include "SamProject.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcAddChannel::RtcAddChannel(SamChannel *channel)
: m_channel(channel)
{
}

void RtcAddChannel::execute(double timestamp, double samples)
{
	samEngine->project()->addChannel(m_channel);
	samEngine->project()->calculateAudioBusDependencies();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcRemoveChannel::RtcRemoveChannel(SamChannel *channel)
: m_channel(channel)
{
}

void RtcRemoveChannel::execute(double timestamp, double samples)
{
	SamInstrument *instrument = m_channel->instrument();

	samEngine->project()->removeChannel(m_channel);
	samEngine->project()->calculateAudioBusDependencies();

	bool removeInstrument = true;
	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		if(samEngine->project()->channel(i)->instrument() == instrument) {
			removeInstrument = false;
			break;
		}
	}

	if(removeInstrument) {
		samEngine->removeInstrument(instrument);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
RtcAssignInstrument::RtcAssignInstrument(SamChannel *channel, SamInstrument *instrument)
: m_channel(channel)
, m_instrument(instrument)
{
}

void RtcAssignInstrument::execute(double timestamp, double samples)
{
	m_channel->assignInstrument(m_instrument, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcAddAudioHardwareBus::RtcAddAudioHardwareBus(SamAudioHardwareBus *audioHardwareBus)
: m_audioHardwareBus(audioHardwareBus)
{
}

void RtcAddAudioHardwareBus::execute(double timestamp, double samples)
{
	aLog("----- RtcAddAudioHardwareBus::execute() -----");

	bool exists = false;

	for(int i=0; i<samEngine->project()->numAudioHardwareBuses(); i++) {
		if(samEngine->project()->audioHardwareBus(i)->id() == m_audioHardwareBus->id()) {
			exists = true;
			break;
		}
	}

	if(!exists) {
		samEngine->project()->addAudioHardwareBus(m_audioHardwareBus);
		samEngine->project()->calculateAudioBusDependencies();	
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcRemoveAudioHardwareBus::RtcRemoveAudioHardwareBus(SamAudioHardwareBus *audioHardwareBus)
: m_audioHardwareBus(audioHardwareBus)
{
}

void RtcRemoveAudioHardwareBus::execute(double timestamp, double samples)
{
	aLog("----- RtcRemoveAudioHardwareBus::execute() -----");

	samEngine->project()->removeAudioHardwareBus(m_audioHardwareBus);
	samEngine->project()->calculateAudioBusDependencies();	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcAddAudioBus::RtcAddAudioBus(SamAudioBus *audioBus)
	: m_audioBus(audioBus)
{
}

void RtcAddAudioBus::execute(double timestamp, double samples)
{
	aLog("----- RtcAddAudioBus::execute() -----");

	bool exists = false;

	for(int i=0; i<samEngine->project()->numAudioBuses(); i++) {
		if(samEngine->project()->audioBus(i)->id() == m_audioBus->id()) {
			exists = true;
			break;
		}
	}

	if(!exists) {
		samEngine->project()->addAudioBus(m_audioBus);
		samEngine->project()->calculateAudioBusDependencies();	
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcRemoveAudioBus::RtcRemoveAudioBus(SamAudioBus *audioBus)
	: m_audioBus(audioBus)
{
}

void RtcRemoveAudioBus::execute(double timestamp, double samples)
{
	aLog("----- RtcRemoveAudioHardwareBus::execute() -----");

	samEngine->project()->removeAudioBus(m_audioBus);
	samEngine->project()->calculateAudioBusDependencies();	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
RtcSetStreamInHardwareBus::RtcSetStreamInHardwareBus(SamAudioHardwareBus *audioHardwareBus, int stream, int channel)
	: m_audioHardwareBus(audioHardwareBus)
	, m_stream(stream)
	, m_channel(channel)
{
}

void RtcSetStreamInHardwareBus::execute(double timestamp, double samples)
{
	m_audioHardwareBus->setStreamHardwareChannel(m_stream, m_channel);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcSetAudioBusOutputId::RtcSetAudioBusOutputId(SamAudioBus *audioBus, int outputId, void *from)
: m_audioBus(audioBus)
, m_outputId(outputId)
{
}

void RtcSetAudioBusOutputId::execute(double timestamp, double samples)
{
	aLog("RtcSetAudioBusOutputId::execute() %d", m_outputId);
	m_audioBus->setAudioOutputBusId(m_outputId, m_from);
	samEngine->project()->calculateAudioBusDependencies();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcAutoAssignFirstChannelOutput::RtcAutoAssignFirstChannelOutput()
{
}

void RtcAutoAssignFirstChannelOutput::execute(double timestamp, double samples)
{
	if(samEngine->project()->numChannels() == 1) {
		if(samEngine->project()->numAudioHardwareBuses() > 0) {
			int id = samEngine->project()->audioHardwareBus(0)->id();
			for(int i=0; i<samEngine->project()->channel(0)->numAudioOutputs(); i++) {
				samEngine->project()->channel(0)->audioOutputBus(i)->setAudioOutputBusId(id);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcSetSendOutput::RtcSetSendOutput(SamAudioBus *audioBus, int sendId, SamAudioBus *sendToAudioBus)
: m_audioBus(audioBus)
, m_sendId(sendId)
, m_sendToAudioBus(sendToAudioBus)
{
}

void RtcSetSendOutput::execute(double timestamp, double samples)
{
	aLog("RtcSetSendOutput::execute() m_audioBus = %d", m_audioBus);

	if(m_sendToAudioBus) {
		aLog("m_sendToAudioBus m_sendId = %d", m_sendId);
		aLog("m_sendToAudioBus->id() = %d", m_sendToAudioBus->id());
		m_audioBus->audioSend(m_sendId)->setAudioBusId(m_sendToAudioBus->id()); aLog("A");
		m_audioBus->audioSend(m_sendId)->setActive(true); aLog("B");
	} else {
		m_audioBus->audioSend(m_sendId)->setAudioBusId(0);
		m_audioBus->audioSend(m_sendId)->setActive(false);
	}
	
	samEngine->project()->calculateAudioBusDependencies();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcSetSendActive::RtcSetSendActive(SamAudioSend *send, bool active)
: m_send(send)
, m_active(active)
{
}

void RtcSetSendActive::execute(double timestamp, double samples)
{
	m_send->setActive(m_active);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcSetSendPreFader::RtcSetSendPreFader(SamAudioSend *send, bool preFader)
: m_send(send)
, m_preFader(preFader)
{
}

void RtcSetSendPreFader::execute(double timestamp, double samples)
{
	m_send->setPreFader(m_preFader);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcToggleEngineTestTone::RtcToggleEngineTestTone()
{
}

void RtcToggleEngineTestTone::execute(double timestamp, double samples)
{
	aLog("RtcToggleEngineTestTone::execute()");
	samEngine->setTestTone(!samEngine->isTestTone());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------

RtcSetMusicTool::RtcSetMusicTool(SamChannel* channel, int insertIndex, bool instrument, SamMusicTool* musicTool)
: m_channel(channel)
, m_insertIndex(insertIndex)
, m_instrument(instrument)
, m_musicTool(musicTool)
{
}

void RtcSetMusicTool::execute(double timestamp, double samples)
{
	aLog("RtcSetMusicTool::execute()");

	if(m_channel && m_insertIndex >= 0 && m_insertIndex < SamChannel::maxMusicTools) {
		m_channel->musicToolInsert(m_insertIndex, m_instrument)->setTool(m_musicTool);
	} else aLog("FAIL! %d", m_insertIndex);
}

RtcSetMusicToolActive::RtcSetMusicToolActive(SamChannel* channel, int insertIndex, bool instrument, bool active)
: m_channel(channel)
, m_insertIndex(insertIndex)
, m_instrument(instrument)
, m_active(active)
{
}

void RtcSetMusicToolActive::execute(double timestamp, double samples)
{
	if(m_channel && m_insertIndex >= 0 && m_insertIndex < SamChannel::maxMusicTools) {
		m_channel->musicToolInsert(m_insertIndex, m_instrument)->setActive(m_active);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
RtcSetChannelConfig::RtcSetChannelConfig(SamChannel *channel, CsSpeakerConfig config)
: m_channel(channel)
, m_config(config)
{
}

void RtcSetChannelConfig::execute(double timestamp, double samples)
{
	m_channel->setSpeakerConfig(m_config);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
RtcAddParameter::RtcAddParameter(SamArticulation *articulation, SamParameter *parameter)
: m_articulation(articulation)
, m_parameter(parameter)
{
}

void RtcAddParameter::execute(double timestamp, double samples)
{
	m_articulation->addParameter(m_parameter);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
RtcRemoveParameter::RtcRemoveParameter(SamArticulation *articulation, int parameterIndex)
: m_articulation(articulation)
, m_parameterIndex(parameterIndex)
{
}

void RtcRemoveParameter::execute(double timestamp, double samples)
{
	m_articulation->removeParameter(m_parameterIndex);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
RtcTransport::RtcTransport(Command command)
: m_command(command)
{
}

void RtcTransport::execute(double timestamp, double samples)
{
	switch(m_command) {
		case Stop:
			samEngine->stop(timestamp, samples);
			break;
		case Play:
			samEngine->play(timestamp, samples);
			break;
		/*case Record:
			engine->record(timestamp, samples);
			break;
		case ReturnToZero:
			if(csEngine->isPlaying()) {
				csEngine->stop(timestamp, samples);
				csEngine->stop(timestamp, samples);
				csEngine->play(timestamp, samples);
			} else {
				csEngine->stop(timestamp, samples);
			}
		case kSync:
			break;
		case kClick:
			csEngine->toggleClick();
			break;
		case kLocate:
			csEngine->locate(timestamp, samples, m_value);
			break;*/
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*RtcSetPerVoiceFxTool::RtcSetPerVoiceFxTool(SamChannel* channel, AString toolUuid)
	: m_channel(channel)
	, m_toolUuid(toolUuid)
{
	//aLog("RtcSetPerVoiceFxTool::RtcSetPerVoiceFxTool(%s)", toolUuid.toChar());
}

void RtcSetPerVoiceFxTool::execute(double timestamp, double samples)
{
	//aLog("RtcSetPerVoiceFxTool::execute()");

	if(m_channel) {
		m_channel->setPerVoiceFxTool(m_toolUuid);
		for(int i=0; i<m_channel->numVoices(); i++) {
			if(m_channel->voice(i)->audioToolInsert()) { //aLog("%d", i);
				m_channel->voice(i)->audioToolInsert()->setActive(true);
			}
		}
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*RtcSetPerVoiceFxToolActive::RtcSetPerVoiceFxToolActive(SamChannel* channel, int insertIndex, bool active)
	: m_channel(channel)
	, m_insertIndex(insertIndex)
	, m_active(active)
{
	aLog("RtcSetAudioToolActive::RtcSetAudioToolActive(%d, %d)", insertIndex, (int)active);
}
	
void RtcSetPerVoiceFxToolActive::execute(double timestamp, double samples)
{
	aLog("RtcSetAudioToolActive::execute()");

	if(!m_channel) return;

	for(int i=0; i<m_channel->numVoices(); i++) {
		if(m_channel->voice(i)->audioToolInsert()) { //aLog("%d", i);
			m_channel->voice(i)->audioToolInsert()->setActive(m_active);
		}
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*RtcSetAudioTool::RtcSetAudioTool(SamAudioBus *audioBus, int mode, int insertIndex, AString toolUuid)
	: m_audioBus(audioBus)
	, m_mode(mode)
	, m_insertIndex(insertIndex)
{
	m_audioBus->audioInsert(m_insertIndex)->setAudioTool(toolUuid, audioBus->speakerConfig());
}

void RtcSetAudioTool::execute(double timestamp, double samples)
{
	m_audioBus->audioInsert(m_insertIndex)->setActive(true);
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
RtcSetAudioToolActive::RtcSetAudioToolActive(SamAudioBus *audioBus, int insertIndex, bool active)
	: m_audioBus(audioBus)
	, m_insertIndex(insertIndex)
	, m_active(active)
{
	aLog("RtcSetAudioToolActive::RtcSetAudioToolActive()");
}

void RtcSetAudioToolActive::execute(double timestamp, double samples)
{
	aLog("RtcSetAudioToolActive::execute()");

	m_audioBus->audioInsert(m_insertIndex)->setActive(m_active);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*RtcSetVstPlugin::RtcSetVstPlugin(SamAudioBus *audioBus, int mode, int insertIndex, TCHAR *filename)
	: m_audioBus(audioBus)
	, m_insertIndex(insertIndex)
{
	m_audioBus->audioInsert(m_insertIndex)->setVstPlugin(filename, audioBus->speakerConfig(), mode);
}

void RtcSetVstPlugin::execute(double timestamp, double samples)
{
	m_audioBus->audioInsert(m_insertIndex)->setActive(true);
}*/