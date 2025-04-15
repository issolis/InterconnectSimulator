#pragma once
#include <vector>
#include <string>
#include <thread>
#include "InstructionList.h"
#include "InstructionMemory.h"

class Proccesor {
public:

    Proccesor(InstructionList &stack, std::vector<std::thread>& worker, std::string& fileName);
    std::vector<std::thread>* worker;
    

    InstructionMemory *instrMem; 

    void processorThreadFunction(std::string instr);
    void processorThread(std::string instr);
    void sendOneInstruction();

    InstructionList* stack;
    std::vector<std::thread>* workers;
};
