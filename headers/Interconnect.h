#ifndef INTERCONNECT_H
#define INTERCONNECT_H

#include "InstructionList.h"
#include <iostream>
#include <mutex>
#include <thread>


class Interconnect
{
private:
    

public:
    InstructionList* stack; 
    std::thread monitor; 
    std::atomic<bool> running{false}; 
    Interconnect(InstructionList& stack);
    void receiveMessage();
    void showStack();
    void startMonitoring();
    void join();
};

#endif
