#include "stepchanges.h"

StepChanges::StepChanges() {}

StepChanges::~StepChanges(){
    delete markedBlocks;
    delete memoryBlocksUpdated;
    delete length;
}

void StepChanges::addStep(SlotsList * slotList){
    if(*length == 0){
        head = slotList;
        currentStep = slotList;
        head->setNext(head);
        head->setPrevious(head);
    }else{
        SlotsList * curr = head;
        while(curr->getNext() != head){
            curr = curr->getNext();
        }
        head->setPrevious(curr);
        curr->setNext(slotList);
        slotList->setPrevious(curr);
        slotList->setNext(head);
    }
    (*length) += 1;
}

SlotsList * StepChanges::moveLeft(){
    if(*length > 0){
        currentStep = currentStep->getPrevious();
        return currentStep;
    }else{
        return nullptr;
    }
}

SlotsList * StepChanges::moveRight(){
    if(*length > 0){
        currentStep = currentStep->getNext();
        return currentStep;
    }else{
        return nullptr;
    }
}

SlotsList * StepChanges::returnHome(){
    currentStep = head;
    return currentStep;
}


