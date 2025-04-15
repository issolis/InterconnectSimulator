#ifndef INSTRUCTIONMEMORY_H
#define INSTRUCTIONMEMORY_H

#include "Instruction.h"

class InstructionMemory
{
private:
    
public:
    
    InstructionMemory(std::string& initializerFileName);
    void addInstruction(std::string instr); 
    void popInstr(); 
    void showInstructionMemory(); 
    Instruction *head; 
    Instruction *nextInstruction; 
    
};



#endif