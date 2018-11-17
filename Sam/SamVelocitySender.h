#ifndef __SamVelocitySender_h__
#define __SamVelocitySender_h__

#include "SamMusicTool.h"

#define SAM_VELOCITYSENDER_UUID "661BA0D2-4A7C-4f0c-AF79-6995ABF0F4F5"

class SamVelocitySender : public SamMusicTool
{
public:
	SamVelocitySender(SamToolMaster *toolMaster);
	~SamVelocitySender();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamVelocitySender(toolMaster); }
	static AString uuid(void) { return SAM_VELOCITYSENDER_UUID; }
	static AString name(void) { return "Velocity Sender"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrSend      = 1000,
		pmrThreshold = 1001,
		pmrScale     = 1002,
		pmrInvert    = 1003,
		pmrMax       = 1004,
		pmrMin       = 1005,
	};

protected:
private:
	int m_send;
	int m_threshold;
	int m_scale;
	int m_invert;
	float m_min;
	float m_max;
};

#endif // __SamVelocitySender_h__