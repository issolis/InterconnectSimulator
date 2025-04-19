#include "Interconnect.h"
#include <iostream>
#include <cstring>

Interconnect::Interconnect(InstructionList& stack, InstructionList& writeCacheStack, List& stacks,  List& cacheReadList) {
    this->stack = &stack; 
    this->stacks = &stacks; 
    this->cacheReadList = &cacheReadList;
    this->writeCacheStack = &writeCacheStack;
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
        
        std::string strInstr(instr);
        std::string writeInstr = strInstr.substr(0, 9); 
        std::string readInstr = strInstr.substr(0, 8); 
        std::string broadcastInv = strInstr.substr(0,20); 
        
        if (writeInstr == "WRITE_MEM"){
            int comas[3] = {-1, -1, -1};
            int count = 0;

            for (int i = 0; i < strInstr.length(); i++){
                if (instr[i] == ',' && count < 3){
                    comas[count] = i;
                    count++;
                }
            }
            
            std::string src       = strInstr.substr(10, comas[0] - 10);
            std::string address   = strInstr.substr(comas[0] + 1, comas[1] - comas[0] - 1);
            std::string data      = strInstr.substr(comas[1] + 1, comas[2] - comas[1] - 1);
            std::string QoS       = strInstr.substr(comas[2] + 1);

            
            if (src == "0"){
                stacks->getListByPos(0)->getList()->executeStackOperation(1, "WRITE RESPONSE");
            }
            else if (src == "1"){
                stacks->getListByPos(1)->getList()->executeStackOperation(1, "WRITE RESPONSE");
            }
            else if (src == "2"){
                stacks->getListByPos(2)->getList()->executeStackOperation(1, "WRITE RESPONSE");
            }
            else if (src == "3"){
                stacks->getListByPos(3)->getList()->executeStackOperation(1, "WRITE RESPONSE");
            }
            else if (src == "4"){
                stacks->getListByPos(4)->getList()->executeStackOperation(1, "WRITE RESPONSE");
            }
            else if (src == "5"){
                stacks->getListByPos(5)->getList()->executeStackOperation(1, "WRITE RESPONSE");
            }
            else if (src == "6"){
                stacks->getListByPos(6)->getList()->executeStackOperation(1, "WRITE RESPONSE");
            }
            else if (src == "7"){
                stacks->getListByPos(7)->getList()->executeStackOperation(1, "WRITE RESPONSE");
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            
        }
        else if(readInstr == "READ_MEM"){
            int comas[3] = {-1, -1, -1};
            int count = 0;
            
            for (int i = 0; i < strInstr.length(); i++){
                if (instr[i] == ',' && count < 3){
                    comas[count] = i;
                    count++;
                }
            }

            std::string src       = strInstr.substr(9, comas[0]  - 9);
            std::string address   = strInstr.substr(comas[0] + 1, comas[1] - comas[0] - 1);
            std::string size      = strInstr.substr(comas[1] + 1, comas[2] - comas[1] - 1);
            std::string QoS       = strInstr.substr(comas[2] + 1);

            if (src == "0"){
                stacks->getListByPos(0)->getList()->executeStackOperation(1, "READ RESPONSE");
            }
            else if (src == "1"){
                stacks->getListByPos(1)->getList()->executeStackOperation(1, "READ RESPONSE");
            }            else if (src == "2"){
                stacks->getListByPos(2)->getList()->executeStackOperation(1, "READ RESPONSE");
            }            else if (src == "3"){
                stacks->getListByPos(3)->getList()->executeStackOperation(1, "READ RESPONSE");
            }            else if (src == "4"){
                stacks->getListByPos(4)->getList()->executeStackOperation(1, "READ RESPONSE");
            }            else if (src == "5"){
                stacks->getListByPos(5)->getList()->executeStackOperation(1, "READ RESPONSE");
            }            else if (src == "6"){
                stacks->getListByPos(6)->getList()->executeStackOperation(1, "READ RESPONSE");
            }            else if (src == "7"){
                stacks->getListByPos(7)->getList()->executeStackOperation(1, "READ RESPONSE");
            }        
        }
        else if (broadcastInv == "BROADCAST_INVALIDATE"){
            int comas[3] = {-1, -1};
            int count = 0;
            
            for (int i = 0; i < strInstr.length(); i++){
                if (instr[i] == ',' && count < 2){
                    comas[count] = i;
                    count++;
                }
            }

            std::string src       = strInstr.substr(21, comas[0]  - 21);
            std::string cacheLine   = strInstr.substr(comas[0] + 1, comas[1] - comas[0] - 1);
            std::string QoS       = strInstr.substr(comas[1] + 1);
 
            for (int i = 0; i < 8; i++){
                cacheReadList->getListByPos(i)->getList()->executeStackOperation(1, "INVALIDATE RESPONSE");
            }
            
            while (writeCacheStack->size.load() != 8){
                std::cout << "Waiting for all caches to invalidate" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));    
            }

            for(int i = 0; i < 8; i++){
                writeCacheStack->executeStackOperation(2, "NOINSTR");
            }

            if (src == "0"){
                stacks->getListByPos(0)->getList()->executeStackOperation(1, "INV_COMPLETE 0," + QoS);
            }
            else if (src == "1"){
                stacks->getListByPos(1)->getList()->executeStackOperation(1, "INV_COMPLETE 1," + QoS);
            }            else if (src == "2"){
                stacks->getListByPos(2)->getList()->executeStackOperation(1, "INV_COMPLETE 2," + QoS);
            }            else if (src == "3"){
                stacks->getListByPos(3)->getList()->executeStackOperation(1, "INV_COMPLETE 3," + QoS);
            }            else if (src == "4"){
                stacks->getListByPos(4)->getList()->executeStackOperation(1, "INV_COMPLETE 4," + QoS);
            }            else if (src == "5"){
                stacks->getListByPos(5)->getList()->executeStackOperation(1, "INV_COMPLETE 5," + QoS);
            }            else if (src == "6"){
                stacks->getListByPos(6)->getList()->executeStackOperation(1, "INV_COMPLETE 6," + QoS);
            }            else if (src == "7"){
                stacks->getListByPos(7)->getList()->executeStackOperation(1, "INV_COMPLETE 7," + QoS);
            }
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



