#ifndef STEPCHANGES_H
#define STEPCHANGES_H
#include "slotslist.h"
#include <QDebug>
class StepChanges
{
private:
    SlotsList * head = nullptr;
    SlotsList * currentStep = nullptr;
    int * length = new int(0);
    int position = 0;

    //Slots invalidados
    SlotsList * markedBlocks = new SlotsList();

    //Cambios en Memoria Compartida
    SlotsList * memoryBlocksUpdated = new SlotsList();

public:
    StepChanges();
    ~StepChanges();
    int getLength(){return *length;}
    void addStep(SlotsList * slotList);
    bool blockIsMarked(int block);
    SlotsList * getCurrent(){return currentStep;}
    SlotsList * moveRight();
    SlotsList * moveLeft();
    SlotsList * returnHome();
    SlotsList * getMarkedBlocksList(){return markedBlocks;}

    SlotsList * getMemoryBlocksUpdated(){return memoryBlocksUpdated;}
};

#endif // STEPCHANGES_H
