#ifndef SLOTINVALIDATED_H
#define SLOTINVALIDATED_H

class SlotInvalidated
{
private:
    int blockInvalidated;
    SlotInvalidated * nextSlot = nullptr;

public:
    SlotInvalidated();
    SlotInvalidated(int block){blockInvalidated = block;};
    void setBlock(int block){blockInvalidated = block;}
    int getBlock(){return blockInvalidated;}
    void setNext(SlotInvalidated * next){nextSlot = next;}
    SlotInvalidated * getNext(){return nextSlot;}
};

#endif // SLOTINVALIDATED_H
