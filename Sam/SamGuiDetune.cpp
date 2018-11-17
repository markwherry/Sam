#include "SamGuiDetune.h"

#include "SamDetune.h"

SamGuiDetune::SamGuiDetune(QWidget *parent, SamTool *tool)
	: QWidget(parent)
	, m_tool(tool)
{
	//ui.setupUi(this);

	setFixedSize(1300, 200);

	m_steps = new SamGuiSteps(this);
	m_steps->move(10,10);
	m_steps->setFixedSize(1280,104);
	m_steps->setNumSteps(128);
	m_steps->setMidZero(true);

	connect(m_steps, SIGNAL(valueUpdated(int, float)), this, SLOT(updateValue(int, float)));
}

SamGuiDetune::~SamGuiDetune()
{

}

void SamGuiDetune::updateParameter(int id, float value)
{
	//qDebug("SamGuiStepRepeater::updateParameter(%d, %f)", id, value);
	//qDebug("m_numSteps->parameter() = %d", m_numSteps->parameter());

	switch(id) {
		//case SamStepRepeater::pmrNumSteps:
		//	m_steps->setNumSteps((int)m_numSteps->parameter()->valueFromNormalised(value));
		//	m_tool->toolMaster()->parameterChanged(SamStepRepeater::pmrNumSteps, value);
		//	break;
	}
}

void SamGuiDetune::updateValue(int step, float value)
{
	//qDebug("SamGuiStepRepeater::updateValue()");
	m_tool->toolMaster()->parameterChanged(SamDetune::pmrDetune+step, value, this);
}