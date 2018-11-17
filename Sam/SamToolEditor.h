#ifndef __CsToolEditor_h__
#define __CsToolEditor_h__

#include "SamTool.h"

class SamToolEditor
{
public:
	SamToolEditor(SamTool *tool)
		: m_tool(tool)
	{
	}

	SamTool *tool(void) { return m_tool; }

protected:
private:
	SamTool *m_tool;
};

#endif