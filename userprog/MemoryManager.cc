#include "copyright.h"
#include "MemoryManager.h"
#include "system.h"
#include "sysdep.h"
#include "utility"




MemoryManager::MemoryManager(int numPages)
{
	numPhysPages = numPages;
	memoryMap = new BitMap(numPages);
	lock = new Lock("memoryLock");
	freePages = numPages;
}





MemoryManager::~MemoryManager()
{
	delete memoryMap;
	delete lock;
}






int MemoryManager::AllocPage()
{
	lock->Acquire();
	int physNum = memoryMap->Find();
	freePages--;		
	lock->Release();
	return physNum;
}






void MemoryManager::FreePage(int physPageNum){
	lock->Acquire();
	memoryMap->Clear(physPageNum);
	lock->Release();
}






bool MemoryManager::PageIsAllocated(int physPageNum) {
	lock->Acquire();
	bool physAllocated = memoryMap->Test(physPageNum);
	lock->Release();
	return physAllocated;
}

int MemoryManager::getFreePages(){
	return freePages;
}
