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
        ~InstructionList();
        Instruction *head = nullptr;
        Instruction *last = nullptr;

        std::atomic<int> size; 
        std::mutex listMutex; 
        
        void addInstr(std::string &instr);
        void popInstr(); 
        void showStack();
        Instruction * getInstruction(int index);

        char* executeStackOperation(int op, std::string instr);
        InstructionList* copy();
};




#endif
