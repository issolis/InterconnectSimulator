#include "Interconnect.h"
#include <iostream>
#include <cstring>

Interconnect::Interconnect(InstructionList& stack,  List& stacks) {
    this->stack = &stack; 
    this->stacks = &stacks; 
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
        if (strcmp(instr, "WRITE '") == 0) {
            std:: cout << "EXECUTING WRITE 1 TO P0" << std::endl; 
            stacks->getListByPos(0)->getList()->executeStackOperation(1, "WRITE RESPONSE"); 
        }
        else if (strcmp(instr, "READ 0") == 0) {
            std:: cout << "EXECUTING READ 1 TO P0" << std::endl; 
            stacks->getListByPos(0)->getList()->executeStackOperation(1, "READ RESPONSE");
        }
        else if (strcmp(instr, "WRITE 1") == 0) {
            std:: cout << "EXECUTING WRITE 2 TO P1" << std::endl; 
            stacks->getListByPos(1)->getList()->executeStackOperation(1, "WRITE RESPONSE");
        }
        else if (strcmp(instr, "READ 1") == 0) {
            std:: cout << "EXECUTING READ 2 TO P1" << std::endl; 
            stacks->getListByPos(1)->getList()->executeStackOperation(1, "READ RESPONSE");
        }
        else if (strcmp(instr, "WRITE 2") == 0) {
            std:: cout << "EXECUTING WRITE 2 TO P2" << std::endl; 
            stacks->getListByPos(2)->getList()->executeStackOperation(1, "WRITE RESPONSE");
        }
        else if (strcmp(instr, "READ 2") == 0) {
            std:: cout << "EXECUTING READ 2 TO P2" << std::endl; 
            stacks->getListByPos(2)->getList()->executeStackOperation(1, "READ RESPONSE");
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



