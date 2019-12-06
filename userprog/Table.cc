#include "copyright.h"
#include "Table.h"
	

Table::Table(int size){
	tabsize = size;
	tab = new void*[size];
	inUse = new bool[size];
	for (int i=0;i<size;i++){
		tab[i] = NULL;
		inUse[i] = false;
	}
	lock = new Lock("TableLock");
}
Table::~Table(){
	delete[] tab;
	delete lock;
}


int Table::Alloc(void *object){
	lock->Acquire();
	 
	for (int i=1;i<tabsize;i++){
		if ( inUse[i]==false){
			inUse[i]=true;
			tab[i]=object;
			lock->Release();
			return i;
		}
	}
	printf("process table is full\n");
	lock->Release();
	return -1;
}

void* Table::Get(int index){
	lock->Acquire();
	if (index < 1 || index >= tabsize || !inUse[index]){
		lock->Release();
		return NULL;
	}else{
		lock->Release();
		return tab[index];
	}
}


void Table::Release(int index){
	lock->Acquire();
	
	 
	if (index > 0 && index < tabsize && inUse[index] ) {
		inUse[index] = false;
	}
	
	lock->Release();
}
