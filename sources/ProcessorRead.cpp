#include "ProcessorRead.h"
#include <cstring>

ProcessorRead::ProcessorRead(InstructionList &readStack, std::vector<std::thread> &workers, int id){
    this->readStack = &readStack;
    this->workers = &workers; 
    this->id = id; 
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

void ProcessorRead::processorThreadFunction() {
    thread_context ctx;

    while (!ctx.committed) {
        switch (ctx.current_state) {
            case state::READ:
                ctx.start_size = readStack->size.load(std::memory_order_acquire);
                ctx.current_state = state::EXECUTE;
                break;
            case state::EXECUTE:
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ctx.current_state = state::VALIDATE;
                break;
            case state::VALIDATE:
                if (readStack->size.load() == ctx.start_size) {
                    if (strcmp(readStack->executeStackOperation(3, "NOINSTR"), "notnull") == 0) {
                        char* instr = readStack->executeStackOperation(4, "NOINSTR"); 
                        readStack->executeStackOperation(2, "NOINSTR"); 
                        if (strcmp(instr, "WRITE RESPONSE") == 0) {
                            std:: cout << "EXECUTED W FROM P" << id << std::endl; 
                        }
                        else if (strcmp(instr, "READ RESPONSE") == 0) {
                            std:: cout << "EXECUTED R FROM P" << id <<  std::endl; 
                        }  
                        else if (strcmp(instr, "INVALIDATE RESPONSE") == 0) {
                            std:: cout << "EXECUTED I FROM P" << id <<  std::endl; 
                        }                
                    }
                   ctx.current_state = state::RETRY;
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

void ProcessorRead::processorThread() {
    workers->emplace_back(&ProcessorRead::processorThreadFunction, this);
}


