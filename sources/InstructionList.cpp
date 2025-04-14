#include "InstructionList.h"
#include <cstring>  

InstructionList::InstructionList()
{
    last = nullptr; 
    head = nullptr; 
    size = 0; 
}


char* InstructionList::executeStackOperation(int op, std::string instr){
    std::lock_guard<std::mutex> lock(listMutex); 
    if (op == 1){
        addInstr(instr);
        return "s";
    }else if (op == 2){
        popInstr(); 
        return "s"; 
    }else if (op == 3){
        if (head == nullptr) return "nullptr";
        return "notnull";  
    }else if (op == 4){
        char* buffer = new char[head->getInstr().size() + 1];  // +1 para el '\0'
        std::strcpy(buffer, head->getInstr().c_str());
        return buffer; 
    }

    return "operation not defined";
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

