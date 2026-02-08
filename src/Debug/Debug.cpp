#include "Debug.h"
void Debug::Log(const std::string &message)
{
    #ifdef ENABLE_LOG
    std::cout << "[Log] " << message << std::endl;
    #endif
}
void Debug::Error(const std::string &message)
{
    std::cerr << "\033[31m[Error] " << message << "\033[0m"  << std::endl;
}
void Debug::Info(const std::string &message)
{
    std::cout << "\033[34m[Info] " << message << "\033[0m" << std::endl;
}