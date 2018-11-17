#ifndef __ATypes_h__
#define __ATypes_h__

namespace Atom {

typedef char AByte;
typedef unsigned char AUByte;

typedef short AInt16;
typedef unsigned short AUInt16;

typedef int AInt32;
typedef unsigned int AUInt32;

typedef long long AInt64;
typedef unsigned long long AUInt64;

typedef int AStatus;
enum {
	AOk = 1,
	AError = 0
};

struct ARiffChunk
{
	char name[4];
	AUInt32 size;
};


}

union floatchar
{
	float f;
	char c[4];
};

#endif // __Types_h__