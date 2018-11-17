#ifndef __SamCentipede_h__
#define __SamCentipede_h__

#include "SamMusicTool.h"

class SamCentipede : public SamMusicTool
{
public:
	SamCentipede(SamToolMaster *toolMaster);
	~SamCentipede();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamCentipede(toolMaster); }
	static AString uuid(void) { return "43910E8E-A310-4ccb-91FB-5FD852B7F020"; }
	static AString name(void) { return "Centipede"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrRangeHigh      = 1000,
		pmrRangeLow       = 1001,
		pmrVelocity       = 1002,
		pmrVelocityInvert = 1003
	};

protected:
private:
	int m_rangeHigh;
	int m_rangeLow;
	float m_velocity;
	int m_velocityInvert;
};

#endif