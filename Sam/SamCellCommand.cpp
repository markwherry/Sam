#include "SamCellCommand.h"

#include "ALog.h"

SamCellCommand::SamCellCommand(SamCell *cell, int propertyId, float value)
: m_cell(cell)
, m_propertyId(propertyId)
, m_value(value)
, m_state(false)
{
	aLog("SamCellCommand::SamCellCommand(value)");
}

/*SamCellCommand::SamCellCommand(SamCell *cell, int propertyId, AString text)
: m_cell(cell)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_text(text)
, m_state(false)
{
	aLog("SamZoneCommand::SamZoneCommand(text)");
}*/

SamCellCommand::SamCellCommand(SamCell *cell, int propertyId, bool state)
: m_cell(cell)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_state(state)
{
	aLog("SamCellCommand::SamCellCommand(state)");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamCellCommand::execute(ACommandManager *commandManager, int id)
{
	aLog("SamCellCommand::execute()");

	ACommand::execute(commandManager, id);

	if(!m_cell) {
		aLog("!m_cell");
		return;
	}

	switch(m_propertyId) {
		case BaseNote:
			m_cell->setNoteBase((int)m_value);
			break;
		case FineTune:
			m_cell->setFineTune((int)m_value);
			break;
		case SourceStart:
			m_cell->setSourceStart((int)m_value);
			break;
		case ReleaseStart:
			m_cell->setReleaseStart((int)m_value);
			break;
		//case ReleaseAttenuation:
		//	m_cell->setRel
		//	break;

		case LoopEnable:
			m_cell->setLoop(m_state);
			break;
		case LoopStart:
			m_cell->setLoopStart((unsigned int)m_value);
			break;
		case LoopEnd:
			m_cell->setLoopEnd((unsigned int)m_value);
			break;
	}
}