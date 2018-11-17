#include "SamGuiStepRepeater.h"

#include "SamStepRepeater.h"

SamGuiStepRepeater::SamGuiStepRepeater(QWidget *parent, SamTool *tool)
	: QWidget(parent)
	, m_tool(tool)
{
	//ui.setupUi(this);

	m_steps = new SamGuiSteps(this);
	m_steps->move(10,10);
	m_steps->setFixedSize(480,3+96+3+16+3+16+3+1+3+16+3+16);

	connect(m_steps, SIGNAL(valueUpdated(int, float)), this, SLOT(updateValue(int, float)));
	connect(m_steps, SIGNAL(muteUpdated(int, bool)), this, SLOT(updateMute(int, bool)));
	connect(m_steps, SIGNAL(accentUpdated(int, bool)), this, SLOT(updateAccent(int, bool)));

	m_steps->setNumSteps((int)(m_tool->parameterFromId(SamStepRepeater::pmrNumSteps)->valueFromNormalised(m_tool->parameterValueNormalised(SamStepRepeater::pmrNumSteps))));//        )));//parameterValueNormalised(SamStepRepeater::pmrNumSteps) * 64.0));
	for(int i=0; i<64; i++) {
		m_steps->setValue(i, (int)(m_tool->parameterValueNormalised(SamStepRepeater::pmrStep+i) * 127.0));
		m_steps->setMute(i, (bool)((int)m_tool->parameterValueNormalised(SamStepRepeater::pmrMute+i)));
		m_steps->setAccent(i, (bool)((int)m_tool->parameterValueNormalised(SamStepRepeater::pmrAccent+i)));
	}

	m_numSteps = new SamGuiCsToolParameter(this, m_tool, m_tool->parameterFromId(SamStepRepeater::pmrNumSteps));
	m_numSteps->setValueNormalised(m_tool->parameterValueNormalised(SamStepRepeater::pmrNumSteps));
	m_numSteps->move(10, 10+m_steps->height()+10);
	connect(m_numSteps, SIGNAL(parameterChanged(int, float)), this, SLOT(updateParameter(int, float)));

	

	/*int prY = 50+m_steps->height()+10;
	prNumSteps = new SamGuiAudioToolEditorParameter(this, 100, "Steps", "", 1, 64, 1);
	prNumSteps->move(0, prY);
	connect(prNumSteps, SIGNAL(parameterChanged(int, float)), this, SLOT(setParameter(int, float)));*/
}

SamGuiStepRepeater::~SamGuiStepRepeater()
{

}

void SamGuiStepRepeater::updateParameter(int id, float value)
{
	//qDebug("SamGuiStepRepeater::updateParameter(%d, %f)", id, value);
	//qDebug("m_numSteps->parameter() = %d", m_numSteps->parameter());

	switch(id) {
		case SamStepRepeater::pmrNumSteps:
			m_steps->setNumSteps((int)m_numSteps->parameter()->valueFromNormalised(value));
			m_tool->toolMaster()->parameterChanged(SamStepRepeater::pmrNumSteps, value, this);
			break;
	}
}

void SamGuiStepRepeater::updateValue(int step, float value)
{
	//qDebug("SamGuiStepRepeater::updateValue()");
	m_tool->toolMaster()->parameterChanged(SamStepRepeater::pmrStep+step, (1.0f/127.0f)*value, this);
}

void SamGuiStepRepeater::updateMute(int step, bool mute)
{
	m_tool->toolMaster()->parameterChanged(SamStepRepeater::pmrMute+step, (int)mute, this);
}

void SamGuiStepRepeater::updateAccent(int step, bool accent)
{
	m_tool->toolMaster()->parameterChanged(SamStepRepeater::pmrAccent+step, (int)accent, this);
}