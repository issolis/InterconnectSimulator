#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "ProcessorRead.h"
#include "ProcessorWrite.h"
#include "ProcessorCache.h"

class Processor {
public:
    ProcessorRead  *processorRead;
    ProcessorWrite *processorWrite;
    ProcessorCache *processorCache;
    InstructionList *cacheReadStack; 
    InstructionList *cacheWriteStack;

   
    Processor(InstructionList &readStack, InstructionList &writeStack, InstructionList &readCacheStack, InstructionList &writeCacheStack, std::vector<std::thread> &workers, std::string &fileName, int id);
};

#endif 