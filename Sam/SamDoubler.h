#ifndef __SamDoubler_h__
#define __SamDoubler_h__

#include "SamMusicTool.h"

#define SAM_DOUBLER_UUID "426BD946-BA19-41fc-844F-2B7078FE4627"

class SamDoubler : public SamMusicTool
{
public:
	SamDoubler(SamToolMaster *toolMaster);
	~SamDoubler();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamDoubler(toolMaster); }
	static AString uuid(void) { return SAM_DOUBLER_UUID; }
	static AString name(void) { return "Doubler"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrPitch     = 1000,
		pmrVelocity  = 1001,
		pmrDelay     = 1002
	};

protected:
private:
	int m_pitch;
	int m_velocity;
	int m_delay;
};

#endif // __SamDoubler_h__