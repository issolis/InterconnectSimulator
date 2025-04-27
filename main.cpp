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

    for(int i = 0; i < 10; i++) {
        controller->step(i);
    }

    //controller->completeExecution();


   

    

    // CRITICAL INSTRUCTION
    controller->interconnectBus->join();

    for (auto& t : *workers) {
        if (t.joinable()) { // Checa si el thread se puede esperar
            t.join();       // Espera a que termine
        }
    }
    return 0;
}
