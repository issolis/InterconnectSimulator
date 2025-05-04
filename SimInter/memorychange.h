#ifndef MEMORYCHANGE_H
#define MEMORYCHANGE_H
#include <cstdint>

class MemoryChange
{
private:
    int * blockUpdated = new int(0);
    uint32_t * newValue = new uint32_t();
    MemoryChange * nextChange = nullptr;

public:
    MemoryChange();
    ~MemoryChange();
    MemoryChange(int block, uint32_t value);
    void setBlockUpdated(int block){*blockUpdated = block;}
    int getBlockUpdated(){return *blockUpdated;}
    void setNewValue(uint32_t value){*newValue = value;}
    uint32_t getNewValue(){return *newValue;}
    void setNext(MemoryChange * next){nextChange = next;}
    MemoryChange * getNext(){return nextChange;}

};

#endif // MEMORYCHANGE_H
