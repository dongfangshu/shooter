#include <iostream>
#include <string>

class Debug
{
public:
    static void Log(const std::string &message);
    static void Error(const std::string &message);
};
