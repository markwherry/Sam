#ifndef __SamLayer_h__
#define __SamLayer_h__

#include "AModel.h"
#include "AString.h"
#include "AList.h"
#include "AXmlDocument.h"
using namespace Atom;

#include "SamZone.h"
//#include "SamVoiceGroup.h"

class SamLayer : public AModel
{
public:
    SamLayer(AModel *parent);
	SamLayer(AModel *parent, const SamLayer& layer, int elementIdOffset = 0, bool parametersOnly = false);
    ~SamLayer();

	void addZone(SamZone *zone, bool shouldUpdate = false);
	int numZones(void) { return zoneList.size(); }
	int numSelectedZones(void);
	SamZone* zone(int id) { return zoneList[id]; }
	SamZone* getZone(int pitch, int velocity);
	//SamZone* getZoneByCrossfadeRange(int pitch, int crossfadeValue);
	int getZonesByPitch(SamZone **zones, int pitch);
	int numZonesByPitch(int pitch);
	void removeZone(SamZone *zone, bool shouldUpdate = false);

	SamZone* existingZone(int noteLo, int noteHi, int velLo, int velHi);

	AList<SamZone*>* zones(void) { return &zoneList; }

	//int numVoiceGroups(void) { return groupList.size(); }
	//SamVoiceGroup* voiceGroup(int id) { return groupList[id]; }
	//void createVoiceGroup(int id, Core::String name);

	// Accessors
	inline AString name (void) const { return m_name; }
	void setName (AString name, void *from = 0, bool shouldUpdate = true);

	inline int zoneSelectMode(void) { return m_zoneSelectMode; }
	void setZoneSelectMode(int zoneSelectMode) { m_zoneSelectMode = zoneSelectMode; }

	inline bool isShortNoteMode(void) { return m_shortNoteMode; }
	void setShortNoteMode(bool shortNoteMode) { m_shortNoteMode = shortNoteMode; }

	inline int groupPlaybackMode(void) { return m_groupPlaybackMode; }
	void setGroupPlaybackMode(int groupPlaybackMode) { m_groupPlaybackMode = groupPlaybackMode; }

	inline int groupPlaybackGroup(void) { return m_groupPlaybackGroup; }
	void setGroupPlaybackGroup(int groupPlaybackGroup) { m_groupPlaybackGroup = groupPlaybackGroup; }

	inline int pitchBendUp(void) { return m_pitchBendUp; }
	void setPitchBendUp(int pitchBendUp) { m_pitchBendUp = pitchBendUp; }

	inline int pitchBendDown(void) { return m_pitchBendDown; }
	void setPitchBendDown(int pitchBendDown) { m_pitchBendDown = pitchBendDown; }

	inline int aliasNote(int note) { return m_aliasNote[note]; }
	void setAliasNote(int note, int alias) { m_aliasNote[note] = alias; }

	inline float fineTune(void) const { return m_fineTune; }
	inline void setFineTune(float fineTune) { m_fineTune = fineTune; }

	inline int downbeatSampleOffset(void) const { return m_downbeatSampleOffset; }
	inline void setDownbeatSampleOffset(int offset) { m_downbeatSampleOffset = offset; }

	inline int downbeatEnvelopeOffset(void) const { return m_downbeatEnvelopeOffset; }
	inline void setDownbeatEnvelopeOffset(int offset) { m_downbeatEnvelopeOffset = offset; }

	inline int midiVelocityLow(void) const { return m_midiVelocityLow; }
	void setMidiVelocityLow(int velocity, void *from = 0, bool shouldUpdate = true);

	inline int midiVelocityHigh(void) const { return m_midiVelocityHigh; }
	void setMidiVelocityHigh(int velocity, void *from = 0, bool shouldUpdate = true);

	inline int midiPitchLow(void) const { return m_midiPitchLow; }
	void setMidiPitchLow(int pitch, void *from = 0, bool shouldUpdate = true);

	inline int midiPitchHigh(void) const { return m_midiPitchHigh; }
	void setMidiPitchHigh(int pitch, void *from = 0, bool shouldUpdate = true);

	inline int intervalLow(void) const { return m_intervalLow; }
	void setIntervalLow(int interval) { m_intervalLow = interval; }

	inline int intervalHigh(void) const { return m_intervalHigh; }
	void setIntervalHigh(int interval) { m_intervalHigh = interval; }

	inline int intervalRecorded(void) const { return m_intervalRecorded; }
	void setIntervalRecorded(int interval) { m_intervalRecorded = interval; }

	inline int timeFromLastNoteMin(void) const { return m_timeFromLastNoteMin; }
	void setTimeFromLastNoteMin(int time) { m_timeFromLastNoteMin = time; }

	inline int timeFromLastNoteMax(void) const { return m_timeFromLastNoteMax; }
	void setTimeFromLastNoteMax(int time) { m_timeFromLastNoteMax = time; }

	inline bool isTimeFromLastNoteMaxInf(void) const { return m_timeFromLastNoteMaxInf; }
	void setTimeFromLastNoteMaxInf(bool infinity) { m_timeFromLastNoteMaxInf = infinity; }

	inline int maximumCells(void) const { return m_maximumCells; }

	inline int releaseVoices(void) const { return m_releaseVoices; }
	void setReleaseVoices(int voices) { m_releaseVoices = voices; }

	inline float gain(void) const { return m_gain; }
	inline void setGain(float gain);

	inline float gainLinear(void) const { return m_gainLinear; } 

	inline int pedalMode(void) const { return m_pedalMode; }
	void setPedalMode(int pedalMode) { m_pedalMode = pedalMode; }

	inline bool sliceMode(void) const { return m_sliceMode; }
	void setSliceMode(bool sliceMode) { m_sliceMode = sliceMode; }

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

	enum LayerItems {
		ItemName  = 0,
		ItemScope = 1,
		ItemZones = 2
	};

private:
	void calculateMaximumCells(void);
	AString m_name;
	int m_zoneSelectMode;
	bool m_shortNoteMode;
	int m_groupPlaybackMode;
	int m_groupPlaybackGroup;
	int m_pitchBendUp;
	int m_pitchBendDown;
	int m_aliasNote[128];
	int m_fineTune;
	int m_downbeatSampleOffset;
	int m_downbeatEnvelopeOffset;
	int m_midiVelocityLow;
	int m_midiVelocityHigh;
	int m_intervalLow;
	int m_intervalHigh;
	int m_intervalRecorded;
	int m_midiPitchLow;
	int m_midiPitchHigh;
	int m_timeFromLastNoteMin;
	int m_timeFromLastNoteMax;
	bool m_timeFromLastNoteMaxInf;
	int m_maximumCells;
	int m_releaseVoices;
	int m_pedalMode;
	bool m_sliceMode;

	AList<SamZone*> zoneList;
	//AList<SamVoiceGroup*> groupList;

	float m_gain;
	float m_gainLinear;
	
};

/*enum kSamGroupPlaybackModes {
	kSamGroupPlaybackAll    = 0,
	kSamGroupPlaybackSingle = 1,
	kSamGroupPlaybackAlt    = 2
};*/

#endif // __SamLayer_h__
