#include "InstructionList.h"


InstructionList::InstructionList()
{
    last = nullptr; 
    head = nullptr; 
    size = 0; 
}


void InstructionList::executeStackOperation(int op, std::string instr){
    std::lock_guard<std::mutex> lock(listMutex); 
    if (op == 1){
        addInstr(instr);
    }else if (op == 2){
        popInstr(); 
    }
}
void InstructionList::addInstr(std::string &instr)
{   
    Instruction *newInstr = new Instruction(instr); 
    if (head == nullptr){
        head = newInstr; 
        last = newInstr;        
    }else{
        Instruction *currentInstr = head; 
        while(currentInstr->getNextInstr() != nullptr){
            currentInstr = currentInstr->getNextInstr();
        }
        currentInstr->setNextInstr(newInstr); 
        last = currentInstr->getNextInstr(); 
    }
    size.fetch_add(1, std::memory_order_relaxed); 
}

void InstructionList::popInstr()
{
    if(head == nullptr){
        return;
    } else if (head->getNextInstr() == nullptr){
        head = nullptr;
        
    } else {

        Instruction *newHead = head->getNextInstr(); 
        delete head; 
        head = newHead;
      
    }
    size.fetch_sub(1, std::memory_order_relaxed); 
}

void InstructionList::showStack(){
    std::cout << "------Stack------" << std::endl;
    Instruction *currentInstr = head; 
    while (currentInstr != nullptr){
        std::cout << currentInstr->getInstr() << std::endl;
        currentInstr = currentInstr->getNextInstr(); 
    }
    std::cout<<size<<std::endl; 
}

InstructionList* InstructionList::copy() {
    InstructionList* copiedList = new InstructionList();
    
    Instruction* current = this->head;
    while (current != nullptr) {
        std::string instr = current->getInstr();
        copiedList->addInstr(instr);  
        current = current->getNextInstr();
    }

    return copiedList;
}

