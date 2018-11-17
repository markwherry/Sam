#ifndef __SamZoneCommand_h__
#define __SamZoneCommand_h__

#include "ACommand.h"
#include "AList.h"
#include "AString.h"
using namespace Atom;

#include "SamZone.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SamZoneCommand : public ACommand
{
public:
	SamZoneCommand(SamZone *zone, int propertyId, float value);
	SamZoneCommand(SamZone *zone, int propertyId, AString text);
	SamZoneCommand(SamZone *zone, int propertyId, bool state);

	void execute(ACommandManager *commandManager = 0, int id = 0);

	enum SamZonePropertyId {

		Name                         =   0,
		Description                  =   1,
		Group                        =   2,
		DynamicRef                   =   3,
		SampleStart                  =   4,

		OutputGain                   = 100,
		OutputReleaseGain            = 101,
		OutputPreLoopReleaseGain     = 102,
		OutputEnable                 = 103,
		OutputOutput                 = 104,
		OutputReleaseOutput          = 105,

		PitchLo                      = 200,
		PitchHi                      = 201,
		PitchTrack                   = 203,
		PitchFineTune                = 204,
		PitchMove                    = 205,

		VelocityLo                   = 300,
		VelocityHi                   = 301,
		VelocityPredictable          = 302,
		VelocitySampleStart          = 303,
		VelocitySampleStartThreshold = 304,
		VelocityMove                 = 305,
		VelocityPredictableCellIndex = 306,

		TriggeringOneShot         = 400,
		TriggeringCellTriggerMode = 401,
		TriggeringSingleCellId    = 402,

		FilterEnabled             = 500,
		FilterCutOff              = 501,
		//FilterCutOffController    = 502,
		FilterMinCutOff           = 503,
		//FilterMaxCutOff           = 504,
		FilterResonance           = 505,
		FilterVelocityScaling     = 506,
		FilterPitchTracking       = 507,
		FilterPitchTrackOffset    = 508,
		FilterEnvelope            = 509,
		FilterVelocityFullRange   = 510,
		FilterVelocityDepth       = 511,
		FilterParameter           = 512,

		Env1Attack                = 600,
		Env1AttackVelocity        = 601,
		Env1AttackVelocityThreshold = 602,
		Env1AttackOffset          = 603,
		Env1AttackOffsetMode      = 604,
		Env1AttackCurve           = 605,
		Env1AttackCurveShape      = 606,
		Env1AttackCurveDepth      = 607,
		Env1Hold                  = 608,
		Env1Decay                 = 609,
		Env1Sustain               = 610,
		Env1Release               = 611,
		Env1AutoRelease           = 612,
		Env1ReleaseCurveShape     = 613,
		Env1ReleaseCurveDepth     = 614,

		Env2Attack                = 700,
		Env2AttackCurveShape      = 701,
		Env2AttackCurveDepth      = 702,
		Env2Hold                  = 703,
		Env2Decay                 = 704,
		Env2Sustain               = 705,
		Env2Release               = 706,
		Env2ReleaseCurveShape     = 707,
		Env2ReleaseCurveDepth     = 708,

		Env3Attack                = 800,
		Env3AttackVelocity        = 801,
		Env3AttackVelocityThreshold = 802,
		Env3AttackCurveShape      = 803,
		Env3AttackCurveDepth      = 804,
		Env3Hold                  = 805,
		Env3Decay                 = 806,
		Env3Sustain               = 807,
		Env3Release               = 808,
		Env3AutoRelease           = 809,
		Env3ReleaseCurveShape     = 810,
		Env3ReleaseCurveDepth     = 811,

		TempoAdjustsRelease       = 900,
		TempoThreshold            = 901,
		TempoReleaseDivisor       = 902,

		CrossfadeInStart          = 1000,
		CrossfadeInEnd            = 1001,
		CrossfadeOutStart         = 1002,
		CrossfadeOutEnd           = 1003,
		CrossfadeInCurveDepth     = 1004,
		CrossfadeOutCurveDepth    = 1005


	};

protected:
private:
	SamZone *m_zone;
	int m_propertyId;
	float m_value;
	AString m_text;
	bool m_state;
};

#endif