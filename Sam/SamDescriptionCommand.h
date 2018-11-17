#ifndef __SamDescriptionCommand_h__
#define __SamDescriptionCommand_h__

#include "ACommand.h"
#include "AList.h"
#include "AString.h"
using namespace Atom;

#include "SamDescription.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SamDescriptionCommand : public ACommand
{
public:
	SamDescriptionCommand(SamDescription *description, int propertyId, AString text);

	void execute(ACommandManager *commandManager = 0, int id = 0);

protected:
private:
	SamDescription *m_description;
	int m_propertyId;
	AString m_text;
};

#endif