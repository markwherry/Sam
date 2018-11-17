#ifndef __ScBounceInstrument_h__
#define __ScBounceInstrument_h__

#include "ACommand.h"
#include "AList.h"
#include "AString.h"
using namespace Atom;

#include "CsAudioBuffer.h"
using namespace Cs;

#include "SamInstrument.h"
//#include "SamStreamMix.h"
#include "SamBounceGroup.h"

class ScBounceInstrument : public ACommand
{
public:
	ScBounceInstrument(SamInstrument *instrument, AString filename, SamStreamMix *streamMix, AList<SamBounceGroup*> *bounceGroups);

	void execute(ACommandManager *commandManager = 0, int id = 0);

	void bounceElement(SamElement *element, CsAudioBuffer **audioBuffers);

protected:
private:
	AString m_filename;
	SamInstrument *m_srcInstrument;
	SamInstrument *m_dstInstrument;
	SamStreamMix *m_streamMix;
	AList<SamBounceGroup*> *m_bounceGroups;
	float **m_decompressionBuffer;
};

#endif // __ScBounceInstrument_h__