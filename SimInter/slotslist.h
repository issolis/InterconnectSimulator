#ifndef SLOTSLIST_H
#define SLOTSLIST_H
#include "slotinvalidated.h"
#include "memorychange.h"
#include <QDebug>
class SlotsList
{
private:
    int length = 0;
    SlotInvalidated * head = nullptr;

    int changesMade = 0;
    MemoryChange * headMC = nullptr;

    SlotsList * next = nullptr;
    SlotsList * previous = nullptr;

public:
    SlotsList();
    ~SlotsList();
    int getLength(){return length;}
    void addSlot(int block);
    SlotInvalidated * getSlotByIndex(int index);
    SlotsList * getNext(){return next;}
    void setNext(SlotsList * slot){next = slot;}
    SlotsList * getPrevious(){return previous;}
    void setPrevious(SlotsList * slot){previous = slot;}
    bool isBlockPresent(int block);
    void print();

    //MemoryChanges
    int getChanges(){return changesMade;}
    void addChange(int block, uint32_t value);
    bool hasBlockBeenUpdated(int block);
    MemoryChange * findChangeByBlock(int block);
    MemoryChange * getChangeByIndex(int index);

};

#endif // SLOTSLIST_H
