#include <iostream>
#include <vector>
#include "ProcessorController.h"



int main() {
    
    std::cout<< " \n\n\n\nPROGRAM EXECUTION \n\n\n\n" << std::endl;
    
    std::vector<std::thread>* workers = new std::vector<std::thread>();

    ProcessorController *controller = new ProcessorController(*workers);
   
    for (auto& t : *workers) {
        t.join();
    }
   
    return 0; 

}