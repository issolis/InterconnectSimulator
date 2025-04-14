#pragma once
#include <vector>
#include <string>
#include <thread>
#include "InstructionList.h"

class Proccesor {
public:
    std::vector<std::thread>* worker;
    Proccesor(InstructionList &stack, std::vector<std::thread>& worker);
    void processorThreadFunction(std::string instr);
    void processorThread(std::string instr);


    InstructionList* stack;
    std::vector<std::thread>* workers;
};
