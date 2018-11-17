#ifndef __AFile_h__
#define __AFile_h__

#include "AString.h"
#include "APlatform.h"
#include "ATypes.h"

namespace Atom {

class AFile
{
public:
	AFile(AString filename);

	int open(void);
	int create(void);
	int close(void);

	AString filename(void) { return m_filename; }

	int setPosition(int fromFlag, long position);
	enum {
		fromBeginning = 0,
		fromCurrent = 1
	};
	long position(void);
	long long size(void);

	int readBytes(int numBytes, void *buffer);
	int writeBytes(int numBytes, void *buffer);

	int readFloatLsb(float *value);
	int writeFloatLsb(float value);

	int readU16Lsb(unsigned short *value);
	int readU32Lsb(AUInt32 *value);
	int read32Lsb(AInt32 *value);
	int read64Lsb(AInt64 *value);
	int readU64Lsb(AUInt64 *value);

	int readU16Msb(unsigned short *value);
	int readU32Msb(AUInt32 *value);
	int read32Msb(AInt32 *value);

	int write16Lsb(short value);
	int write24Lsb(int value);

	int writeU16Lsb(unsigned short value);
	int writeU24Lsb(unsigned int value);
	int writeU32Lsb(AUInt32 value);
	int write32Lsb(AInt32 value);
	int write64Lsb(AInt64 value);
	int writeU64Lsb(AUInt64 value);

	int readChunkName(char *chunkName);
	int writeChunkName(char *chunkName);
	bool compareChunkName(char *chunk1, char *chunk2);

protected:
private:
	AString m_filename;
	HANDLE hFile;
};

}

#endif // __AFile_h__