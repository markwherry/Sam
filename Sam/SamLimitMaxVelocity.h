#ifndef __SamLimitMaxVelocity_h__
#define __SamLimitMaxVelocity_h__

#include "SamMusicTool.h"

class SamLimitMaxVelocity : public SamMusicTool
{
public:
	SamLimitMaxVelocity(SamToolMaster *toolMaster);
	~SamLimitMaxVelocity();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamLimitMaxVelocity(toolMaster); }
	static AString uuid(void) { return "689114F1-82D0-404e-B444-391BEBC0FAD9"; }
	static AString name(void) { return "Limit Max Velocity"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrMaxVelocity    = 1000,
		pmrMidiController = 1001
	};

protected:
private:
	int m_maxVelocity;
	int m_midiController;
};

#endif