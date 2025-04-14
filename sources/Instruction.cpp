#include "Instruction.h"

Instruction::Instruction(std::string &instr){
    this->instr = instr; 
    next = nullptr; 
}

void Instruction::setNextInstr(Instruction *next){
    this->next = next; 
}


Instruction* Instruction::getNextInstr(){
    return next; 
}

std::string Instruction::getInstr()
{
    
    return instr;
}
