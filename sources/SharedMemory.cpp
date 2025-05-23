#include "SharedMemory.h"
#include <string>
#include <cmath>

int SharedMemory::fromStrToInt(std::string str){
    int num = 0;
    int len = str.length();
    int vec[len] = {0};
    for (int i = 0; i < len; i++){
        if (str.substr(i, 1) == "1") vec[i] = 1;
        else if (str.substr(i, 1) == "2") vec[i] = 2;
        else if (str.substr(i, 1) == "3") vec[i] = 3;
        else if (str.substr(i, 1) == "4") vec[i] = 4;
        else if (str.substr(i, 1) == "5") vec[i] = 5;
        else if (str.substr(i, 1) == "6") vec[i] = 6;
        else if (str.substr(i, 1) == "7") vec[i] = 7;
        else if (str.substr(i, 1) == "8") vec[i] = 8;
        else if (str.substr(i, 1) == "9") vec[i] = 9;
        else if (str.substr(i, 1) == "0") vec[i] = 0;
    }

    for (int i = 0; i < len; i++){
        num += vec[i] * pow(10, len - i - 1);
    }

    return num;
    
}

uint32_t SharedMemory::setSharedMemory(std::string address, std::string value)
{
    int len = value.length();
    uint32_t values[len/5] = {0};
    int addressInt = 0;

    for (int i = 0; i < len; i = i + 5){
        values[i/5] = fromStrToInt(value.substr(i, 5));
    }
  

    for (int i = 0; i < 4096; i++){
        if (std::to_string(i) == address){
            addressInt = i;
            break;  
        }
    }
    int addressSpaces = len/10 + len%2;
    int j = 0; 
    for (int i = 0; i < addressSpaces; i++){
        if (i == addressSpaces - 1) {
            if (len%2 == 0){
                uint32_t val = values[j] << 16 ;
                val += values[j + 1];
                (*sharedMemory)[addressInt + j/2] = val;
            }else{
                (*sharedMemory)[addressInt + j/2] = values[len/5 - 1];               
            }
            break;
        }
        
        uint32_t val = values[j] << 16 ;
        val += values[j + 1];
        (*sharedMemory)[addressInt + j/2] = val;     
        j+= 2;  
    }  

    return  (*sharedMemory)[addressInt + j/2]; 
}

std::string SharedMemory::getSharedMemory(std::string address, std::string size){
    int addressInt = 0;
    int sizeInt = 0;
    for (int i = 0; i < 4096; i++){
        if (std::to_string(i) == address){
            addressInt = i;
            break;  
        }
    }
    for (int i = 0; i <= 32; i++){
        if (std::to_string(i) == size){
            sizeInt = i; 
            break;  
        }
    }

    return std::to_string((*sharedMemory)[addressInt] >> (32 - sizeInt) );
}

SharedMemory::SharedMemory()
{
    sharedMemory = new std::vector<uint32_t>(4096, 0);
}