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


void Interconnect::interconectSendingFunction(std::string instr) {
    thread_context ctx;

    while (!ctx.committed) {
        switch (ctx.current_state) {
            case state::READ:
                ctx.start_size = stack->size.load(std::memory_order_acquire);
                ctx.current_state = state::EXECUTE;
                break;
            case state::EXECUTE:
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                ctx.current_state = state::VALIDATE;
                break;
            case state::VALIDATE:
                if (stack->size.load() == ctx.start_size) {
                    stack->executeStackOperation(1, instr);
                    ctx.current_state = state::COMMIT;
                } else {
                    ctx.current_state = state::RETRY;
                }
                break;
            case state::COMMIT:
                ctx.committed = true;
                break;
            case state::RETRY:
                ctx.current_state = state::READ;
                break;
        }
    }
}

void Interconnect::processorThread(std::string instr) {
    responseThreads->emplace_back(&Interconnect::interconectSendingFunction, this, instr);
}

void Interconnect::receiveMessage( ){ 
    if (strcmp(stack->executeStackOperation(3, "NOINSTR"), "notnull") == 0) {
        char* instr = stack->executeStackOperation(4, "NOINSTR"); 
        stack->executeStackOperation(2, "NOINSTR"); 


        if (strcmp(instr, "WRITE") == 0) {
            std:: cout << "EXECUTING WRITE" << std::endl; 
            responseStack->executeStackOperation(1, "WRITE RESPONSE"); 
            responseStack->showStack(); 

        }
        if (strcmp(instr, "READ") == 0) {
            std:: cout << "EXECUTING READ" << std::endl; 
            responseStack->executeStackOperation(1, "READ RESPONSE"); 
            responseStack->showStack(); 
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



