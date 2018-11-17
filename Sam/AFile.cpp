#include "AFile.h"
#include "ALog.h"

namespace Atom {

// ----------------------------------------------------------------------------------------------------
AFile::AFile(AString filename)
: m_filename(filename)
{
	//wsprintf("%s\n", m_filename.data());
}

// ----------------------------------------------------------------------------------------------------
int AFile::open(void)
{
	//csDebug("File::open() [%s]", m_filename.toChar());

	hFile = CreateFile(m_filename.data(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE) {
		aLog("Error = %d", GetLastError());
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::create(void)
{
	hFile = CreateFile(m_filename.data(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE) {
		aLog("Error = %d", GetLastError());
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::close(void)
{
	if(CloseHandle(hFile) == 0) {
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::setPosition(int fromFlag, long position)
{
	//aLog("AFile::setPosition(%d)", position);
	
	if(SetFilePointer(hFile, position, NULL, fromFlag) == INVALID_SET_FILE_POINTER) {
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------------------------------
long AFile::position(void)
{
	return SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
}

// ----------------------------------------------------------------------------------------------------
long long AFile::size(void)
{
	LARGE_INTEGER s;

	if(GetFileSizeEx(hFile, &s)) {
		return s.QuadPart;
	}

	return -1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::readBytes(int numBytes, void *buffer)
{
	DWORD bytesRead;
	if(!ReadFile(hFile, buffer, numBytes, &bytesRead, NULL)) {
		return 0;
	}

	return (int)bytesRead;
}

// ----------------------------------------------------------------------------------------------------
int AFile::writeBytes(int numBytes, void *buffer)
{
	DWORD bytesWritten;

	if(WriteFile(hFile, buffer, numBytes, &bytesWritten, NULL)==0) {
		return 0;
	}

	return (int)bytesWritten;
}

int AFile::readFloatLsb(float *value)
{
	floatchar fc;

	if(readBytes(4, &fc.c) != 4) {
		*value = 0;
		return 0;
	}

	*value = fc.f;
	
	return 1;
}

int AFile::writeFloatLsb(float value)
{
	floatchar fc;
	fc.f = value;

	if(writeBytes(4, fc.c)!=4) {
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::readU16Lsb(unsigned short *value)
{
	unsigned char data[2];
	
	if(readBytes(2, &data) != 2) {
		*value = 0;
		return 0;
	}
	
	*value = data[1]<<8;
	*value |= data[0];
	
	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::readU32Lsb(AUInt32 *value)
{
	unsigned char data[4];
	
	if(readBytes(4, &data) != 4) {
		*value = 0;
		return 0;
	}

	*value = data[3]<<24;
	*value |= data[2]<<16;
	*value |= data[1]<<8;
	*value |= data[0];
	
	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::read32Lsb(AInt32 *value)
{
	unsigned char data[4];
	
	if(readBytes(4, &data) != 4) {
		*value = 0;
		return 0;
	}

	*value = data[3]<<24;
	*value |= data[2]<<16;
	*value |= data[1]<<8;
	*value |= data[0];
	
	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int AFile::read64Lsb(AInt64 *value)
{
	char data[8];
	
	if(readBytes(8, &data) != 8) {
		*value = 0;
		return 0;
	}

	*value = data[7]<<56;
	*value |= data[6]<<48;
	*value |= data[5]<<40;
	*value |= data[4]<<32;
	*value |= data[3]<<24;
	*value |= data[2]<<16;
	*value |= data[1]<<8;
	*value |= data[0];
	
	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int AFile::readU64Lsb(AUInt64 *value)
{
	unsigned char data[8];
	
	if(readBytes(8, &data) != 8) {
		*value = 0;
		return 0;
	}

	*value = data[7]<<56;
	*value |= data[6]<<48;
	*value |= data[5]<<40;
	*value |= data[4]<<32;
	*value |= data[3]<<24;
	*value |= data[2]<<16;
	*value |= data[1]<<8;
	*value |= data[0];
	
	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::readU16Msb(unsigned short *value)
{
	unsigned char data[2];

	if(readBytes(2, &data) != 2) {
		*value = 0;
		return 0;
	}

	*value = data[0]<<8;
	*value |= data[1];

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::readU32Msb(AUInt32 *value)
{
	unsigned char data[4];

	if(readBytes(4, &data) != 4) {
		*value = 0;
		return 0;
	}

	*value = data[0]<<24;
	*value |= data[1]<<16;
	*value |= data[2]<<8;
	*value |= data[3];

	return 1;
}

int AFile::read32Msb(AInt32 *value)
{
	char data[4];

	if(readBytes(4, &data) != 4) {
		*value = 0;
		return 0;
	}

	*value = data[0]<<24;
	*value |= data[1]<<16;
	*value |= data[2]<<8;
	*value |= data[3];

	return 1;
}

int AFile::write16Lsb(short value)
{
	char data[2];

	data[0] = (char)value;
	data[1] = (char)(value>>8);

	if(writeBytes(2, data) != 2) {
		return 0;
	}

	return 1;
}

int AFile::write24Lsb(int value)
{
	char data[3];

	data[0] = (char)value;
	data[1] = (char)(value>>8);
	data[2] = (char)(value>>16);

	if(writeBytes(3, data) != 3) {
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::writeU16Lsb(unsigned short value)
{
	unsigned char data[2];

	data[0] = (unsigned char)value;
	data[1] = (unsigned char)(value>>8);

	if(writeBytes(2, data)!=4) {
		return 0;
	} 

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::writeU24Lsb(unsigned int value)
{
	unsigned char data[3];

	data[0] = (unsigned char)value;
	data[1] = (unsigned char)(value>>8);
	data[2] = (unsigned char)(value>>16);

	if(writeBytes(3, data)!=3) {
		return 0;
	} 

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::writeU32Lsb(AUInt32 value)
{
	unsigned char data[4];

	data[0] = (unsigned char)value;
	data[1] = (unsigned char)(value>>8);
	data[2] = (unsigned char)(value>>16);
	data[3] = (unsigned char)(value>>24);

	if(writeBytes(4, data)!=4) {
		return 0;
	} 

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::write32Lsb(AInt32 value)
{
	char data[4];

	data[0] = (char)value;
	data[1] = (char)(value>>8);
	data[2] = (char)(value>>16);
	data[3] = (char)(value>>24);

	if(writeBytes(4, data)!=4) {
		return 0;
	} 

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int AFile::write64Lsb(AInt64 value)
{
	char data[8];

	data[0] = (char)value;
	data[1] = (char)(value>>8);
	data[2] = (char)(value>>16);
	data[3] = (char)(value>>24);
	data[4] = (char)(value>>32);
	data[5] = (char)(value>>40);
	data[6] = (char)(value>>48);
	data[7] = (char)(value>>56);

	if(writeBytes(8, data)!=8) {
		return 0;
	} 

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int AFile::writeU64Lsb(AUInt64 value)
{
	unsigned char data[8];

	data[0] = (unsigned char)value;
	data[1] = (unsigned char)(value>>8);
	data[2] = (unsigned char)(value>>16);
	data[3] = (unsigned char)(value>>24);
	data[4] = (unsigned char)(value>>32);
	data[5] = (unsigned char)(value>>40);
	data[6] = (unsigned char)(value>>48);
	data[7] = (unsigned char)(value>>56);

	if(writeBytes(8, data)!=8) {
		return 0;
	} 

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::readChunkName(char *chunkName)
{
	if(readBytes(4, chunkName) != 4) {
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------------------------------
int AFile::writeChunkName(char *chunkName)
{
	if(writeBytes(4, chunkName) != 4) {
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------------------------------------------
bool AFile::compareChunkName(char *chunkName1, char *chunkName2)
{
	if(memcmp(chunkName1, chunkName2, 4) != 0) {
		return false;
	}

	return true;

	/*bool ret = true;

	for(int i=0; i<4; i++) {
		csDebug("[%d] %d,%d", i, chunkName1[i], chunkName2[i]);
		if(chunkName1[i] != chunkName2[i]) {
			ret = false;
			break;
		}
	}

	return ret;*/
}

}