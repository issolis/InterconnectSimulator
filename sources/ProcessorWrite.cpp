#include "ProcessorWrite.h"
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>


ProcessorWrite::ProcessorWrite(InstructionList &stack, std::vector<std::thread>& workers, CacheMemory &cacheMemory, std::string& fileName, int id) {
    this->stack = &stack;
    this->workers = &workers; 
    instrMem = new InstructionMemory(fileName); 
    this->cacheMemory = &cacheMemory; 
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

void ProcessorWrite::processorThreadFunction(std::string instr) {
    thread_context ctx;

    while (!ctx.committed) {
        switch (ctx.current_state) {
            case state::READ:
                ctx.start_size = stack->size.load(std::memory_order_acquire);
                ctx.current_state = state::EXECUTE;
                break;
            case state::EXECUTE:
                //std::this_thread::sleep_for(std::chrono::milliseconds(200));
                ctx.current_state = state::VALIDATE;
                break;
            case state::VALIDATE:
                if (stack->size.load() == ctx.start_size) {
                    std::cout << instr << "  (SENDING) --- FROM P" << id << std::endl;
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

void ProcessorWrite::processorThread(std::string instr) {
    workers->emplace_back(&ProcessorWrite::processorThreadFunction, this, instr);
}

void ProcessorWrite::sendOneInstruction(){
    if (instrMem->head != nullptr){
        std::string instr = instrMem->head->getInstr();
        instrMem->popInstr();
        instr = manipulateInstruction(instr); 
        processorThread(instr); 
    }
}

std::string ProcessorWrite::manipulateInstruction(std::string &instr){
    std::string writeInstr = instr.substr(0, 9); 
    std::string readInstr = instr.substr(0,8);

    if (writeInstr == "WRITE_MEM"){
        int comas[4] = {-1, -1, -1, -1};
        int count = 0;

        for (int i = 0; i < instr.length(); i++){
            if (instr[i] == ',' && count < 4){
                comas[count] = i;
                count++;
            }
        }

        std::string src       = instr.substr(10, comas[0] - 10);
        std::string address   = instr.substr(comas[0] + 1, comas[1] - comas[0] - 1);
        std::string numLines  = instr.substr(comas[1] + 1, comas[2] - comas[1] - 1);
        std::string startLine = instr.substr(comas[2] + 1, comas[3] - comas[2] - 1);
        std::string QoS       = instr.substr(comas[3] + 1);
        
        
        int lines = std::stoi(numLines);
        int start = std::stoi(startLine);
        std::string data = cacheMemory->getData(lines, start);
        std::string newInstr = "WRITE_MEM " + src + "," + address + "," + data + "," + QoS;

        return newInstr; 
    } else if (readInstr == "READ_MEM"){

    } 
    return instr; 
}


