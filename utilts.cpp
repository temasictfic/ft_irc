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
        i++;
    }
}

bool ValidModeChars(const std::string &ModeString)
{
    int count = 0;
    if (ModeString[0] == '+' || ModeString[0] == '-')
    {
        std::string modes = "ikt";
        for (size_t i = 0; i < 3; i++)
        {
            if (ModeString[1] == modes[i])
                count++;
        }
    }
    return count;
}