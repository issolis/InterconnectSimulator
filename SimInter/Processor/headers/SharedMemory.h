#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <iostream>
#include <vector>
#include <cstdint> 

class SharedMemory {
public:
   
    std::vector<uint32_t> *sharedMemory; 
    int fromStrToInt(std::string str);
    uint32_t setSharedMemory(std::string address, std::string value);
    std::string getSharedMemory(std::string address, std::string size);
    SharedMemory();   
};


#endif
