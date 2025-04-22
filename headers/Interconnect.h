#ifndef INTERCONNECT_H
#define INTERCONNECT_H

#include "InstructionList.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <thread>
#include "List.h"
#include "SharedMemory.h"


class Interconnect
{
private:
    

public:

    Interconnect(InstructionList& stack, InstructionList& cacheWriteStack, List& stacks, List& cacheReadList);
    
    InstructionList* stack; 
    InstructionList* writeCacheStack;
    List* readStackList;
    List* cacheReadList;
    SharedMemory* sharedMemory;

    std::thread monitor; 
    std::atomic<bool> running{false}; 
  
    void receiveMessage();
    void showStack();
    void startSnooping();
    void join();
};

#endif
