#ifndef __SamVpModulator_h__
#define __SamVpModulator_h__

#include "SamMusicTool.h"

#define SAM_VP_MODULATOR_UUID "84704BAF-5DBF-499F-852A-2701B886BE78"
#define SAM_VP_MODULATOR_NAME "VP Modulator"

class SamVpModulator : public SamMusicTool
{
public:
	SamVpModulator(SamToolMaster *toolMaster);
	~SamVpModulator();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamVpModulator(toolMaster); }
	static AString uuid(void) { return SAM_VP_MODULATOR_UUID; }
	static AString name(void) { return SAM_VP_MODULATOR_NAME; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrPc         = 1000,
		pmrThreshold  = 1001,
		pmrScale      = 1002
	};

protected:
private:
	int m_pc;
	int m_threshold;
	int m_scale;
};

#endif // __SamVpModulator_h__