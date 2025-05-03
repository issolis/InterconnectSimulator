#ifndef STEPCHANGES_H
#define STEPCHANGES_H
#include "slotslist.h"
#include <QDebug>
class StepChanges
{
private:
    SlotsList * head = nullptr;
    SlotsList * currentStep = nullptr;
    SlotsList * markedBlocks = new SlotsList();
    int * length = new int(0);
    int position = 0;

public:
    StepChanges();
    ~StepChanges();
    int getLength(){return *length;}
    void addStep(SlotsList * slotList);
    bool blockIsMarked(int block);
    void markBlock(int block);
    SlotsList * getCurrent(){return currentStep;}
    SlotsList * moveRight();
    SlotsList * moveLeft();
    SlotsList * returnHome();
    SlotsList * getMarkedBlocksList(){return markedBlocks;}

};

#endif // STEPCHANGES_H
