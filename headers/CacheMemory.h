#ifndef CACHE_MEMORY_H 
#define CACHE_MEMORY_H 
#include <cstdint>
#include <string>
#include <iostream>
#include <array>


class CacheMemory{
    public: 
        CacheMemory(); 

        uint16_t cache[128] = {0}; 
        std::array<std::string, 128> cacheState = {};

        void writeMem(int address, uint16_t value);
        void changeMemState(int address, std::string state);
        void showMem(); 
        
}; 

#endif