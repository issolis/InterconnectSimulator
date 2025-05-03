#ifndef SLOTSLIST_H
#define SLOTSLIST_H
#include "slotinvalidated.h"
#include <QDebug>
class SlotsList
{
private:
    int length = 0;
    SlotInvalidated * head = nullptr;
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
};

#endif // SLOTSLIST_H
