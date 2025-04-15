#ifndef INTERCONNECT_H
#define INTERCONNECT_H

#include "InstructionList.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <thread>


class Interconnect
{
private:
    

public:

    Interconnect(InstructionList& stack, InstructionList& responseStack, std::vector<std::thread>& responseThreads);
    
    InstructionList* stack; 
    InstructionList* responseStack; 
    std::vector<std::thread>* responseThreads;

    std::thread monitor; 
    std::atomic<bool> running{false}; 
    
    void interconectSendingFunction(std::string instr);
    void processorThread(std::string instr);
    void receiveMessage();
    void showStack();
    void startSnooping();
    void join();
};

#endif
