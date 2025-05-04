#include "slotslist.h"

SlotsList::SlotsList() {}

SlotsList::~SlotsList(){
    delete head;
    delete headMC;
}

void SlotsList::addSlot(int block){
    SlotInvalidated * newSlot = new SlotInvalidated(block);
    if(head == nullptr){
        head = newSlot;
    }else{
        SlotInvalidated * curr = head;
        while(curr->getNext() != nullptr){
            curr = curr->getNext();
        }
        curr->setNext(newSlot);
    }
    length++;
}

void SlotsList::addChange(int block, uint32_t value){
    MemoryChange * newUpdate = new MemoryChange(block, value);
    //qDebug() << "Pointer: " << newUpdate;
    if(headMC == nullptr){
        headMC = newUpdate;
    }else{
        MemoryChange * curr = headMC;
        while(curr->getNext() != nullptr){
            curr = curr->getNext();
        }
        curr->setNext(newUpdate);
    }
    changesMade++;
}


SlotInvalidated * SlotsList::getSlotByIndex(int index){
    if(index < length){
        if(length > 0){
            SlotInvalidated * curr = head;
            for(int i = 0; i < index; i++){
                curr = curr->getNext();
            }
            return curr;
        }else{
            return nullptr;
        }
    }else{
        return nullptr;
    }
    return nullptr;
}

MemoryChange * SlotsList::getChangeByIndex(int index){
    if(index < changesMade){
        if(changesMade > 0){
            MemoryChange * curr = headMC;
            for(int i = 0; i < index; i++){
                curr = curr->getNext();
            }
            return curr;
        }else{
            return nullptr;
        }
    }else{
        return nullptr;
    }
    return nullptr;
}

bool SlotsList::isBlockPresent(int block){
    if(length > 0){
        SlotInvalidated * curr = head;
        while(curr->getNext() != nullptr){
            if(curr->getBlock() == block){
                return true;
            }else{
                curr = curr->getNext();
            }
        }
        if(curr->getBlock() == block){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
    return false;
}

void SlotsList::print(){
    qDebug() << "+++++++++++++++++++";
    if(length > 0){
        for(int i = 0; i < length; i++){
            qDebug() << "Marked: " << this->getSlotByIndex(i)->getBlock();
        }
    }else{
        qDebug() << "List Empty";
    }
    qDebug() << "--------------------";
}
