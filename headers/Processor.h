#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "ProcessorRead.h"
#include "ProcessorWrite.h"

class Processor {
public:
    ProcessorRead *processorRead;
    ProcessorWrite *processorWrite;

   
    Processor(InstructionList &readStack, InstructionList &writeStack, std::vector<std::thread> &workers, std::string &fileName, int id);
};

#endif 