#ifndef __SamRealTimeCommands_h__
#define __SamRealTimeCommands_h__

#include "CsRealTimeCommand.h"
using namespace Cs;

#include "SamChannel.h"
#include "SamAudioHardwareBus.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcAddChannel : public CsRealTimeCommand
{
public:
	RtcAddChannel(SamChannel *channel);
	void execute(double timestamp, double samples);

protected:
private:
	SamChannel *m_channel;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcRemoveChannel : public CsRealTimeCommand
{
public:
	RtcRemoveChannel(SamChannel *channel);
	void execute(double timestamp, double samples);

protected:
private:
	SamChannel *m_channel;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcAssignInstrument : public CsRealTimeCommand
{
public:
	RtcAssignInstrument(SamChannel *channel, SamInstrument *instrument);
	void execute(double timestamp, double samples);

protected:
private:
	SamChannel *m_channel;
	SamInstrument *m_instrument;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcAddAudioHardwareBus : public CsRealTimeCommand
{
public:
	RtcAddAudioHardwareBus(SamAudioHardwareBus *audioHardwareBus);
	void execute(double timestamp, double samples);

protected:
private:
	SamAudioHardwareBus *m_audioHardwareBus;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcRemoveAudioHardwareBus : public CsRealTimeCommand
{
public:
	RtcRemoveAudioHardwareBus(SamAudioHardwareBus *audioHardwareBus);
	void execute(double timestamp, double samples);

protected:
private:
	SamAudioHardwareBus *m_audioHardwareBus;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcAddAudioBus : public CsRealTimeCommand
{
public:
	RtcAddAudioBus(SamAudioBus *audioBus);
	void execute(double timestamp, double samples);

protected:
private:
	SamAudioBus *m_audioBus;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcRemoveAudioBus : public CsRealTimeCommand
{
public:
	RtcRemoveAudioBus(SamAudioBus *audioHardwareBus);
	void execute(double timestamp, double samples);

protected:
private:
	SamAudioBus *m_audioBus;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcSetStreamInHardwareBus : public CsRealTimeCommand
{
public:
	RtcSetStreamInHardwareBus(SamAudioHardwareBus *audioHardwareBus, int stream, int channel);
	void execute(double timestamp, double samples);

protected:
private:
	SamAudioHardwareBus *m_audioHardwareBus;
	int m_stream;
	int m_channel;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------

class RtcSetAudioBusOutputId : public CsRealTimeCommand
{
public:
	RtcSetAudioBusOutputId(SamAudioBus *audioBus, int outputId, void *from);
	void execute(double timestamp, double samples);

protected:
private:
	SamAudioBus *m_audioBus;
	int m_outputId;
	void *m_from;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------

class RtcAutoAssignFirstChannelOutput : public CsRealTimeCommand
{
public:
	RtcAutoAssignFirstChannelOutput();
	void execute(double timestamp, double samples);
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------

class RtcSetSendOutput : public CsRealTimeCommand
{
public:
	RtcSetSendOutput(SamAudioBus *audioBus, int sendId, SamAudioBus *sendToAudioBus);
	void execute(double timestamp, double samples);
protected:
private:
	SamAudioBus *m_audioBus;
	int m_sendId;
	SamAudioBus *m_sendToAudioBus;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcSetSendActive : public CsRealTimeCommand
{
public:
	RtcSetSendActive(SamAudioSend *send, bool active);
	void execute(double timestamp, double samples);
protected:
private:
	SamAudioSend *m_send;
	bool m_active;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcSetSendPreFader: public CsRealTimeCommand
{
public:
	RtcSetSendPreFader(SamAudioSend *send, bool preFader);
	void execute(double timestamp, double samples);

protected:
private:
	SamAudioSend *m_send;
	bool          m_preFader;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcToggleEngineTestTone : public CsRealTimeCommand
{
public:
	RtcToggleEngineTestTone(void);
	void execute(double timestamp, double samples);

protected:
private:
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcSetMusicTool : public CsRealTimeCommand
{
public:
	RtcSetMusicTool(SamChannel* channel, int insertIndex, bool instrument, SamMusicTool* musicTool);
	void execute(double timestamp, double samples);

protected:
private:
	SamChannel* m_channel;
	int m_insertIndex;
	bool m_instrument;
	SamMusicTool* m_musicTool;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcSetMusicToolActive : public CsRealTimeCommand
{
public:
	RtcSetMusicToolActive(SamChannel* channel, int insertIndex, bool instrument, bool active);
	void execute(double timestamp, double samples);

protected:
private:
	SamChannel* m_channel;
	int m_insertIndex;
	bool m_instrument;
	bool m_active;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcSetChannelConfig : public CsRealTimeCommand
{
public:
	RtcSetChannelConfig(SamChannel *channel, CsSpeakerConfig config);
	void execute(double timestamp, double samples);

protected:
private:
	SamChannel *m_channel;
	CsSpeakerConfig m_config;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcAddParameter : public CsRealTimeCommand
{
public:
	RtcAddParameter(SamArticulation *articulation, SamParameter *parameter);
	void execute(double timestamp, double samples);

protected:
private:
	SamArticulation *m_articulation;
	SamParameter *m_parameter;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcRemoveParameter : public CsRealTimeCommand
{
public:
	RtcRemoveParameter(SamArticulation *articulation, int parameterIndex);
	void execute(double timestamp, double samples);

protected:
private:
	SamArticulation *m_articulation;
	int m_parameterIndex;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcTransport : public CsRealTimeCommand
{
public:
	enum Command {
		Stop = 0,
		Play = 1,
		Record = 2,
		ReturnToZero = 3,
		Sync = 4,
		Click = 5,
		Locate = 6
	};
	RtcTransport(Command command);

	void execute(double timestamp, double samples);

protected:
private:
	Command m_command;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*class RtcSetPerVoiceFxTool : public CsRealTimeCommand
{
public:
	RtcSetPerVoiceFxTool(SamChannel* channel, AString toolUuid);
	void execute(double timestamp, double samples);

protected:
private:
	SamChannel* m_channel;
	AString m_toolUuid;
};*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*class RtcSetPerVoiceFxToolActive : public CsRealTimeCommand
{
public:
	RtcSetPerVoiceFxToolActive(SamChannel* channel, int insertIndex, bool active);
	void execute(double timestamp, double samples);

protected:
private:
	SamChannel* m_channel;
	int m_insertIndex;
	bool m_perVoiceFx;
	bool m_active;
};*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*class RtcSetAudioTool : public CsRealTimeCommand
{
public:
	RtcSetAudioTool(SamAudioBus *audioBus, int mode, int insertIndex, AString toolUuid);
	void execute(double timestamp, double samples);

protected:
private:
	SamAudioBus *m_audioBus;
	int m_mode;
	int m_insertIndex;
};*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class RtcSetAudioToolActive : public CsRealTimeCommand
{
public:
	RtcSetAudioToolActive(SamAudioBus *audioBus, int insertIndex, bool active);
	void execute(double timestamp, double samples);

protected:
private:
	SamAudioBus *m_audioBus;
	int m_insertIndex;
	bool m_active;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*class RtcSetVstPlugin : public CsRealTimeCommand
{
public:
	RtcSetVstPlugin(SamAudioBus *audioBus, int mode, int insertIndex, TCHAR *filename);
	void execute(double timestamp, double samples);

protected:
private:
	SamAudioBus *m_audioBus;
	int m_insertIndex;
};*/


#endif // __SamRealTimeCommands_h__