#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>

class Instruction
{
    private:
        std::string instr; 
        Instruction *next; 
        
    public:
        Instruction(std::string& instr); 
        void setNextInstr(Instruction* next); 
        Instruction* getNextInstr(); 
        std::string getInstr(); 

};


#endif