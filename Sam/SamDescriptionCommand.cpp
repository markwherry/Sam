#include "SamDescriptionCommand.h"

#include "ALog.h"

SamDescriptionCommand::SamDescriptionCommand(SamDescription *description, int propertyId, AString text)
: m_description(description)
, m_propertyId(propertyId)
, m_text(text)
//, m_state(false)
{
	aLog("SamDescriptionCommand::SamDescriptionCommand(text)");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamDescriptionCommand::execute(ACommandManager *commandManager, int id)
{
	aLog("SamDescriptionCommand::execute()");

	ACommand::execute(commandManager, id);

	if(!m_description) {
		aLog("!m_description");
		return;
	}

	m_description->setMetaData(m_propertyId, m_text);
}