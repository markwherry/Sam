#ifndef __SamTool_h__
#define __SamTool_h__

#include "AString.h"
#include "AList.h"
#include "AXmlDocument.h"
using namespace Atom;

//#include "NeObject.h"
//using namespace Neon;

#include "SamToolMaster.h"
#include "SamToolParameter.h"

#include <QWidget>

class SamTool
{
public:
	SamTool(SamToolMaster *toolMaster, AString name, AString uuid);

	virtual int init(void) { return 1; }
	virtual int exit(void) { return 1; }

	virtual void start(void) { }
	virtual void stop(void) { }

	virtual void processEvent(SamEvent *ev) = 0;
	
	int numParameters(void) { return parameterList.size(); }
	SamToolParameter* parameter(int listId) { return parameterList[listId]; }
	SamToolParameter* parameterFromId(int id);
	SamToolParameter* parameterFromMidiController(int controller);
	int parameterIndexFromId(int id);
	int parameterIdFromIndex(int index);
	virtual float parameterValueNormalised(int id);
	virtual void setParameterValueNormalised(int id, float value);
	virtual AString parameterValueDisplay(int id) { return ""; }

	int learningParameter(void) { return m_learningParameter; }
	void setLearningParameter(int parameterId) { m_learningParameter = parameterId; }

	void setParameterMidiController(int parameterId, int midiController);

	//enum Parameters {
	//	pmrBypassCC = 2000, // Not to be used. Was a bad idea!
	//	pmrBypass   = 2001
	//};

	//bool isBypassed(void) { return m_bypassed; }


	//NeObject *editor(void) { return m_editor; }
	//void setEditor(NeObject *editor) { m_editor = editor; }

	virtual QWidget* createEditor(void) { return 0; }

	//static const CsString name = "Tool";
	//static const CsString guid = "";

	SamToolMaster* toolMaster(void) { return m_toolMaster; }
	void setToolMaster(SamToolMaster *toolMaster) { m_toolMaster = toolMaster; }

	AString name(void) { return m_name; }
	AString uuid(void) { return m_uuid; }

	int readState(AXmlDocument *document, AXmlElement *element);
	int writeState(AXmlDocument *document, AXmlElement *element);

protected:
	void addParameter(SamToolParameter *parameter);

private:
	SamToolMaster *m_toolMaster;
	AString m_name;
	AString m_uuid;
	AList<SamToolParameter*> parameterList;
//	QWidget *m_editor;
	bool m_bypassed;
	int m_learningParameter;
};

#endif