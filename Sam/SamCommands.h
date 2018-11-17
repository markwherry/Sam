#ifndef __SamCommands_h__
#define __SamCommands_h__

#include "ACommand.h"
#include "AList.h"
#include "AString.h"
using namespace Atom;

#include "SamChannel.h"
#include "SamInstrument.h"
#include <QDir>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class NewInstrument : public ACommand
{
public:
	NewInstrument(AList<SamSet*> *setList);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class LoadInstruments : public ACommand
{
public:
	LoadInstruments(AList<AString> *files, int numFiles, bool createChannels);

	void execute(ACommandManager *commandManager = 0, int id = 0);

	SamInstrument* loadInstrument(AString filename);
	SamInstrument* importExsInstrument(AString filename);
	SamInstrument* importSfzInstrument(AString filename);

protected:
private:
	AList<AString> *m_files;
	int m_numFiles;
	bool m_createChannels;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SaveInstrument : public ACommand
{
public:
	SaveInstrument(SamInstrument *instrument, AString filename, bool newUuid);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamInstrument *m_instrument;
	AString m_filename;
	bool m_newUuid;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class ReplaceInstrument : public ACommand
{
public:
	ReplaceInstrument(SamInstrument *instrumentToReplace, AString withFile);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamInstrument *m_instrumentToReplace;
	AString m_withFile;
};	

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class NewProject : public ACommand
{
public:
	NewProject();

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class OpenProject : public ACommand
{
public:
	OpenProject(AString filename);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	AString m_filename;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SaveProject : public ACommand
{
public:
	SaveProject(AString filename);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	AString m_filename;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class BuildDatabase : public ACommand
{
public:
	BuildDatabase();

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	void parseDirectory(QDir dir);
	void countDirectory(QDir dir, int &count);
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SetInstrumentStreamMix : public ACommand
{
public:
	SetInstrumentStreamMix(SamChannel *channel, bool saveInstrument);

	void execute(ACommandManager *commandManager, int id);

protected:
private:
	SamChannel *m_channel;
	bool m_saveInstrument;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SetStreamMixPreset : public ACommand
{
public:
	SetStreamMixPreset(SamChannel *channel, int presetIndex);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamChannel *m_channel;
	int m_presetIndex;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class CopyLayerFromInstrument : public ACommand
{
public:
	CopyLayerFromInstrument(SamInstrument *src, SamInstrument *dst);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamInstrument *m_srcInstrument;
	SamInstrument *m_dstInstrument;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class MergeLoadedInstruments : public ACommand
{
public:
	MergeLoadedInstruments();

	void execute(ACommandManager *commandManager = 0, int id = 0);
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class ExportArticulationAsInstrument : public ACommand
{
public:
	ExportArticulationAsInstrument(SamInstrument *instrument, SamArticulation *articulation, AString filename, bool xmlOnly);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamInstrument   *m_instrument;
	SamArticulation *m_articulation;
	AString          m_filename;
	bool             m_xmlOnly;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class ExportElementPool : public ACommand
{
public:
	ExportElementPool(SamInstrument *instrument, SamArticulation *articulation, AString path);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	int writeElementToFile(SamElement *element, SamCell *cell, AString filename);

	SamInstrument   *m_instrument;
	SamArticulation *m_articulation;
	AString          m_path;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class MoveCopyZones : public ACommand
{
public:
	MoveCopyZones(SamLayer *layer, int semitones, bool transposeRootNote, bool copy);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamLayer *m_layer;
	int m_semitones;
	bool m_transposeRootNote;
	bool m_copy;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class PasteZoneSettings : public ACommand
{
public:
	PasteZoneSettings(SamZone *zone, char *settings, SamLayer *layer);

	void execute(ACommandManager *commandManager = 0, int id = 0);

	enum {
		ZoneSampleStart        =  0,
		LevelsAndOutputs       =  1,
		Pitch                  =  2,
		Velocity               =  3,
		Triggering             =  4,
		Filter                 =  5,
		Envelope1              =  6,
		Envelope2              =  7,
		Envleope3              =  8,
		TempoReleaseModulation =  9,
		Crossfade              = 10
	};

protected:
private:
	SamZone  *m_zone;
	char      m_settings[11];
	SamLayer *m_layer;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SetSetName : public ACommand
{
public:
	SetSetName(SamSet *set, AString name);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamSet *m_set;
	AString m_name;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class AssignSourceToRelease : public ACommand
{
public:
	AssignSourceToRelease(SamLayer * layer);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamLayer *m_layer;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class UnassignRelease : public ACommand
{
public:
	UnassignRelease(SamLayer * layer);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamLayer *m_layer;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class EnableLoops : public ACommand
{
public:
	EnableLoops(SamLayer * layer, bool enabled);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamLayer *m_layer;
	bool m_enabled;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class UpdateLoops : public ACommand
{
public:
	UpdateLoops(SamLayer * layer);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamLayer *m_layer;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class UpdateSetElementStates : public ACommand
{
public:
	UpdateSetElementStates(SamInstrument *instrument, int setId, bool load);
	
	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamInstrument *m_instrument;
	int m_setId;
	bool m_load;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class ImportSampleFolder : public ACommand
{
public:
	ImportSampleFolder(SamLayer *layer, QString path, int numTokens, int tokens[], int numSpeakers);

	void execute(ACommandManager *commandManager = 0, int id = 0);

	enum {
		TkIgnore    = 0,
		TkNoteRoot  = 1,
		TkNoteLo    = 2,
		TkNoteHi    = 3,
		TkVelLo     = 4,
		TkVelHi     = 5,
		TkVariation = 6,
		TkSpeaker   = 7
	};

	struct SimpleCellZoneDefinition {
		AString file;
		int noteRoot;
		int noteLo;
		int noteHi;
		int velLo;
		int velHi;
		int variation;
		int speaker;
	};

protected:
private:
	void buildList(QString in, QList<QString> &tokens);
	void buildDefinition(QList<QString> &listToken, SimpleCellZoneDefinition &def, int *tokenDef, int speakerMode);
	void processDefinition(SamElementPool *ep, SamLayer *layer, SimpleCellZoneDefinition def, int &runningId, bool addRelease = false);

	SamLayer *m_layer;
	QString   m_path;
	int       m_numTokens;
	int       m_tokens[16];
	int       m_numSpeakers;
};

#endif