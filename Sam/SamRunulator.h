#ifndef __SamRunulator_h__
#define __SamRunulator_h__

#include "SamMusicTool.h"

class SamRunulator : public SamMusicTool
{
public:
	SamRunulator(SamToolMaster *toolMaster);
	~SamRunulator();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamRunulator(toolMaster); }
	static AString uuid(void) { return "CEE03836-07C2-4E20-9FFD-42852B01D8C6"; }
	static AString name(void) { return "Runulator"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrSampleStart  = 1000,
		pmrAttackOffset = 1001
	};

protected:
private:
	bool isNoteDown(void);

	int m_sampleStart;
	int m_attackOffset;
	char m_noteDown[128];
};

#endif //__SamRunulator_h__