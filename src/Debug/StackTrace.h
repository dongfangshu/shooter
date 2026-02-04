#pragma once
#include <windows.h>
#include <dbghelp.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#pragma comment(lib, "dbghelp.lib")

class StackTrace
{
private:
    static LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* exceptionInfo);
    static std::string GetStackTrace(EXCEPTION_POINTERS* exceptionInfo);
    static std::string GetExceptionCode(DWORD code);
    static void PrintStackTrace(const std::string& stackTrace);
    
public:
    static void Initialize();
    static void PrintCurrentStack();
};