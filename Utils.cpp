#include "Utils.hpp"

std::string ToLowercase(std::string& Names)
{
    size_t i = 0;
    for (std::string::iterator it = Names.begin(); it != Names.end(); it++)
    {
        tolower(*it);
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

bool InvalidPassword(const std::string &Password)
{
    if (Password.size() < 4 && Password.size() > 8)
    {
        return true;
        // Password size hatası?
    }
    for (size_t i = 0; i < Password.size() - 1; i++)
    {
        if (!isalpha(Password[i]))
        {
            // password invalid char hatası?
            return true;
        }
    }
    return false;
}

bool InvalidLetter(const std::string &Nick)
{
    std::string forbid = " .,*?!@";
    for (size_t i = 0; i < 4; i++)
    {
        if (Nick.find(forbid[i]) != std::string::npos)
            return true;
    }
    return false;
}
bool InvalidPrefix(const std::string &Nick)
{
    std::string prefixforbid = "$:#&/";
    for (int i = 0; i < 4; i++)
    {
        if (Nick[0] == prefixforbid[i] || isdigit(Nick[0]))
            return true;
    }
    return false;
}
