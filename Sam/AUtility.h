#ifndef __AUtility_h__
#define __AUtility_h__

#include "APlatform.h"
#include "AString.h"

#include <psapi.h>

namespace Atom {



class AUtility
{
public:
	static int numProcessesByName(AString processName);
	static float physicalMemoryUsage(void);

	static char* encodeBase16(unsigned char *data, int size, int lineLength);
	static unsigned char* decodeBase16(char *data, int size);
	static int decodeBase16Size(char *data, int size);
};

}

#endif