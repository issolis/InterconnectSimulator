#include "../../Processor/headers/InstructionList.h"
#include <cstring>  

InstructionList::InstructionList()
{
    last = nullptr; 
    head = nullptr; 
    size = 0; 
}

InstructionList::~InstructionList(){
    delete head;
    delete last;
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
    } else if (op == 5){
        addIntrIntStack(instr);
        return "s";
    } else if (op == 6){
        Instruction* instr = getByPriority();
        char* buffer = new char[instr->getInstr().size() + 1];  // +1 para el '\0'
        std::strcpy(buffer, instr->getInstr().c_str());
        deleteInstr(instr);
        return buffer;
    }else{
        showStack();
        return "s";
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

void InstructionList::addIntrIntStack(std::string &instr){
    Instruction *newInstr = new Instruction(instr);
    newInstr->QoS = getQoS(instr);
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
    //std::cout << "------Stack------" << std::endl;
    Instruction *currentInstr = head; 
    while (currentInstr != nullptr){
        std::cout << currentInstr->getInstr() << std::endl;
        currentInstr = currentInstr->getNextInstr(); 
    }
   // std::cout<<size<<std::endl; 
}

void InstructionList::deleteInstr(Instruction *instr){
    if (instr == head){
        head = head->getNextInstr();
        delete instr;
    }else{
        Instruction *currentInstr = head;
        while (currentInstr != nullptr){
            if (currentInstr->getNextInstr() == instr){
                currentInstr->setNextInstr(instr->getNextInstr());
                delete instr;
                break;
            }
            currentInstr = currentInstr->getNextInstr();
        }
    }
    size.fetch_sub(1, std::memory_order_relaxed);
}

int InstructionList::getQoS(std::string instr){
    int pos = 0;
    for (int i = instr.length() - 1; i >= 0; i--){
        if (instr[i] == ','){
            pos = i;
            break;
        }
    }

    std::string QoS = instr.substr(pos + 1, instr.length() - pos - 1);
    int result = 0;
    for(int i = QoS.length() - 1; i >= 0; i--){
        if (QoS[i] == '0')
            result +=  0;
        else if (QoS[i] == '1')
            result += 1 * pow(10, QoS.length() - i - 1);
        else if (QoS[i] == '2')
            result += 2 * pow(10, QoS.length() - i - 1);
        else if (QoS[i] == '3')
            result += 3 * pow(10, QoS.length() - i - 1);
        else if (QoS[i] == '4')
            result += 4 * pow(10, QoS.length() - i - 1);
        else if (QoS[i] == '5')
            result += 5 * pow(10, QoS.length() - i - 1);
        else if (QoS[i] == '6')
            result += 6 * pow(10, QoS.length() - i - 1);
        else if (QoS[i] == '7')
            result += 7 * pow(10, QoS.length() - i - 1);
        else if (QoS[i] == '8')
            result += 8 * pow(10, QoS.length() - i - 1);
        else if (QoS[i] == '9')
            result += 9 * pow(10, QoS.length() - i - 1);
    }
    return result;
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

Instruction * InstructionList::getInstruction(int index){
    Instruction * curr = this->head;
    if(index < size){
        int i = 0;
        while(i < index){
            curr = curr->getNextInstr();
            i++;
        }
        return curr;
    }else{
        return nullptr;
    }
}

Instruction* InstructionList::getByPriority(){
    Instruction *currentInstr = head;
    Instruction *maxQoSInstr = head;
    while (currentInstr != nullptr){
        if (currentInstr->QoS > maxQoSInstr->QoS){
            maxQoSInstr = currentInstr;
        }
        // std::cout << "currentInstr: " << currentInstr->getInstr() << std::endl;
        currentInstr = currentInstr->getNextInstr();

    }
    // std::cout << "maxQoSInstr: " << maxQoSInstr->getInstr() << std::endl;

    return maxQoSInstr;
}


