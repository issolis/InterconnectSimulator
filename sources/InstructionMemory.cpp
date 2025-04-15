#include "InstructionMemory.h"
#include <fstream>



InstructionMemory::InstructionMemory(std::string &initializerFileName)
{
    head = nullptr; 
    nextInstruction = nullptr; 

    std::ifstream file(initializerFileName);
    std::string instr;
    while (std::getline(file, instr)) addInstruction(instr); 
    file.close();
}

void InstructionMemory::addInstruction(std::string instr)
{
    Instruction *newInstruction = new Instruction(instr);
    
    if (head == nullptr)
        head = newInstruction; 
    else{
        Instruction *currentInstruction = head; 
        while(currentInstruction->getNextInstr()!=nullptr) currentInstruction = currentInstruction->getNextInstr(); 
        currentInstruction->setNextInstr(newInstruction); 
    }
}

void InstructionMemory::popInstr()
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
}
void InstructionMemory::showInstructionMemory(){
    Instruction *currentInstruction = head; 
    std::cout << "__________INSTRUCTION MEMORY___________"<< std::endl; 
    while(currentInstruction!=nullptr) {
        std::cout << currentInstruction->getInstr()<< std::endl;
        currentInstruction = currentInstruction->getNextInstr(); 
    }

}
