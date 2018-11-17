#ifndef __SamDetune_h__
#define __SamDetune_h__

#include "SamMusicTool.h"

class SamDetune : public SamMusicTool
{
public:
	SamDetune(SamToolMaster *toolMaster);
	~SamDetune();

	QWidget* createEditor(void);

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamDetune(toolMaster); }
	static AString uuid(void) { return "2FED9152-E5EA-40bb-9E91-723BAD09F215"; }
	static AString name(void) { return "Detune"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrDetune = 1000 // to 1127
	};

protected:
private:
	int m_detune[128];
};

#endif