#pragma once
#include <iostream>

std::string ToLowercase(std::string Names);
bool ValidModeChars(const std::string &ModeString);
bool InvalidPassword(const std::string &Password);
bool InvalidLetter(const std::string &Nick);
bool InvalidPrefix(const std::string &Nick);