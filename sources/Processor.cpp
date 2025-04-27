#include "Processor.h"


Processor::Processor(InstructionList &readStack, InstructionList &writeStack, InstructionList &readCacheStack, InstructionList &writeCacheStack, std::vector<std::thread> &workers, std::string &fileName, int id, InstructionList &responsesStack, InstructionList &requestStack) {
    cacheMemory = new CacheMemory();
    this->responsesStack = &responsesStack;
    processorRead  = new ProcessorRead(readStack, workers, id, responsesStack);
    processorWrite = new ProcessorWrite(writeStack, workers, *cacheMemory, fileName, id, requestStack);
    processorCache = new ProcessorCache(readCacheStack, writeCacheStack,workers, *cacheMemory, id);
    
}