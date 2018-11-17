#include "ACommand.h"

#include "ACommandManager.h"
#include "ALog.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
ACommand::ACommand()
: m_commandManager(0)
, m_id(-1)
, m_feedback(true)
, m_messageType(0)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ACommand::execute(ACommandManager *commandManager, int id)
{
	m_commandManager = commandManager;
	m_id = id;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ACommand::setProgress(float progress)
{
	//aLog("ACommand::setProgress(%f)", progress);
	if(m_commandManager) {
		m_commandManager->setProgress(m_id, progress);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void ACommand::setProgressLabel(int labelId, AString label)
{
	if(m_commandManager) {
		m_commandManager->setProgressLabel(m_id, labelId, label);
	}
}

}