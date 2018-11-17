#ifndef __ExsInstrument_h__
#define __ExsInstrument_h__

#include <AFile.h>
#include <AList.h>
#include <ATypes.h>
using namespace Atom;

class ExsZone
{
public:
	ExsZone()
		: oneShot(false)
		, disablePitch(false)
		, reverse(false)
		, useZoneVelocityRange(false)
		, useZoneOutput(false)
		, loopEnabled(false)
		, loopEqualPower(false)
	{
	}

	char name[64];

	// Flags
	bool oneShot;
	bool disablePitch;
	bool reverse;
	bool useZoneVelocityRange;
	bool useZoneOutput;

	// Basic Zone Parameters
	int keyNote;
	int tuneCents;
	int pan;
	int volume;
	int scale;
	int zoneRangeLo;
	int zoneRangeHi;
	int zoneVelocityRangeLo;
	int zoneVelocityRangeHi;

	// Loop Flags
	bool loopEnabled;
	bool loopEqualPower;

	unsigned int startFrame;
	unsigned int endFrame;
	unsigned int loopStart;
	unsigned int loopEnd;
	unsigned int autoCrossfade;

	int tuneSemitones;
	int output;
	unsigned long audioFileId;
	long group;
};

class ExsGroup
{
public:
	ExsGroup()
		: triggerReleaseToggle(false)
	{
		for(int i=0; i<3; i++) {
			selectByMode[i] = 0;
		}
	}

	char name[64];

	// Basic Group Parameters
	int volume;
	int pan;
	int voices;

	bool triggerReleaseToggle;

	int selectRangeLo;
	int selectRangeHi;

	int releaseDecay;

	// Filter
	int filterCutOffOffset;
	int filterResonanceOffset;
	int filterAttackOffset;
	unsigned int filterHoldOffset;
	int filterDecayOffset;
	int filterSustainOffset;
	int filterReleaseOffset;

	// Amp
	int ampAttackOffset;
	unsigned int ampHoldOffset;
	int ampDecayOffset;
	int ampSustainOffset;
	int ampReleaseOffset;

	int triggerOn;
	int output;
	int selectByNotePitch;
	int selectByGroupNum;
	int selectByMode[3];

	int selectByControlNum;
	int selectByControlRangeLo;
	int selectByControlRangeHi;
	int keyRangeLo;
	int keyRangeHi;
	int selectByMidiChannelChannel;
	int selectByBendRangeLo;
	int selectByBendRangeHi;

};

class ExsAudioFile
{
public:
	char filename[64];
};

class ExsParameters
{
public:
	ExsParameters()
		: monoMode(0)
		, transpose(0)
		, coarseTune(0)
		, fineTune(0)
		, env1Attack(0.0f)
		, env1AttackViaVelocity(0.0f)
		, env1Decay(0.0f)
		, env1Sustain(0.0f)
		, env1Release(0.0f)
		, env2Attack(0.0f)
		, env2AttackViaVelocity(0.0f)
		, env2Decay(0.0f)
		, env2Sustain(1.0f)
		, env2Release(0.0f)
	{
	}

	char name[64];

	int outputVolume;
	int keyScale;
	int pitchBendUp;
	int pitchBendDown; // -1 = Linked to pitchBendUp
	int monoMode;
	int voices;

	int transpose;
	int coarseTune;
	int fineTune;
	int glideTime;
	int pitcher;
	int pitcherViaVelocity;

	float env1Attack;
	float env1AttackViaVelocity;
	float env1Decay;
	float env1Sustain;
	float env1Release;

	int timeViaKey;
	int attackCurve;

	float env2Attack;
	float env2AttackViaVelocity;
	float env2Decay;
	float env2Sustain;
	float env2Release;
};

class ExsInstrument : public AFile
{
public:
	ExsInstrument(AString filename);
	~ExsInstrument();

	AStatus read(void);

	char* name(void) { return m_name; }

	int numZones(void) { return zoneList.size(); }
	ExsZone* zone(int index) { return zoneList[index]; }

	int numAudioFiles(void) { return audioFileList.size(); }
	ExsAudioFile* audioFile(int index) { return audioFileList[index]; }

	int numGroups(void) { return groupList.size(); }
	ExsGroup* group(int index) { return groupList[index]; }

protected:
private:
	AStatus readInstrument(long dataSize);
	AStatus readZone(long dataSize);
	AStatus readGroup(long dataSize);
	AStatus readAudioFile(long dataSize, char b1, char b2);
	AStatus readParameters(long dataSize);

	AList<ExsZone*> zoneList;
	AList<ExsGroup*> groupList;
	AList<ExsAudioFile*> audioFileList;
	ExsParameters *m_parameters;

	char m_name[64];

	int m_numZones;
	int m_numGroups;
	int m_numAudioFiles;
	
	float m_envelopeCurve[128];
	float m_percentCurve[128];
	//int m_glideCurve[128];
};

#endif