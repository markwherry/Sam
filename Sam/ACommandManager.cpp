#include "ACommandManager.h"
#include "ALog.h"

namespace Atom {

ACommandManager::ACommandManager()
: m_progressReceiver(0)
{
	aLog("CommandManager::CommandManager()");

	backgroundCommands = new ARingBuffer<ACommand*>(32);
	
	waitForCommand = new AWaitEvent();

	startThread();

	aLog("EXIT: CommandManager::CommandManager()");
}

ACommandManager::~ACommandManager()
{
	aLog("CommandManager::~CommandManager()");
	stopThread();
	waitForCommand->set();
	waitForThreadToExit();
	aLog("EXIT: CommandManager::~CommandManager()");
}

void ACommandManager::threadProcess(void)
{
	while(isThreadRunning()) {
		//aLog("Waiting for command...");
		
		if(backgroundCommands->size()>0) {
			ACommand *command = backgroundCommands->get();
			if(m_progressReceiver && command->isFeedback()) {
				m_progressReceiver->startCommand(command->id());
			}
			aLog("^*^*^*^*^*^*^*^*^*^*^ START COMMAND %s ^*^*^*^*^*^*^*^*^*^*^", command->name().toChar());
			command->execute(this);
			aLog("^*^*^*^*^*^*^*^*^*^*^ STOP COMMAND %s ^*^*^*^*^*^*^*^*^*^*^", command->name().toChar());
			if(m_progressReceiver && command->isFeedback()) {
				aLog("Going to trigger stopCommand");
				m_progressReceiver->stopCommand(command->id(), command->message(), command->messageType());
			} else {
				aLog("Not triggering stop Command.");
			}//*/
			delete command;
		} else {
			waitForCommand->wait();
		}
	}
	aLog("ACommandManager::threadProcess() Exiting...");
}

void ACommandManager::execute(ACommand *command)
{
	aLog("ACommandManager::execute()");
	command->execute(this);
	delete command;
}

void ACommandManager::executeInBackground(ACommand *command)
{
	aLog(" *-> ACommandManager::executeInBackground()");

	backgroundCommands->add(command);
	waitForCommand->set();
}

void ACommandManager::setProgress(int commandId, float progress)
{
	//aLog("CommandManager::setProgress(%d, %f)", commandId, progress);
	if(m_progressReceiver) {
		m_progressReceiver->setProgress(commandId, progress);
	}
}

void ACommandManager::setProgressLabel(int commandId, int labelId, AString label)
{
	//aLog("CommandManager::setProgressLabel(%d, %d, %s)", commandId, labelId);
	if(m_progressReceiver) {
		m_progressReceiver->setProgressLabel(commandId, labelId, label);
	}
}

}