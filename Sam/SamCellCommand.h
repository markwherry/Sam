#ifndef __SamCellCommand_h__
#define __SamCellCommand_h__

#include "ACommand.h"
#include "AList.h"
#include "AString.h"
using namespace Atom;

#include "SamCell.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SamCellCommand : public ACommand
{
public:
	SamCellCommand(SamCell *cell, int propertyId, float value);
	//SamCellCommand(SamZone *zone, int propertyId, AString text);
	SamCellCommand(SamCell *cell, int propertyId, bool state);

	void execute(ACommandManager *commandManager = 0, int id = 0);

	enum SamCellPropertyId {
		BaseNote           = 0,
		FineTune           = 1,
		SourceStart        = 2,
		ReleaseStart       = 3,
		//ReleaseAttenuation = 4,

		LoopEnable         = 5,
		LoopStart          = 6,
		LoopEnd            = 7
	};

protected:
private:
	SamCell *m_cell;
	int m_propertyId;
	float m_value;
	//AString m_text;
	bool m_state;
};

#endif