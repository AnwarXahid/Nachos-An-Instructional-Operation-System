#include "list.h"

extern List<int> *commonBuffer;
extern int size;

class producer{
public:
	producer(char* producerName);
	char* getName() { return (name); }
	int produce();	

private:
	char* name;

};
