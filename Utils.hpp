#pragma once
#include <iostream>
#include <vector>

std::string ToLowercase(const std::string &Names);
std::string ToUppercase(std::string &Names);
bool ValidModeChars(const std::string &ModeString);
bool InvalidPassword(const std::string &Password);
bool InvalidLetter(const std::string &Nick);
bool InvalidPrefix(const std::string &Nick);
std::vector<const std::string & > split(std::string s, std::string delimiter);