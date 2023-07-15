#include "utilits.hpp"

std::string LowercaseAlphabet(std::string Names)
{
    size_t i = 0;
    while(Names[i])
    {
        if(Names[i] >= 'A' && Names[i] <= 'Z')
        {
            Names[i] += 32; 
        }
        i++
    }
}