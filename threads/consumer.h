#include "list.h"
//#include "producer.h"

extern List<int> *commonBuffer;
extern int size;

class consumer{
public:
	consumer(char* consumerName);
	char* getName() { return (name); }
	int consume();	

private:
	char* name;

};
