#include <iostream>
#include <vector>
#include <thread>
#include "ProcessorController.h"


int main() {
    std::cout << " \n\n\n\nPROGRAM EXECUTION\n\n\n\n" << std::endl;
    
    std::vector<std::thread>* workers = new std::vector<std::thread>();
    ProcessorController* controller = new ProcessorController(*workers);


    controller->interconnectBus->schedulingPolicy = 0; // 0 = FIFO , 1 = QoS
    
    for(int i = 1; i < 11; i++) {
        controller->step(i);
    }


    // CRITICAL INSTRUCTION
    controller->interconnectBus->join();

    for (auto& t : *workers) {
        if (t.joinable()) { // Checa si el thread se puede esperar
            t.join();       // Espera a que termine
        }
    }
    
    return 0;
}
