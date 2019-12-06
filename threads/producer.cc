#include "producer.h"

producer::producer(char* producerName)
	{
		name = producerName;
	}


int producer::produce()
{
	int r = rand()%299;
	commonBuffer->Append(r);
        size--;
	return r;
}
