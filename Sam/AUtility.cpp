#include "AUtility.h"

#include "ALog.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int AUtility::numProcessesByName(AString processName)
{
	if(processName.length() < 1) {
		return 0;
	}

	int numProcesses = 0;
	DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

	if(!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return 0;
	}

    cProcesses = cbNeeded / sizeof(DWORD);

	for(i=0; i<cProcesses; i++) {
		if(aProcesses[i]!=0) {
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);
			if(hProcess != NULL) {
				HMODULE hModule;
				DWORD cbNeeded;
				if(EnumProcessModules( hProcess, &hModule, sizeof(hModule), &cbNeeded)) {
					GetModuleBaseName( hProcess, hModule, szProcessName,sizeof(szProcessName)/sizeof(TCHAR) );
					//_tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, aProcesses[i] );
					if(wcscmp(szProcessName, processName.data()) == 0) {
						numProcesses++;
					}
				}
			}
			CloseHandle(hProcess);
		}
	}


	return numProcesses;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float AUtility::physicalMemoryUsage(void)
{
	MEMORYSTATUS mst;
	GlobalMemoryStatus(&mst);

	return 100.0-(((float)mst.dwAvailPhys / (float)mst.dwTotalPhys) * 100.0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
char* AUtility::encodeBase16(unsigned char *data, int size, int lineLength)
{
	char hex[] = "0123456789ABCDEF";

	const unsigned char map[256] = {
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,

		64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
		64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,

		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
	};

	int numBytesRequired = size * 2;


	int numLines = size / lineLength;
	//printf("numLines = %d\n", numLines);


	//printf("numBytesRequired = %d\n", numBytesRequired);

	char *encoded = (char*)malloc(numBytesRequired+(numLines*2)+2);
	memset(encoded, 0, numBytesRequired+(numLines*2)+2);

	int counter = 0;
	int lineCounter = 0;

	for(int i=0; i<size; i++) {
		/*unsigned char value = data[i];
		char lower = hex[value>>4];
		char upper = hex[value&0xF];


		printf("0x%c%c, ", hex[value>>4], hex[value&0xF]);*/

		encoded[counter++] = hex[data[i]>>4];
		encoded[counter++] = hex[data[i]&0xF];

		lineCounter++;
		if(lineCounter >= lineLength) {
			encoded[counter++] = 0x0D; // Carriage Return
			encoded[counter++] = 0x0A; // New Line
			lineCounter = 0;
		}
	}

	return encoded;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
unsigned char* AUtility::decodeBase16(char *data, int size)
{
	aLog("AUtility::decodeBase16(size = %d)", size);

	char asciiToHex[256] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,

		0, 10, 11, 12, 13, 14, 15, 16, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 10, 11, 12, 13, 14, 15, 16, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	// How many line breaks?
	int numLineBreaks = 0;
	for(int i=0; i<size; i++) {
		if(data[i] == 0x0A) {
			numLineBreaks++;
		}
	}
	aLog("numLineBreaks = %d", numLineBreaks);

	unsigned char *decoded = (unsigned char*)malloc((size-(numLineBreaks*2))/2); aLog("decoded size = %d", (size-(numLineBreaks*2))/2);

	int counter = 0;

	//aLog("[");
	for(int i=0; i<size; i+=2) {
		if(data[i]!=0x0D && data[i+1] != 0x0A) {
			char c = asciiToHex[data[i]]<<4;
			c |= asciiToHex[data[i+1]];
			//aLog("%c", c);
			decoded[counter++] = c;
		}
	}
	//aLog("]");

	return decoded;
}

int  AUtility::decodeBase16Size(char *data, int size)
{
	// How many line breaks?
	int numLineBreaks = 0;
	for(int i=0; i<size; i++) {
		if(data[i] == 0x0A) {
			numLineBreaks++;
		}
	}
	aLog("numLineBreaks = %d", numLineBreaks);

	return (size-(numLineBreaks*2))/2;
}

}