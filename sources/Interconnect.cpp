#include "Interconnect.h"
#include <iostream>
#include <cstring>

Interconnect::Interconnect(InstructionList& stack, InstructionList& writeCacheStack, List& readStackList,  List& cacheReadList) {
    this->stack = &stack; 
    this->readStackList = &readStackList; 
    this->cacheReadList = &cacheReadList;
    this->writeCacheStack = &writeCacheStack;
    this->sharedMemory = new SharedMemory();
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



            std::cout << "Data written to address " << address << ": " << data << std::endl;
            sharedMemory->setSharedMemory(address, data);

            std::string dataResp = "WRITE_RESP " + src + ", 0x1" + ", " + QoS;

            for (int i = 0; i < 8; i++){
                if (src == std::to_string(i)){
                    readStackList->getListByPos(i)->getList()->executeStackOperation(1, dataResp); 
                    break;
                }
            }

           // std::this_thread::sleep_for(std::chrono::milliseconds(300));
            
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

            std::string data = sharedMemory->getSharedMemory(address, size);
            std::cout << "Data read from address " << address << ": " << data << std::endl;
            std::string dataResp = "READ_RESP " + src +  ", " + data + ", " + QoS;

            for (int i = 0; i < 8; i++){
                if (src == std::to_string(i)){
                    readStackList->getListByPos(i)->getList()->executeStackOperation(1,  dataResp); 
                    break;
                }
            }

           // std::this_thread::sleep_for(std::chrono::milliseconds(300));

                
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
                cacheReadList->getListByPos(i)->getList()->executeStackOperation(1, "INVALIDATE " + cacheLine);
            }
            
            while (writeCacheStack->size.load() != 8){
                std::cout << "Waiting for all caches to invalidate" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));    
            }

            for(int i = 0; i < 8; i++){
                writeCacheStack->executeStackOperation(2, "NOINSTR");
            }


            for (int i = 0; i < 8; i++){
                if (src == std::to_string(i)){
                    readStackList->getListByPos(i)->getList()->executeStackOperation(1, "INV_COMPLETE " + src + ", " + QoS);
                    break;
                }
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



