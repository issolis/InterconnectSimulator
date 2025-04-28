
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

    interconnectBus = new Interconnect(*writeStack, *writeCacheStack, *readStackList, *readCacheStackList, *responsesStack);
    
    for (int i = 0; i < 8; i++) {
        processors.emplace_back(
            *readStackList->getListByPos(i)->getList(), 
            *writeStack, 
            *readCacheStackList->getListByPos(i)->getList(), 
            *writeCacheStack, 
            workers, 
            paths[i], 
            i
        );
    }

    for (int i = 0; i < 8; i++) {
        processors[i].processorRead->processorThread(); 
        processors[i].processorCache->processorThread();
    }
   

}

int ProcessorController::step(int step){
  
     
    for (int i = 0; i < 8; i++) {
        processors[i].processorWrite->sendOneInstruction(); 
    }
    
    while(responsesStack->size.load() != 8){
       
    }

     
    std::cout << "____________Responses____________" << step << std::endl;
    responsesStack->showStack();  
    for (int i = 0; i < 8; i++) {
        responsesStack->executeStackOperation(2, "NOINSTR");
    }   
    std::cout << "_________________________________" << std::endl;


    return 1; 

}

void ProcessorController::closeExecution(){
    interconnectBus->running = false;
    for(int i = 0; i < 8; i++){
        processors[i].processorRead->isRunning = false;
        processors[i].processorCache->isRunning = false;
    }
}

int ProcessorController::completeExecution()
{
    for(int i = 0; i < 10; i++)
        for (int j = 0; j < 8; j++)
            processors[j].processorWrite->sendOneInstruction();
    
    while(responsesStack->size.load() != 80 ){

    }

    responsesStack->showStack();
    for (int i = 0; i < 80; i++) {
        responsesStack->executeStackOperation(2, "NOINSTR");
    }

    /*interconnectBus->running = false;
    for(int i = 0; i < 8; i++){
        processors[i].processorRead->isRunning = false;
        processors[i].processorCache->isRunning = false;
    }*/



    return 1;
}
