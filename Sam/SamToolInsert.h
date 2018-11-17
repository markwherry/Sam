#ifndef __SamToolInsert_h__
#define __SamToolInsert_h__

#include "AModel.h"
#include "AXmlDocument.h"
using namespace Atom;

#include "SamTool.h"

class SamToolInsert : public AModel
{
public:
	SamToolInsert(AModel *parent, int type, int index);
	virtual ~SamToolInsert();

	virtual void setTool(SamTool *tool, void* from = 0) { m_tool = tool; }
	//SamTool* tool(void) { return m_tool; }

	int type(void) { return m_type; }
	int index(void) { return m_index; }

	virtual AString name(void) { return m_tool->name(); }

	void processEvent(SamEvent *ev);

	virtual bool isAssigned(void);

	bool isActive(void) { return m_active; }
	virtual void setActive(bool active, void* from = 0) = 0;

	//bool isBypassed(void) { return m_bypassed; }
	//void setBypassed(bool bypassed) { m_bypassed = bypassed; }

	virtual bool isActiveLearn(void) { return m_activeLearn; }
	virtual void setActiveLearn(bool learn) { m_activeLearn = learn; }

	virtual int activeController(void) { return m_activeController; }
	virtual void setActiveController(int controller) { m_activeController = controller; }

	virtual int writeElement(AXmlDocument *document, AXmlElement *element) = 0;

protected:
	bool m_active;
	//bool m_bypassed;
	int m_index;

private:
	SamTool *m_tool;
	int m_type;
	

	bool m_activeLearn;
	int m_activeController;
};

#endif //__SamToolInsert_h__