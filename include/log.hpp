#pragma once
#include <iostream>

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";

inline void PrintError(std::string message)
{
    std::cout << RED << message << RESET << std::endl;
}

inline void PrintWarning(std::string message)
{
    std::cout << YELLOW << message << RESET << std::endl;
}

inline void PrintMessage(std::string message)
{
    std::cout << GREEN << message << RESET << std::endl;
}
