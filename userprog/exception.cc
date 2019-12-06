// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "Table.h"


extern Table *pt;
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    int memval, vaddr;
    OpenFile *of;

    if (which == SyscallException) {
	 switch(type) 
	{ 
        case SC_Halt: 
        { 
              DEBUG('a', "Shutdown, initiated by user program.\n"); 
              interrupt->Halt(); 
              break; 
        }

	 case SC_Exec:
	 {

		char filename[100];
       		int i=0;

      		 vaddr = machine->ReadRegister(4);
      		  machine->ReadMem(vaddr, 1, &memval);
        	while ((*(char*)&memval) != '\0') {
          	  filename[i]  = (char)memval;
           		 ++i;
           		 vaddr++;
           		 machine->ReadMem(vaddr, 1, &memval);
        	}
        	filename[i]  = (char)memval;

        	OpenFile *executable = fileSystem->Open(filename);
       		 AddrSpace *space;

       		 if (executable == NULL) {
            		printf("Unable to open file %s\n", filename);
           		 return;
       		 }
		int id = pt->Alloc(executable);
    		//ASSERT(id!=-1);
        	space = new AddrSpace(executable);    
        	currentThread->space = space;

        	delete executable;			// close file

        	space->InitRegisters();		// set the initial register values
        	space->RestoreState();		// load page table register

        	machine->Run();			// jump to the user progam
		pt->Release(id);
		ASSERT(false);
		break;
	}

	case SC_Exit:
	{
		int exitStatus = machine->ReadRegister(4);
		currentThread->Finish();
		DEBUG('a', "%d.\n",exitStatus);
		break;
		
	}
	case SC_Read:
	{
		int bufAddr = machine->ReadRegister(4);
		int NumBuf = machine->ReadRegister(5);
		int m_index = machine->ReadRegister(6);
		char *buf = new char[NumBuf];
		of = new OpenFile(m_index);
		of->ReadAt(buf, NumBuf, 0);
		int bufAddrDemo = bufAddr;
		for(int k = 0 ; k<NumBuf ; k++,bufAddrDemo++){
			machine->WriteMem(bufAddrDemo, 1 , buf[k]);
		}
		machine->WriteRegister(2,bufAddr);
		break;			
	}

	case SC_Write:
	{
		int bufAddr = machine->ReadRegister(4);
		int NumBuf = machine->ReadRegister(5);
		int m_index = machine->ReadRegister(6);
		char *buf = new char[NumBuf];
		int bufAddrDemo = bufAddr;
		for(int k = 0 ; k<NumBuf ; k++,bufAddrDemo++){
			machine->ReadMem(bufAddrDemo, 1, (int*) buf);
		}
		of = new OpenFile(m_index);
		of->WriteAt(buf, NumBuf, 0);
		break;			
	}
    }
  }

  	if (which != SyscallException) {
	 switch(type) 
	{ 
            case ReadOnlyException:
     		 printf("Read Only Exception\n");
    		  Exit(-1);
    		  break;
   	    case BusErrorException:
   		   printf("Bus Error Exception\n");
   		   Exit(-1);
   		   break;
  	     case OverflowException:
   		   printf("Overflow Exception\n");
    		  Exit(-1);
    		  break;
   	     case IllegalInstrException:
    		  printf("Illegal Instruction Exception\n");
    		  Exit(-1);
     		 break;
   	    default:
    		  printf("Unknown Exception\n");
    		  Exit(-1);
		break;
  	}
  }
}	





