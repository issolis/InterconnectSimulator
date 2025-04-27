#ifndef PROCESSORREAD_H
#define PROCESSORREAD_H

#include <vector>
#include <string>
#include <thread>
#include "InstructionList.h"
#include "CacheMemory.h"

class ProcessorRead {
public:

    ProcessorRead(InstructionList &readStack, std::vector<std::thread>& worker, int id, InstructionList &responsesStack);
    std::vector<std::thread>* workers;
    InstructionList *readStack;
    int id; 
    InstructionList *responsesStack;
    void processorThreadFunction();
    void processorThread();

    void processorResponseThreadFunction(std::string instr);
    void processorResponseThread(std::string instr);
};

#endif