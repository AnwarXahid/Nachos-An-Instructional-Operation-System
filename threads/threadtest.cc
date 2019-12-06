// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create several threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustrate the inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
//
// Parts from Copyright (c) 2007-2009 Universidad de Las Palmas de Gran Canaria
//

#include "copyright.h"
#include "system.h"
#include "synch.h"
#include "producer.h"
#include "consumer.h"

List<int> *commonBuffer;
int size;
Lock *l;
Condition *pro;
Condition *con;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 10 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"name" points to a string with a thread name, just for
//      debugging purposes.
//----------------------------------------------------------------------

void
SimpleThreadProducer(void* name)
{
    char* threadName = (char*)name;
    producer *p;
    p=new producer(threadName);
    int i;
    if(size>0)
    {
        l->Acquire();
	i = p->produce();
	con->Signal(l);
        l->Release();
    }
    else
    {
        l->Acquire();
	pro->Wait(l);
	i = p->produce();
	con->Signal(l);
	l->Release(); 
    }
    
   
    printf(">>> Thread %s has finished , %d\n", threadName,i);
    
}

void
SimpleThreadConsumer(void* name)
{
    
    char* threadName = (char*)name;
    consumer *c;
    c=new consumer(threadName);
    int i;
    if(size==2)
    {
	l->Acquire();
        con->Wait(l);
        i = c->consume();
	pro->Signal(l);
	l->Release();
    }
    else
    {
	l->Acquire();
        i = c->consume();
	pro->Signal(l);
	l->Release();
    }
	
    
    
    printf(">>> Thread %s has finished , %d\n", threadName,i);
    
}

/*
void
SimpleThread(void* name)
{
    // Reinterpret arg "name" as a string
    char* threadName = (char*)name;
    
    // If the lines dealing with interrupts are commented,
    // the code will behave incorrectly, because
    // printf execution may cause race conditions.
    for (int num = 0; num < 10; num++) {
        //IntStatus oldLevel = interrupt->SetLevel(IntOff);
	printf("*** thread %s looped %d times\n", threadName, num);
	//interrupt->SetLevel(oldLevel);
        //currentThread->Yield();
    }
    //IntStatus oldLevel = interrupt->SetLevel(IntOff);
    printf(">>> Thread %s has finished\n", threadName);
    //interrupt->SetLevel(oldLevel);
}
*/

//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between several threads, by launching
//	ten threads which call SimpleThread, and finally calling 
//	SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest()
{
    DEBUG('t', "Entering SimpleTest");
    size = 2;
    commonBuffer = new List<int>;
    l = new Lock("bufferLock");
    con = new Condition("con");
    pro = new Condition("pro");
//--------------------starting producer threads-------------------------
    for ( int k=1; k<=10; k++) {
      char* threadname = new char[100];
      sprintf(threadname, "producer %d", k);
      Thread* newThread = new Thread (threadname);
      newThread->Fork (SimpleThreadProducer, (void*)threadname);
    }


//----------------------starting consumer threads-----------------------
    for ( int k=1; k<=10; k++) {
      char* threadname = new char[100];
      sprintf(threadname, "consumer %d", k);
      Thread* newThread = new Thread (threadname);
      newThread->Fork (SimpleThreadConsumer, (void*)threadname);
    }
    
    
    //SimpleThread( (void*)"Hilo 0");
}

