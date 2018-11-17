#ifndef __SamMusicTool_h__
#define __SamMusicTool_h__

#include "SamTool.h"
#include "SamEvent.h"

#include "CsTypes.h"
using namespace Cs;

class SamMusicTool : public SamTool
{
public:
	SamMusicTool(SamToolMaster *toolMaster, AString name, AString uuid);
	~SamMusicTool();

	//virtual void processEvent(SamEvent *ev) = 0;
	virtual void processRange(CsPpqnTime from, CsPpqnTime to) { }

protected:
private:
};

#endif