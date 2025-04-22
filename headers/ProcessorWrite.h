#pragma once
#include <vector>
#include <string>
#include <thread>
#include "InstructionList.h"
#include "InstructionMemory.h"
#include "CacheMemory.h"

class ProcessorWrite {
public:

    ProcessorWrite(InstructionList &stack, std::vector<std::thread>& worker, CacheMemory &cacheMemory, std::string& fileName);
    std::vector<std::thread>* worker;
    
    CacheMemory *cacheMemory; 

    InstructionMemory *instrMem; 

    void processorThreadFunction(std::string instr);
    void processorThread(std::string instr);
    void sendOneInstruction();


    std::string manipulateInstruction(std::string &str); 

    InstructionList* stack;
    std::vector<std::thread>* workers;
};
