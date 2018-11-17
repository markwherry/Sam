#include "SamToolInsert.h"

#include "SamEngine.h"

SamToolInsert::SamToolInsert(AModel *parent, int type, int index)
	: AModel(parent)
	, m_type(type)
	, m_index(index)
	, m_active(false)
	, m_tool(0)
	, m_activeLearn(false)
	, m_activeController(-1)
{
}

SamToolInsert::~SamToolInsert()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamToolInsert::processEvent(SamEvent *ev)
{
	//aLog("SamMusicToolInsert::processEvent");

	if(ev->type == SamEvent::Controller) {

		if(m_activeLearn) {
			m_activeController = ev->midiValue1;
			m_activeLearn = false;
		}

		if(m_tool->learningParameter() > -1) {

			SamToolParameter *parameter = m_tool->parameterFromId(m_tool->learningParameter());
			if(parameter) {
				//parameter->setMidiController(ev->midiValue1);
				//m_musicTool->setLearningParameter(-1);
				m_tool->setParameterMidiController(m_tool->learningParameter(), ev->midiValue1);
				aLog("Parameter assigned %d", parameter->midiController());
			}

		} else {

			if(ev->midiValue1 == m_activeController) {
				//aLog("ev->midiValue2 = %d", ev->midiValue2);
				if(ev->midiValue2 == 0) {
					//m_bypassed = true;
					setActive(true);
				} else {
					//m_bypassed = false;
					setActive(false);
				}
			}

			for(int i=0; i<m_tool->numParameters(); i++) {
				//aLog("%d/%d %d == %d", i+1, m_musicTool->numParameters(), m_musicTool->parameter(i)->midiController(), ev->midiValue1 );
				if(m_tool->parameter(i)->midiController() == ev->midiValue1) {

					float range = m_tool->parameter(i)->midiMax() - m_tool->parameter(i)->midiMin();
					float value = m_tool->parameter(i)->midiMin() + ((range / 127.0f)* (float)ev->midiValue2);

					m_tool->setParameterValueNormalised(m_tool->parameter(i)->id(), value); // 0.00787401575f * (float)ev->midiValue2
					if(m_type == 0) {
						updateState(SamEngine::ContextAudioTool, 1, m_tool, 0, m_tool->parameter(i)->id());
					} else if(m_type == 1) {
						updateState(SamEngine::ContextMusicTool, 1, m_tool, 0, m_tool->parameter(i)->id());
					}
				}
			}

			
		}

	} else if(ev->type == SamEvent::Parameter) {
		m_tool->setParameterValueNormalised(ev->iValue, ev->fValue);
	}

	if(m_active) {
		m_tool->processEvent(ev);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamToolInsert::isAssigned(void)
{
	if(m_tool) {
		return true;
	}

	return false;
}