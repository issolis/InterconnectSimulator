#include "Interconnect.h"
#include <iostream>
#include <cstring>

Interconnect::Interconnect(InstructionList& stack) {
    this->stack = &stack;
    startMonitoring(); 

}

void Interconnect::receiveMessage( ){ 
    if (strcmp(stack->executeStackOperation(3, "NOINSTR"), "notnull") == 0) {
        char* instr = stack->executeStackOperation(4, "NOINSTR"); 
        stack->executeStackOperation(2, "NOINSTR"); 


        if (strcmp(instr, "WRITE") == 0) {
            std:: cout << "EXECUTING WRITE" << std::endl; 
        }
        if (strcmp(instr, "READ") == 0) {
            std:: cout << "EXECUTING READ" << std::endl; 
        }

    }
}
void Interconnect::showStack(){

}

void Interconnect::startMonitoring() {
    running = true;
    monitor = std::thread([this]() {
        while (running) {
            receiveMessage(); 
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void Interconnect::join(){
    monitor.join(); 
}



