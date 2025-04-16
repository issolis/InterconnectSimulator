#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include "InstructionList.h"
#include "Interconnect.h"
#include "InstructionMemory.h"
#include "Proccesor.h"
#include "ProcessorRead.h"
#include "List.h"



int main() {
    std::cout<< std::endl << std::endl << std::endl << std::endl << std::endl;
    std::cout<< "PROGRAM EXECUTION" << std::endl << std::endl << std::endl << std::endl << std::endl;
    
    std::vector<std::thread>* workers = new std::vector<std::thread>();
    std::vector<std::thread>* responseThreads = new std::vector<std::thread>();
    std::atomic<int> total_successes{0};
    InstructionList *stack = new InstructionList ();    
    InstructionList *responseStack = new InstructionList ();  
   
    std::vector<Proccesor> processors; 
    std::vector<ProcessorRead> processorsRead; 
    std::vector<InstructionList> readStacks; 
    std::string paths[8] = {"InstructionsFile/InstructionsP1.txt", "InstructionsFile/InstructionsP2.txt", "InstructionsFile/InstructionsP3.txt",
        "InstructionsFile/InstructionsP4.txt", "InstructionsFile/InstructionsP5.txt", "InstructionsFile/InstructionsP6.txt", "InstructionsFile/InstructionsP7.txt",
        "InstructionsFile/InstructionsP8.txt"};


    List *stackList = new List(); 
    for (int i = 0; i < 8; i++){
        stackList->insertList(i); 
    }

    Interconnect interconnectBus = Interconnect(*stack, *stackList);

    
    std::string stringList[5] = {"WRITE", "READ", "INV", "INVALIDATEALL", "RESPONSE"};

    
    for (int i = 0; i < 8; i++) {
        processors.emplace_back(*stack, *workers, paths[i]);
        processorsRead.emplace_back(*stackList->getListByPos(i)->getList(), *responseThreads, i);
    }


    for (int i = 0; i < 8; i++) {
        processorsRead[i].processorThread(); 
    }
    for(int j = 0; j<10; j++){
        for (int i = 0; i < 8; i++) {
            processors[i].sendOneInstruction(); 
        }
    }


    for (auto& t : *workers) {
        t.join();
    }

    //stack->showStack(); 

    interconnectBus.join(); 
    
    
    return 0; 

}
