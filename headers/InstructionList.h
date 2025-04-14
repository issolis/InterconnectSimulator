#ifndef INSTRUCTIONLIST_H
#define INSTRUCTIONLIST_H

#include "Instruction.h"
#include <iostream>
#include <atomic>
#include <mutex>
class InstructionList
{
    private:
        
        
    
    public:
        Instruction *head; 
        Instruction *last;
        std::atomic<int> size; 
        std::mutex listMutex; 
        InstructionList();
        char* executeStackOperation(int op, std::string instr);
        void addInstr(std::string &instr);
        void popInstr(); 
        void showStack();
        InstructionList* copy();
};




#endif