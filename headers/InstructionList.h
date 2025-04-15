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

    InstructionList();

        Instruction *head; 
        Instruction *last;

        std::atomic<int> size; 
        std::mutex listMutex; 
        
        void addInstr(std::string &instr);
        void popInstr(); 
        void showStack();

        char* executeStackOperation(int op, std::string instr);
        InstructionList* copy();
};




#endif