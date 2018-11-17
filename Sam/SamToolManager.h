#ifndef __SamAudioToolManager_h__
#define __SamAudioToolManager_h__

#include "SamTool.h"

#include "AString.h"
#include "AList.h"
using namespace Atom;

struct SamToolInfo
{
	SamTool* (*createFunction)(SamToolMaster*);
	AString uuid;
	AString name;
};

class SamToolManager
{
public:
	SamToolManager();
	void registerTool(SamTool* (*createFunction)(SamToolMaster*), AString uuid, AString name);
	SamTool* createInstance(AString uuid, SamToolMaster *toolMaster);
	

	int numTools(void) { return m_toolInfos.size(); }
	SamToolInfo toolInfo(AString uuid);
	SamToolInfo toolInfo(int index);

protected:
private:
	AList<SamToolInfo> m_toolInfos;
};

#endif