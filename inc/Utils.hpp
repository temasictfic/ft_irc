#pragma once
#include <iostream>
#include <vector>

std::string ToLowercase(const std::string &Names);
bool InvalidPassword(const std::string &Password);
bool InvalidLetter(const std::string &Nick);
bool InvalidPrefix(const std::string &Nick);
std::vector<std::string > split(const std::string &s, const std::string &delimiter);