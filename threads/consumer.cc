#include "consumer.h"

consumer::consumer(char* consumerName)
	{
		name = consumerName;
	}

int consumer::consume()
{
    int r = commonBuffer->Remove();
    size++;
    return r;
}
