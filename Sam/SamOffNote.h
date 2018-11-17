#ifndef __SamOffNote_h__
#define __SamOffNote_h__

#include "SamMusicTool.h"

class SamOffNote : public SamMusicTool
{
public:
	SamOffNote(SamToolMaster *toolMaster);
	~SamOffNote();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamOffNote(toolMaster); }
	static AString uuid(void) { return "EBB47FB7-156B-448D-9359-F4498BB9691F"; }
	static AString name(void) { return "Off Note"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrRepeats  = 1000,
		pmrVelocity = 1001,
		pmrFade     = 1002
	};

protected:
private:
	int m_repeats;
	int m_velocity;
	int m_fade;
	char m_previousVelocities[128];
	double m_previousTimestamps[128];
};

#endif //__SamOffNote_h__