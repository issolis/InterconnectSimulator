#include "ProcessorCache.h"
#include <cstring>

ProcessorCache::ProcessorCache(InstructionList &readCacheStack, InstructionList &writeCacheStack, std::vector<std::thread> &workers, int id){
    this->readCacheStack = &readCacheStack;
    this->writeCacheStack = &writeCacheStack;
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

void ProcessorCache::processorThreadFunction() {
    thread_context ctx;

    while (!ctx.committed) {
        switch (ctx.current_state) {
            case state::READ:
                ctx.start_size = readCacheStack->size.load(std::memory_order_acquire);
                ctx.current_state = state::EXECUTE;
                break;
            case state::EXECUTE:
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ctx.current_state = state::VALIDATE;
                break;
            case state::VALIDATE:
                if (readCacheStack->size.load() == ctx.start_size) {
                    if (strcmp(readCacheStack->executeStackOperation(3, "NOINSTR"), "notnull") == 0) {
                        char* instr = readCacheStack->executeStackOperation(4, "NOINSTR"); 
                        readCacheStack->executeStackOperation(2, "NOINSTR"); 
                        if (strcmp(instr, "INVALIDATE RESPONSE") == 0) {
                            std:: cout << "EXECUTED I FROM P" << id << std::endl; 
                            if(id == 0){
                                processorWriteThread("ICK_ACK 0, 0");
                            }
                            else if(id == 1){
                                processorWriteThread("ICK_ACK 1, 0");
                            }
                            else if(id == 2){
                                processorWriteThread("ICK_ACK 2, 0");
                            }
                            else if(id == 3){
                                processorWriteThread("ICK_ACK 3, 0");
                            }  
                            else if(id == 4){
                                processorWriteThread("ICK_ACK 4, 0");
                            }
                            else if(id == 5){
                                processorWriteThread("ICK_ACK 5, 0");
                            }
                            else if(id == 6){
                                processorWriteThread("ICK_ACK 6, 0");
                            }
                            else if(id == 7){
                                processorWriteThread("ICK_ACK 7, 0");
                            }   
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

void ProcessorCache::processorThread() {
    workers->emplace_back([this]() { this->processorThreadFunction(); });
}

void ProcessorCache::processorWriteThreadFunction(std::string instr) {
    thread_context ctx;

    while (!ctx.committed) {
        switch (ctx.current_state) {
            case state::READ:
                ctx.start_size = writeCacheStack->size.load(std::memory_order_acquire);
                ctx.current_state = state::EXECUTE;
                break;
            case state::EXECUTE:
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                ctx.current_state = state::VALIDATE;
                break;
            case state::VALIDATE:
                if (writeCacheStack->size.load() == ctx.start_size) {
                    writeCacheStack->executeStackOperation(1, instr);
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

void ProcessorCache::processorWriteThread(std::string instr) {
    ([this, instr]() { this->processorWriteThreadFunction(instr); });
}
