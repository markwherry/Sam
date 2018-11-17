#ifndef __SamVersionCommand_h__
#define __SamVersionCommand_h__

#include "ACommand.h"
#include "AList.h"
#include "AString.h"
using namespace Atom;

#include "SamVersion.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SamVersionCommand : public ACommand
{
public:
	SamVersionCommand(SamVersion *version, int propertyId, float value);
	SamVersionCommand(SamVersion *version, int propertyId, AString text);

	void execute(ACommandManager *commandManager = 0, int id = 0);

	enum SamZonePropertyId {
		Name                      = 0,
		DefaultVoices             = 1,
		Reference                 = 2
	};

protected:
private:
	SamVersion *m_version;
	int m_propertyId;
	float m_value;
	AString m_text;
};

#endif