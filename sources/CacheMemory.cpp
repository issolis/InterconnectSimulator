#include "CacheMemory.h"
#include <iostream>
#include <random>

CacheMemory::CacheMemory(){
    for (int i = 0; i < 128; i++){
        cacheState[i] = "Validate";
    }
}

void CacheMemory::writeMem(int address, uint16_t  value){
    if (address < 128) cache[address] = value; 
    else std::cout << "Address out of bounds" << std::endl;  
}

void CacheMemory::changeMemState(int address, std::string state){
    if (address < 128) cacheState[address] = state; 
    else std::cout << "Address out of bounds" << std::endl;  
}

void CacheMemory::showMem() {
    for (int i = 0; i < 128; i += 4) {
        std::cout << i << "    ";
        std::cout << cache[i]         << " : " << cacheState[i] << "    ";
        std::cout << cache[i + 1]     << " : " << cacheState[i + 1] << "    ";
        std::cout << cache[i + 2]     << " : " << cacheState[i + 2] << "    ";
        std::cout << cache[i + 3]     << " : " << cacheState[i + 3] << "    ";
        std::cout << std::endl;
    }
}

std::string CacheMemory::getData(int numLines, int startLine)
{
    std::string data = ""; 
    for (int i = 0; i < numLines; i++) {
        if (cache[i] == 0) {
            std::random_device rd;                     
            std::mt19937 gen(rd());                     
            std::uniform_int_distribution<> distrib(1, 65535);
            cache[i] = distrib(gen);
        }
        for (int j = 0; j < 5 - std::to_string(cache[i]).length(); j++)
            data += '0'; 
        data += std::to_string(cache[i]);
    }
    return data;
}

