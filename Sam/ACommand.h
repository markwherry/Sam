#ifndef __ACommand_h__
#define __ACommand_h__

#include "AString.h"

namespace Atom {

class ACommandManager;

class ACommand
{
public:
	ACommand();

	virtual void execute(ACommandManager *commandManager = 0, int id = 0);
	virtual void undo(void) { }

	int id(void) { return m_id; }

	AString name(void) { return m_name; }
	void setName(AString name) { m_name = name; }

	bool isFeedback(void) { return m_feedback; }
	void setFeedback(bool feedback) { m_feedback = feedback; }

	void setProgress(float progress);
	void setProgressLabel(int labelId, AString label);

	AString message(void) { return m_message; }
	void setMessage(AString message) { m_message = message; }

	int messageType(void) { return m_messageType; }
	void setMessageType(int messageType) { m_messageType = messageType; }

	ACommandManager* commandManager(void) { return m_commandManager; }
	void setCommandManager(ACommandManager *commandManager) { m_commandManager = commandManager; }

protected:
	

private:
	int m_id;
	AString m_name;
	bool m_feedback;
	ACommandManager *m_commandManager;
	AString m_message;
	int m_messageType;
};

}

#endif