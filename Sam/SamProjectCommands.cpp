#include "SamProjectCommands.h"

#include "aLog.h"

#include "SamEngine.h"
#include "SamRealTimeCommands.h"
#include "Sam.h"

#include <strsafe.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CreateChannel::CreateChannel(SamInstrument *instrument)
: m_instrument(instrument)
{
	setFeedback(false);
}

void CreateChannel::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	aLog("CreateChannel::execute()");

	int nextMidiChannel = 1;
	int midiPort = 0;
	// If there aren't any Channels already, let's set the MIDI Input to the first available Input Bus.
	if(samEngine->numMidiInputBuses() > 0) {
		midiPort = samEngine->midiInputBus(0)->id();
	}
	SamChannel *lastSelectedChannel = 0;
	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		aLog("channel %d/%d", i, samEngine->project()->numChannels());
		if(samEngine->project()->channel(i)->isSelected()) {
			aLog("selected");
			lastSelectedChannel = samEngine->project()->channel(i);
		}
	}

	if(lastSelectedChannel) {
		//cDebug("lastSelectedChannel");
		//track->setMidiInputBusId(lastSelectedChannel->midiInputBusId());
		midiPort = lastSelectedChannel->midiInputPort();
		nextMidiChannel = lastSelectedChannel->midiInputChannel()+1;
		if(nextMidiChannel>16) {
			nextMidiChannel = 1;
		}
	}

	SamChannel *channel = new SamChannel(samEngine->project());
	channel->setAudioProcessInfo(samEngine->audioProcessInfo());
	channel->setMidiInputChannel(nextMidiChannel, 0, false);
	channel->setMidiInputPort(midiPort, 0, false);
	channel->assignInstrument(m_instrument, 0);

	if(samEngine->project()->numAudioHardwareBuses() > 0) {
		for(int i=0; i<channel->numAudioOutputs(); i++) {
			int id = samEngine->project()->audioHardwareBus(0)->id();
			//for(int i=0; i<channel->numAudioOutputs(); i++) { // What the fuck was this for?
				channel->audioOutputBus(i)->setAudioOutputBusId(id);

			//}
			if(lastSelectedChannel && channel->numAudioOutputs() == lastSelectedChannel->numAudioOutputs()) {
				if(i < lastSelectedChannel->numAudioOutputs()) {
					for(int j=0; j<channel->audioOutputBus(i)->numAudioSends(); j++) {
						channel->audioOutputBus(i)->audioSend(j)->setAudioBusId(lastSelectedChannel->audioOutputBus(i)->audioSend(j)->audioBusId());
						channel->audioOutputBus(i)->audioSend(j)->setActive(lastSelectedChannel->audioOutputBus(i)->audioSend(j)->isActive());
						channel->audioOutputBus(i)->audioSend(j)->setGain(lastSelectedChannel->audioOutputBus(i)->audioSend(j)->gain());
						channel->audioOutputBus(i)->audioSend(j)->setPreFader(lastSelectedChannel->audioOutputBus(i)->audioSend(j)->isPreFader());
					}
				}
			}
		}



	}
	

	samEngine->executeRealTimeCommand(new RtcAddChannel(channel));

	if(samEngine->project()->numAudioHardwareBuses() < 1) {
		AString name = AString("Output ") + AString::number(samEngine->project()->numAudioHardwareBuses()+1) + AString(" (") + channel->streamMix()->speakerConfig().name() + AString(")");
		samEngine->project()->commandManager()->execute(new CreateAudioHardwareBus(-1, name, channel->streamMix()->speakerConfig(), -1));
	}

	samEngine->executeRealTimeCommand(new RtcAutoAssignFirstChannelOutput());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
DeleteChannel::DeleteChannel(SamChannel *channel)
: m_channel(channel)
{
}

void DeleteChannel::execute(ACommandManager *commandManager, int id)
{
	aLog("DeleteChannel::execute()");

	//SamInstrument *instrument = m_channel->instrument();

	int numChannels = samEngine->project()->numChannels();

	samEngine->executeRealTimeCommand(new RtcRemoveChannel(m_channel));

	/*while(numChannels == samEngine->project()->numChannels()) {
		aLog("Spin");
		Sleep(1);
	}

	aLog("Released...");*/

	//delete m_channel;

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CreateAudioHardwareBus::CreateAudioHardwareBus(int id, AString name, CsSpeakerConfig config, int hardwareStartStream)
: m_id(id)
, m_name(name)
, m_config(config)
, m_hardwareStartStream(hardwareStartStream)
{
	aLog("CreateAudioHardwareBus::CreateAudioHardwareBus()");
	setFeedback(false);
}

void CreateAudioHardwareBus::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	aLog("CreateAudioHardwareBus::execute()");
	if(m_id < 0) {
		m_id = samEngine->project()->nextAudioHardwareBusId();
	}

	SamAudioHardwareBus *audioHardwareBus = new SamAudioHardwareBus(samEngine->project(), m_id, m_name, m_config.numSpeakers(), m_config);

	if(m_hardwareStartStream<0) {
		m_hardwareStartStream = samEngine->project()->nextAudioHardwareBusStartStream();
	}
	
	for(int i=0; i<m_config.numSpeakers(); i++){
		aLog("[%d] = [%d]",i, m_hardwareStartStream);
		audioHardwareBus->setStreamHardwareChannel(i, m_hardwareStartStream);
		m_hardwareStartStream++;
	}

	audioHardwareBus->setAudioProcessInfo(samEngine->audioProcessInfo());
	audioHardwareBus->setActive();
	
	samEngine->executeRealTimeCommand(new RtcAddAudioHardwareBus(audioHardwareBus));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CreateAudioBus::CreateAudioBus(int id, AString name, CsSpeakerConfig config)
: m_id(id)
, m_name(name)
, m_config(config)
{
	aLog("CreateAudioBus::CreateAudioBus()");
	setFeedback(false);
}

void CreateAudioBus::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);

	aLog("CreateAudioBus::execute()");
	if(m_id < 0) {
		m_id = samEngine->project()->nextAudioBusId();
	}

	SamAudioBus *audioBus = new SamAudioBus(samEngine->project(), m_id, m_name, m_config.numSpeakers(), m_config);

	audioBus->setAudioProcessInfo(samEngine->audioProcessInfo());
	audioBus->setActive();
	
	samEngine->executeRealTimeCommand(new RtcAddAudioBus(audioBus));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SetAudioInsert::SetAudioInsert(SamAudioBus *audioBus, int mode, int insertIndex, AString toolUuid)
	: m_audioBus(audioBus)
	, m_mode(mode)
	, m_insertIndex(insertIndex)
	, m_toolUuid(toolUuid)
{
	memset(m_filename, 0, sizeof(TCHAR)*256);
}

SetAudioInsert::SetAudioInsert(SamAudioBus *audioBus, int mode, int insertIndex, TCHAR *filename)
	: m_audioBus(audioBus)
	, m_mode(mode)
	, m_insertIndex(insertIndex)
{
	StringCchCopy(m_filename, 256, filename);
}

void SetAudioInsert::execute(ACommandManager *commandManager, int id)
{
	samEngine->executeRealTimeCommand(new RtcSetAudioToolActive(m_audioBus, m_insertIndex, false));

	while(m_audioBus->audioInsert(m_insertIndex)->isActive()) {
		Sleep(1);
	}

	samGui->toolEditor()->setToolInsert(0, 0);

	if(m_toolUuid.length() > 0) {
		m_audioBus->audioInsert(m_insertIndex)->setAudioTool(m_toolUuid);
		samEngine->executeRealTimeCommand(new RtcSetAudioToolActive(m_audioBus, m_insertIndex, true));
	} else if(wcslen(m_filename) > 0) {
		m_audioBus->audioInsert(m_insertIndex)->setVstPlugin(m_filename, m_mode);
		samEngine->executeRealTimeCommand(new RtcSetAudioToolActive(m_audioBus, m_insertIndex, true));
	} else {
		m_audioBus->audioInsert(m_insertIndex)->setAudioTool("");		
		return;
	}

	for(int i=0; i<2000; i++) {
		if(m_audioBus->audioInsert(m_insertIndex)->isAssigned()) {
			samGui->toolEditor()->setToolInsert(m_audioBus->audioInsert(m_insertIndex), m_audioBus);
			samGui->toolEditor()->show();
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------