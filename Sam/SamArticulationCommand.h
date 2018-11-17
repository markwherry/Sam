#ifndef __SamArticulationCommand_h__
#define __SamArticulationCommand_h__

#include "ACommand.h"
#include "AList.h"
#include "AString.h"
using namespace Atom;

#include "SamArticulation.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SamArticulationCommand : public ACommand
{
public:
	SamArticulationCommand(SamArticulation *articulation, int propertyId, float value);
	SamArticulationCommand(SamArticulation *articulation, int propertyId, AString text);
	SamArticulationCommand(SamArticulation *articulation, int propertyId, bool state);

	void execute(ACommandManager *commandManager = 0, int id = 0);

	enum SamZonePropertyId {

		Name                      =  0,
		TriggerId                 =  1,
		ZoneCrossfadeResponseTime =  2,
		ZoneFilterResponseTime    =  3,
		PolyphonicModulation      =  4,
		MaxAftertouch             =  5,
		//MinimumVelocityController =  5,

		CurveVelocityShape  = 10,
		CurveVelocityDepth  = 11,
		CurveVelocityStart  = 12,
		CurveVelocityEnd    = 13,

		CurveCrossfadeShape = 20,
		CurveCrossfadeDepth = 21,
		CurveCrossfadeStart = 22,
		CurveCrossfadeEnd   = 23,

		CurveReleaseShape   = 30,
		CurveReleaseDepth   = 31,
		CurveReleaseStart   = 32,
		CurveReleaseEnd     = 33,

		Output              = 40,
		OutputRelease       = 41,

		LayerDefault        = 50,
		LayerSelectMode     = 51,
		LayerSwitchTime     = 52,
		LayerLegatoOverlap  = 53,

		VelocitySetsCrossfade      = 60,
		VelocityCrossfadeDecay     = 61,
		VelocityCrossfadeDecayTime = 62,
		VelocityCrossfadeDecayTo   = 63

	};

protected:
private:
	SamArticulation *m_articulation;
	int m_propertyId;
	float m_value;
	AString m_text;
	bool m_state;
};

#endif