#ifndef __SamToolMaster_h__
#define __SamToolMaster_h__

#include "SamEvent.h"
#include <AString.h>

class SamVoice;

class SamToolMaster
{
public:
	virtual void parameterChanged(int id, float value, void *from) { }
	virtual void parameterAssigned(int id) { }

	virtual bool isPlaying(void) { return false; }

	virtual void queueEvent(SamEvent ev) { }

	virtual int numVoices(void) { return 0; }
	virtual SamVoice* voice(int index) { return 0; }

	virtual int numArticulations(void) { return 0; }
	virtual AString articulationName(int index) { return ""; }

protected:
private:
};

#endif