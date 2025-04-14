#include "Interconnect.h"
#include <iostream>

Interconnect::Interconnect(InstructionList& stack) {
    this->stack = &stack;
    startMonitoring(); 

}

void Interconnect::receiveMessage( ){ 
}
void Interconnect::showStack(){
   

}

void Interconnect::startMonitoring() {
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



