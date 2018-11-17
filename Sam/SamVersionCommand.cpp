#include "SamVersionCommand.h"

#include "ALog.h"

#include "SamInstrument.h"

SamVersionCommand::SamVersionCommand(SamVersion *version, int propertyId, float value)
: m_version(version)
, m_propertyId(propertyId)
, m_value(value)
//, m_state(false)
{
	aLog("SamVersionCommand::SamVersionCommand(value)");
}

SamVersionCommand::SamVersionCommand(SamVersion *version, int propertyId, AString text)
: m_version(version)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_text(text)
//, m_state(false)
{
	aLog("SamVersionCommand::SamVersionCommand(text)");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVersionCommand::execute(ACommandManager *commandManager, int id)
{
	aLog("SamVersionCommand::execute()");

	ACommand::execute(commandManager, id);

	if(!m_version) {
		aLog("!m_version");
		return;
	}

	switch(m_propertyId) {
		case Name:
			{
				aLog("Name: %s", m_text.toChar());
				m_version->setName(m_text);

				SamInstrument *instrument = (SamInstrument*)m_version->parent();
				instrument->setName(m_text);
			}
			break;
		case DefaultVoices:
			m_version->setNumVoices((int)m_value);
			break;
		case Reference:
			m_version->setReference((int)m_value);
			break;
	}
}