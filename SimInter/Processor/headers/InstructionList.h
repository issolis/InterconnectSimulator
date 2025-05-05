#ifndef INSTRUCTIONLIST_H
#define INSTRUCTIONLIST_H

#include "Instruction.h"
#include <atomic>
#include <cmath>
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
        void addIntrIntStack(std::string &instr);//Atender
        void popInstr(); 
        void showStack();
        void deleteInstr(Instruction* instr);
        Instruction * getByPriority();
        int getQoS(std::string instr);

        Instruction * getInstruction(int index);

        char* executeStackOperation(int op, std::string instr);
        InstructionList* copy();
};




#endif
