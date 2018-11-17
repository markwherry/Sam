#ifndef __SamCrossfadeStringsSetup_h__
#define __SamCrossfadeStringsSetup_h__

#include "SamMusicTool.h"

#define SAM_CROSSFADESTRINGSSETUP_UUID "5D2DED64-DEB2-4986-8E9B-93E1D7507B29"

class SamCrossfadeStringsSetup : public SamMusicTool
{
public:
	SamCrossfadeStringsSetup(SamToolMaster *toolMaster);
	~SamCrossfadeStringsSetup();

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamCrossfadeStringsSetup(toolMaster); }
	static AString uuid(void) { return SAM_CROSSFADESTRINGSSETUP_UUID; }
	static AString name(void) { return "Crossfade Strings Setup"; }

	virtual void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	enum Parameters {
		pmrPitch          = 1000,
		pmrMidiController = 1001,
		pmrTracking       = 1002
	};

protected:
private:
	
};

#endif // __SamCrossfadeStringsSetup_h__