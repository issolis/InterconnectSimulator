#ifndef PROCESSORREAD_H
#define PROCESSORREAD_H

#include <vector>
#include <string>
#include <thread>
#include "InstructionList.h"

class ProcessorRead {
public:

    ProcessorRead(InstructionList &readStack, std::vector<std::thread>& worker, int id);
    std::vector<std::thread>* workers;
    InstructionList *readStack;
    int id; 
    void processorThreadFunction();
    void processorThread();

};

#endif