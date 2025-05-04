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
        void addIntrIntStack(std::string &instr);
        void popInstr(); 
        void showStack();
        void deleteInstr(Instruction* instr);
        
        Instruction* getByPriority(); 

        int getQoS(std::string instr);


        char* executeStackOperation(int op, std::string instr);
        InstructionList* copy();
};




#endif