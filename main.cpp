#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include "InstructionList.h"
#include "Interconnect.h"
#include "InstructionMemory.h"
#include "Proccesor.h"



int main() {
    std::cout<< std::endl << std::endl << std::endl << std::endl << std::endl;
    std::cout<< "PROGRAM EXECUTION" << std::endl << std::endl << std::endl << std::endl << std::endl;
    
    std::vector<std::thread>* workers = new std::vector<std::thread>();
    std::atomic<int> total_successes{0};

    
    InstructionList *stack = new InstructionList ();    
    Interconnect interconnectBus = Interconnect(*stack); 

    std::vector<Proccesor> processors;
    

    for (int i = 0; i < 8; ++i) {
        processors.emplace_back(*stack, *workers);
    }

    std::string stringList[5] = {"WRITE", "READ", "INV", "INVALIDATEALL", "RESPONSE"};


    
    for (int i = 0; i < 5; ++i) {
        processors[i].processorThread(stringList[i]);
    }


    for (auto& t : *workers) {
        t.join();
    }

    stack->showStack(); 

    interconnectBus.join(); 
    
    


    return 0; 

}
