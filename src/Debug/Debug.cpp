#include "Debug.h"

void Debug::Log(const std::string &message)
{
    std::cout << message << std::endl;
}
void Debug::Error(const std::string &message)
{
    std::cerr << "\033[31mError: " << message << "\033[0m" << std::endl;
}