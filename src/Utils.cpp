#include "../inc/Utils.hpp"

std::string ToLowercase(const std::string& Names)
{
    std::string new_str(Names);
    for (std::string::iterator it = new_str.begin(); it != new_str.end(); it++)
    {
        tolower(*it);
    }
    return new_str;
}

bool InvalidPassword(const std::string &Password)
{
    if (Password.size() < 4 && Password.size() > 8)
        return true;
    for (size_t i = 0; i < Password.size() - 1; i++)
    {
        if (!isalnum(Password[i]))
            return true;
    }
    return false;
}

bool InvalidLetter(const std::string &Nick)
{
    if(Nick.empty())
        return false;
    std::string forbid = " .,*?!@";
    for (size_t i = 0; i < 7; i++)
    {
        if (Nick.find(forbid[i]) != std::string::npos)
            return true;
    }
    return false;
}

bool InvalidPrefix(const std::string &Nick)
{
    if(Nick.empty())
        return false;
    std::string prefixforbid = "$:#&/";
    for (int i = 0; i < 5; i++)
    {
        if (Nick[0] == prefixforbid[i] || isdigit(Nick[0]))
            return true;
    }
    return false;
}

std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string > res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}
