#ifndef __SamProjectCommands_h__
#define __SamProjectCommands_h__

#include "ACommand.h"
#include "AString.h"
using namespace Atom;

#include "CsSpeakerConfig.h"
using namespace Cs;

#include "SamProject.h"
#include "SamInstrument.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class CreateChannel : public ACommand
{
public:
	CreateChannel(SamInstrument *instrument);
	void execute(ACommandManager *commandManager = 0, int id = 0);
protected:
private:
	SamInstrument *m_instrument;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class DeleteChannel : public ACommand
{
public:
	DeleteChannel(SamChannel *channel);
	void execute(ACommandManager *commandManager = 0, int id = 0);
protected:
private:
	SamChannel *m_channel;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class CreateAudioHardwareBus : public ACommand
{
public:
	CreateAudioHardwareBus(int id, AString name, CsSpeakerConfig config, int hardwareStartStream);
	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	int m_id;
	AString m_name;
	CsSpeakerConfig m_config;
	int m_hardwareStartStream;

};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class CreateAudioBus : public ACommand
{
public:
	CreateAudioBus(int id, AString name, CsSpeakerConfig config);
	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	int m_id;
	AString m_name;
	CsSpeakerConfig m_config;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SetAudioInsert : public ACommand
{
public:
	SetAudioInsert(SamAudioBus *audioBus, int mode, int insertIndex, AString toolUuid);
	SetAudioInsert(SamAudioBus *audioBus, int mode, int insertIndex, TCHAR *filename);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamAudioBus *m_audioBus;
	int m_mode;
	int m_insertIndex;
	AString m_toolUuid;
	TCHAR m_filename[256];
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

#endif