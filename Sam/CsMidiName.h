#ifndef __CsMidiName_h__
#define __CsMidiName_h__

#include "AString.h"

class CsMidiName
{
public:
	static Atom::AString pitchToName(int pitch, int middleCOctave = 3, int namingScheme = 0);
	static Atom::AString controllerToName(int controller, int type);
protected:
private:
};

#endif // __CsMidiName_h__