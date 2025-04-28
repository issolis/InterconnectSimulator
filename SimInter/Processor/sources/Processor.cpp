#include "../../Processor/headers/Processor.h"


Processor::Processor(InstructionList &readStack, InstructionList &writeStack, InstructionList &readCacheStack, InstructionList &writeCacheStack, std::vector<std::thread> &workers, std::string &fileName, int id) {
    cacheMemory = new CacheMemory();
   
    processorRead  = new ProcessorRead(readStack, workers, id);
    processorWrite = new ProcessorWrite(writeStack, workers, *cacheMemory, fileName, id);
    processorCache = new ProcessorCache(readCacheStack, writeCacheStack,workers, *cacheMemory, id);
}
