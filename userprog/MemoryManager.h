#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "copyright.h"
#include "bitmap.h"
#include "synch.h"
#include "machine.h"
#include "list.h"
#include "addrspace.h"

class MemoryManager {

public:
	MemoryManager(int numPages);
	~MemoryManager();

	int AllocPage();
	void FreePage(int physPageNum);
	bool PageIsAllocated(int physPageNum);
	int getFreePages();

private:
	int numPhysPages;
	BitMap* memoryMap;
	Lock* lock;
	int freePages;
};

#endif


