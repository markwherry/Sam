#ifndef __AMemoryManager_h__
#define __AMemoryManager_h__

namespace Atom {

class AMemoryManager
{
public:
	static void* _malloc(long long size);
	static void _free(void *data, long long size);
protected:
private:
};

}

#define aMalloc Atom::AMemoryManager::_malloc
#define aFree   Atom::AMemoryManager::_free

#endif