#include "SamGuiCsToolGenericEditor.h"

#include <QPainter>

#include "SamEngine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiCsToolGenericEditor::SamGuiCsToolGenericEditor(QWidget *parent, SamTool *tool)
	: QWidget(parent)
	, m_tool(tool)
{
	qDebug("SamGuiCsToolGenericEditor::SamGuiCsToolGenericEditor()");
	//ui.setupUi(this);
	setFixedSize(400,205);

	if(m_tool) {
		int y = 0;
		for(int i=0; i<m_tool->numParameters(); i++) {
			SamGuiCsToolParameter *p = new SamGuiCsToolParameter(this, m_tool, m_tool->parameter(i));
			p->setValueNormalised(m_tool->parameterValueNormalised(m_tool->parameterIdFromIndex(i)));
			connect(p, SIGNAL(parameterChanged(int, float)), this, SLOT(sendParameter(int, float)));
			p->move(0,y);
			p->show();
			y+=p->height();
			m_parameters.push_back(p);
		}
		setFixedHeight(y);
	}

	connect(this, SIGNAL(parameterAssigned()), this, SLOT(updateAssignedParameter()));
	connect(this, SIGNAL(parameterChanged(int)), this, SLOT(updateChangedParameter(int)));

	samEngine->registerListener(this, SamEngine::ContextMusicTool);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiCsToolGenericEditor::~SamGuiCsToolGenericEditor()
{
	samEngine->unregisterListener(this, SamEngine::ContextMusicTool);

	m_tool->setLearningParameter(-1);

	for(int i=0; i<m_parameters.size(); i++) {
		//disconnect(m_parameters[i], SIGNAL(parameterChanged(int, float)), this, SLOT(internalSetParameter(int, float)));
		delete m_parameters[i];
	}
	m_parameters.clear();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolGenericEditor::onState(AStateEvent *ev)
{
	if(ev->context() == SamEngine::ContextMusicTool) {
		switch(ev->item()) {
			case 0:
				emit parameterAssigned();
				break;
			case 1:
				if(ev->updatedObject() == m_tool && ev->fromObject() != this) {
					//qDebug("Updadint!");
					emit parameterChanged((int)ev->value());
				} //else qDebug("We're moving a fader");
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolGenericEditor::updateAssignedParameter(void)
{
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolGenericEditor::updateChangedParameter(int id)
{
	for(int i=0; i<m_parameters.size(); i++) {
		if(m_parameters[i]->parameter()->id() == id) {
			m_parameters[i]->setValueNormalised(m_tool->parameterValueNormalised(id));
			return;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolGenericEditor::updateParameters(void)
{
	for(int i=0; i<m_parameters.size(); i++) {
		m_parameters[i]->setValueNormalised(m_tool->parameterValueNormalised(m_tool->parameterIdFromIndex(i)));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolGenericEditor::sendParameter(int id, float value)
{
	//qDebug("SamGuiCsToolGenericEditor::sendParameter(%d, %f)", id, value);

	m_tool->toolMaster()->parameterChanged(id, value, this);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCsToolGenericEditor::paintEvent(QPaintEvent *ev)
{
	//qDebug("SamGuiCsToolGenericEditor::paintEvent()");

	/*QPainter p(this);

	p.setPen(QColor(255,0,0));
	p.setBrush(QColor(0,0,0,0));
	p.drawRect(0,0,width()-1,height()-1);//*/
}