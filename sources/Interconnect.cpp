#include "Interconnect.h"
#include <iostream>
#include <cstring>

Interconnect::Interconnect(InstructionList& stack, InstructionList& responseStack, std::vector<std::thread>& responseThreads) {
    this->stack = &stack;
    this->responseThreads = &responseThreads; 
    this->responseStack = &responseStack; 
    startSnooping(); 
}

enum class state {
    READ, 
    EXECUTE, 
    VALIDATE, 
    COMMIT, 
    RETRY
}; 

struct thread_context {
    state current_state {state::READ};
    int start_size {0};
    bool committed {false};
};


void Interconnect::receiveMessage( ){ 
    if (strcmp(stack->executeStackOperation(3, "NOINSTR"), "notnull") == 0) {
        char* instr = stack->executeStackOperation(4, "NOINSTR"); 
        stack->executeStackOperation(2, "NOINSTR"); 


        if (strcmp(instr, "WRITE 1") == 0) {
            //std:: cout << "EXECUTING WRITE" << std::endl; 
            responseStack->executeStackOperation(1, "WRITE RESPONSE"); 
            //responseStack->showStack(); 

        }
        if (strcmp(instr, "READ 1") == 0) {
            //std:: cout << "EXECUTING READ" << std::endl; 
            responseStack->executeStackOperation(1, "READ RESPONSE"); 
            //responseStack->showStack(); 
        }

    }
}
void Interconnect::showStack(){

}

void Interconnect::startSnooping() {
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



