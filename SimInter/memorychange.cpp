#include "memorychange.h"

MemoryChange::MemoryChange() {}

MemoryChange::MemoryChange(int block, uint32_t value){
    *blockUpdated = block;
    *newValue = value;
}

MemoryChange::~MemoryChange(){
    delete blockUpdated;
    delete newValue;
}
