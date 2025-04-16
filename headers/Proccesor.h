#pragma once
#include <vector>
#include <string>
#include <thread>
#include "InstructionList.h"
#include "InstructionMemory.h"
#include "CacheMemory.h"

class Proccesor {
public:

    Proccesor(InstructionList &stack, std::vector<std::thread>& worker, std::string& fileName);
    std::vector<std::thread>* worker;
    
    CacheMemory *cache; 

    InstructionMemory *instrMem; 

    void processorThreadFunction(std::string instr);
    void processorThread(std::string instr);
    void sendOneInstruction();


    std::string manipulateInstruction(std::string &str); 

    InstructionList* stack;
    std::vector<std::thread>* workers;
};
