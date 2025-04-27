#include "ProcessorCache.h"
#include <cstring>

ProcessorCache::ProcessorCache(InstructionList &readCacheStack, InstructionList &writeCacheStack, std::vector<std::thread> &workers,  CacheMemory &cacheMemory, int id){
    this->readCacheStack = &readCacheStack;
    this->writeCacheStack = &writeCacheStack;
    this->workers = &workers;
    this->cacheMemory = &cacheMemory;
    this->id = id;
    isRunning = true;
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

    while (!ctx.committed && isRunning) {
        switch (ctx.current_state) {
            case state::READ:
                ctx.start_size = readCacheStack->size.load(std::memory_order_acquire);
                ctx.current_state = state::EXECUTE;
                break;
            case state::EXECUTE:
                //std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ctx.current_state = state::VALIDATE;
                break;
            case state::VALIDATE:
                if (readCacheStack->size.load() == ctx.start_size) {
                    if (strcmp(readCacheStack->executeStackOperation(3, "NOINSTR"), "notnull") == 0) {
                        char* instr = readCacheStack->executeStackOperation(4, "NOINSTR"); 
                        readCacheStack->executeStackOperation(2, "NOINSTR"); 
                        std::string strInstr(instr);
                        if (strInstr.substr(0, 10) == "INVALIDATE"){
                            std::string cacheLine = strInstr.substr(11);
                            

                            int address = 0; 
                            for (int i = 0; i < 128; i++){
                                if (cacheLine == std::to_string(i)){
                                    address = i;
                                    cacheMemory->changeMemState(i, "INVALID");
                                    break;
                                }
                            }

                            
                            std::string response = "ICK_ACK " + std::to_string(id) + ", 0" ;    
                            //std::cout << response << " (SENDING) --- FROM P" << id << std::endl;
                            processorWriteThread(response);
                            
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
                //std::this_thread::sleep_for(std::chrono::milliseconds(200));
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
    workers->emplace_back([this, instr]() {
        this->processorWriteThreadFunction(instr);
    });
}

