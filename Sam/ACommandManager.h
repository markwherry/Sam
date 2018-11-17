#ifndef __ACommandManager_h__
#define __ACommandManager_h__

#include "ACommand.h"
#include "AThread.h"
#include "AWaitEvent.h"
#include "ARingBuffer.h"

namespace Atom {

class ACommandManagerProgress
{
public:
	virtual void startCommand(int commandId) = 0;
	virtual void stopCommand(int commandId, AString message, int messageType) = 0;
	virtual void setProgress(int commandId, float progress) = 0;
	virtual void setProgressLabel(int commandId, int labelId, AString label) = 0;
};

class ACommandManager : public AThread
{
public:
	ACommandManager();
	~ACommandManager();

	void threadProcess(void);

	void execute(ACommand *command);
	void executeInBackground(ACommand *command);
	
	void setProgressReceiver(ACommandManagerProgress *progressReceiver) { m_progressReceiver = progressReceiver; }
	void setProgress(int commandId, float progress);
	void setProgressLabel(int commandId, int labelId, AString label);

protected:
private:
	AWaitEvent *waitForCommand;
	ARingBuffer<ACommand*> *backgroundCommands;
	ACommandManagerProgress *m_progressReceiver;
};

}

#endif // __ACommandManager_h__