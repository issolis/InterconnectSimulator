#include "ProcessorController.h"

ProcessorController::ProcessorController(std::vector<std::thread> &workers) {
    this->workers = &workers;

    writeStack = new InstructionList(); 
    writeCacheStack = new InstructionList();  
    responsesStack = new InstructionList();
    requestStack = new InstructionList();

    readStackList = new List(); 
    readCacheStackList = new List();

    for (int i = 0; i < 8; i++) {
        readStackList->insertList(i); 
        readCacheStackList->insertList(i);
    }

    interconnectBus = new Interconnect(*writeStack, *writeCacheStack, *readStackList, *readCacheStackList);
    
    for (int i = 0; i < 8; i++) {
        processors.emplace_back(
            *readStackList->getListByPos(i)->getList(), 
            *writeStack, 
            *readCacheStackList->getListByPos(i)->getList(), 
            *writeCacheStack, 
            workers, 
            paths[i], 
            i, 
            *responsesStack, 
            *requestStack
        );
    }

    for (int i = 0; i < 8; i++) {
        processors[i].processorRead->processorThread(); 
        processors[i].processorCache->processorThread();
    }

    step();
    
}

void ProcessorController::step(){
    for (int i = 0; i < 8; i++) {
        processors[i].processorWrite->sendOneInstruction(); 
    }

    while(responsesStack->size.load() != 8) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "Step finished" << std::endl;
    responsesStack->showStack();


}
