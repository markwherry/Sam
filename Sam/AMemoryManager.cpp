#include "AMemoryManager.h"
#include "ALog.h"

namespace Atom {

void* AMemoryManager::_malloc(long long size)
{
	void *data = 0;
	
	data = malloc(size);
	memset(data, 0, size);

	/*data = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
	if(!data) {
		cDebug("MemoryManager::malloc() data = NULL");
		return 0;
	}*/

	//ZeroMemory(data, size); // This is done by VirtualAlloc?

	/*int ret = VirtualLock(data, size);
	if (!ret) {
		cDebug("MemoryManager::malloc() VirtualLock Fail");
	}*/

	return data;
}

void AMemoryManager::_free(void *data, long long size)
{
	if(data) {
		free(data);
	}

	/*if(data) {
		BOOL bRet = VirtualUnlock(data, size);
		if(!bRet) {
			cDebug("MemoryManager::free() VirtualUnlock FAIL (GetLastError=%d)", GetLastError());
		}
		
		VirtualFree(data, 0, MEM_RELEASE);
	}*/
}

}