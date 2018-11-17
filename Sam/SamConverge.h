#ifndef __SamConverge_h__
#define __SamConverge_h__

#include "SamMusicTool.h"

#define SAM_CONVERGE_UUID "88DDA77C-A7C3-41ff-9E65-8A70E9A09E76"

class SamConverge : public SamMusicTool
{
public:
	SamConverge(SamToolMaster *toolMaster);
	~SamConverge();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamConverge(toolMaster); }
	static AString uuid(void) { return SAM_CONVERGE_UUID; }
	static AString name(void) { return "Converge"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrPitch           = 1000,
		pmrMidiController  = 1001,
		pmrTracking        = 1002,
		pmrPitchController = 1003,
		pmrConvergence     = 1004
	};

protected:
private:
	int m_pitch;
	int m_midiController;
	float m_value;
	int m_tracking;
	int m_pitchController;
};

#endif // __SamConverge_h__