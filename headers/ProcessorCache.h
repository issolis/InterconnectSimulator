#ifndef PROCESSORCACHE_H
#define PROCESSORCACHE_H

#include "InstructionList.h"
#include "CacheMemory.h"

#include <vector>
#include <thread>

class ProcessorCache
{

public:
    ProcessorCache(InstructionList &readCacheStack, InstructionList &writeCacheStack,  std::vector<std::thread> &worker, int id);
    
    std::vector<std::thread>* workers;
    InstructionList *readCacheStack; 
    InstructionList *writeCacheStack; 
    CacheMemory *cacheMemory;
    int id; 

    void processorThreadFunction();
    void processorThread();

    void processorWriteThreadFunction(std::string instr);
    void processorWriteThread(std::string instr);
};

#endif