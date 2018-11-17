#ifndef __SamTranspose_h__
#define __SamTranspose_h__

#include "SamMusicTool.h"

class SamTranspose : public SamMusicTool
{
public:
	SamTranspose(SamToolMaster *toolMaster);
	~SamTranspose();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamTranspose(toolMaster); }
	static AString uuid(void) { return "132FB96F-726F-4a5b-B7ED-A404059B8C13"; }
	static AString name(void) { return "Transpose"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrTranspose = 1000
	};

protected:
private:
	int m_transpose;
};

#endif