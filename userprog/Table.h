#ifndef TABLE_H
#define TABLE_H

#include "copyright.h"
#include "synch.h"

class Table {
public:
	Table(int size);
	~Table();

		
	int Alloc(void *object);

	void *Get(int index);

		
	void Release(int index) ;

private:
	void** tab;
	bool* inUse;
	int tabsize;
	Lock* lock;
};

#endif
