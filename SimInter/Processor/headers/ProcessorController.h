#ifndef PROCESSORCONTROLLER_H
#define PROCESSORCONTROLLER_H

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include "InstructionList.h"
#include "Interconnect.h"
#include "InstructionMemory.h"
#include "List.h"
#include "Processor.h"
#include "SharedMemory.h"

class ProcessorController{
    public: 

    ProcessorController(std::vector<std::thread>& workers);

    int step(int step);
    int previous = 0;

    std::vector<std::thread>* workers;
    InstructionList *writeStack; 
    InstructionList *writeCacheStack; 
    
   
    std::vector<ProcessorWrite> processorsWrite; 
    std::vector<ProcessorRead> processorsRead; 
    std::vector<Processor> processors;
    std::vector<InstructionList> readStacks;
    std::string paths[8] = {"../../InstructionsFile/InstructionsP1.txt", "../../InstructionsFile/InstructionsP2.txt", "../../InstructionsFile/InstructionsP3.txt",
        "../../InstructionsFile/InstructionsP4.txt", "../../InstructionsFile/InstructionsP5.txt", "../../InstructionsFile/InstructionsP6.txt", "../../InstructionsFile/InstructionsP7.txt",
        "../../InstructionsFile/InstructionsP8.txt"};

    List *readStackList = new List(); 
    List *readCacheStackList = new List();

    Interconnect *interconnectBus; 
    InstructionList *responsesStack; 
    InstructionList *requestStack; 
    void closeExecution();

    int completeExecution();

 

};       
#endif
