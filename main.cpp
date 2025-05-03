#include <iostream>
#include <vector>
#include <thread>
#include "ProcessorController.h"

void pseudoMain() {
    

}

int main() {
    std::cout << " \n\n\n\nPROGRAM EXECUTION \n\n\n\n" << std::endl;
    
    std::vector<std::thread>* workers = new std::vector<std::thread>();
    ProcessorController* controller = new ProcessorController(*workers);


    controller->interconnectBus->schedulingPolicy = 0; 
    
    for(int i = 1; i < 10; i++) {
        controller->step(i);
    }

    //controller->closeExecution();

    

    

    //controller->completeExecution();

    // CRITICAL INSTRUCTION
    controller->interconnectBus->join();

    for (auto& t : *workers) {
        if (t.joinable()) { // Checa si el thread se puede esperar
            t.join();       // Espera a que termine
        }
    }
    
    /*
    std::string instrucciones [10] = {
        "BROADCAST_INVALIDATE 0,1,16",
        "WRITE_MEM 0,0,2,0,0",
        "READ_MEM 0,0,32,0",
        "READ_MEM 0,0,32,0",
        "BROADCAST_INVALIDATE 0,1,10",
        "WRITE_MEM 0,0,2,0,0",
        "READ_MEM 0,0,32,11",
        "READ_MEM 0,0,32,0",
        "READ_MEM 0,0,32,0",
        "READ_MEM 0,0,32,0"
    }; 
    InstructionList *list = new InstructionList();
    for (int i = 0; i < 10; i++) {
        list->executeStackOperation(5, instrucciones[i]);   
    }
    list->executeStackOperation(6, "NOINSTR");
    list->executeStackOperation(6, "NOINSTR");
    list->showStack();
    */
    
    return 0;
}
