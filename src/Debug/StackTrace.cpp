#include "StackTrace.h"
#include "Debug.h"
#include <iostream>

void StackTrace::Initialize()
{
    SetUnhandledExceptionFilter(ExceptionFilter);
    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
    SymInitialize(GetCurrentProcess(), nullptr, TRUE);
}

LONG WINAPI StackTrace::ExceptionFilter(EXCEPTION_POINTERS* exceptionInfo)
{
    std::string stackTrace = GetStackTrace(exceptionInfo);
    PrintStackTrace(stackTrace);
    
    return EXCEPTION_EXECUTE_HANDLER;
}

std::string StackTrace::GetStackTrace(EXCEPTION_POINTERS* exceptionInfo)
{
    std::stringstream ss;
    
    ss << "========================================\n";
    ss << "CRASH DETECTED!\n";
    ss << "========================================\n";
    ss << "Exception Code: " << GetExceptionCode(exceptionInfo->ExceptionRecord->ExceptionCode) << "\n";
    ss << "Exception Address: 0x" << std::hex << exceptionInfo->ExceptionRecord->ExceptionAddress << std::dec << "\n";
    ss << "========================================\n";
    ss << "Stack Trace:\n";
    ss << "========================================\n";
    
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    
    CONTEXT context = *exceptionInfo->ContextRecord;
    
    STACKFRAME64 stackFrame = {};
    stackFrame.AddrPC.Offset = context.Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context.Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context.Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
    
    SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char));
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = 255;
    
    IMAGEHLP_LINE64* line = (IMAGEHLP_LINE64*)malloc(sizeof(IMAGEHLP_LINE64));
    line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    
    int frameCount = 0;
    while (StackWalk64(
        IMAGE_FILE_MACHINE_AMD64,
        process,
        thread,
        &stackFrame,
        &context,
        nullptr,
        SymFunctionTableAccess64,
        SymGetModuleBase64,
        nullptr))
    {
        if (frameCount > 50)
        {
            ss << "... (truncated)\n";
            break;
        }
        
        ss << "[" << frameCount << "] ";
        
        DWORD64 address = stackFrame.AddrPC.Offset;
        
        if (SymFromAddr(process, address, nullptr, symbol))
        {
            ss << symbol->Name << " (0x" << std::hex << symbol->Address << std::dec << ")";
            
            if (SymGetLineFromAddr64(process, address, nullptr, line))
            {
                ss << "\n    File: " << line->FileName;
                ss << "\n    Line: " << line->LineNumber;
            }
        }
        else
        {
            ss << "0x" << std::hex << address << std::dec;
        }
        
        ss << "\n";
        frameCount++;
    }
    
    free(symbol);
    free(line);
    
    ss << "========================================\n";
    
    return ss.str();
}

std::string StackTrace::GetExceptionCode(DWORD code)
{
    switch (code)
    {
        case EXCEPTION_ACCESS_VIOLATION:
            return "EXCEPTION_ACCESS_VIOLATION (0xC0000005) - 试图访问无效内存";
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED (0xC000008C) - 数组越界";
        case EXCEPTION_BREAKPOINT:
            return "EXCEPTION_BREAKPOINT (0x80000003) - 断点";
        case EXCEPTION_DATATYPE_MISALIGNMENT:
            return "EXCEPTION_DATATYPE_MISALIGNMENT (0x80000002) - 数据未对齐";
        case EXCEPTION_FLT_DENORMAL_OPERAND:
            return "EXCEPTION_FLT_DENORMAL_OPERAND (0xC000008D) - 浮点数操作数异常";
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            return "EXCEPTION_FLT_DIVIDE_BY_ZERO (0xC000008E) - 浮点数除零";
        case EXCEPTION_FLT_INEXACT_RESULT:
            return "EXCEPTION_FLT_INEXACT_RESULT (0xC000008F) - 浮点数结果不精确";
        case EXCEPTION_FLT_INVALID_OPERATION:
            return "EXCEPTION_FLT_INVALID_OPERATION (0xC0000090) - 浮点数无效操作";
        case EXCEPTION_FLT_OVERFLOW:
            return "EXCEPTION_FLT_OVERFLOW (0xC0000091) - 浮点数溢出";
        case EXCEPTION_FLT_STACK_CHECK:
            return "EXCEPTION_FLT_STACK_CHECK (0xC0000092) - 浮点数栈检查失败";
        case EXCEPTION_FLT_UNDERFLOW:
            return "EXCEPTION_FLT_UNDERFLOW (0xC0000093) - 浮点数下溢";
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            return "EXCEPTION_ILLEGAL_INSTRUCTION (0xC000001D) - 非法指令";
        case EXCEPTION_IN_PAGE_ERROR:
            return "EXCEPTION_IN_PAGE_ERROR (0xC0000006) - 页面错误";
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            return "EXCEPTION_INT_DIVIDE_BY_ZERO (0xC0000094) - 整数除零";
        case EXCEPTION_INT_OVERFLOW:
            return "EXCEPTION_INT_OVERFLOW (0xC0000095) - 整数溢出";
        case EXCEPTION_INVALID_DISPOSITION:
            return "EXCEPTION_INVALID_DISPOSITION (0xC0000026) - 无效的异常处理";
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            return "EXCEPTION_NONCONTINUABLE_EXCEPTION (0xC0000025) - 不可继续的异常";
        case EXCEPTION_PRIV_INSTRUCTION:
            return "EXCEPTION_PRIV_INSTRUCTION (0xC0000096) - 特权指令";
        case EXCEPTION_SINGLE_STEP:
            return "EXCEPTION_SINGLE_STEP (0x80000004) - 单步执行";
        case EXCEPTION_STACK_OVERFLOW:
            return "EXCEPTION_STACK_OVERFLOW (0xC00000FD) - 栈溢出";
        default:
        {
            char buffer[64];
            sprintf_s(buffer, "Unknown Exception (0x%08X)", code);
            return buffer;
        }
    }
}

void StackTrace::PrintStackTrace(const std::string& stackTrace)
{
    Debug::Error(stackTrace);
    
    std::cerr << stackTrace << std::endl;
    
    std::cerr << "Press Enter to exit..." << std::endl;
    std::cin.get();
}

void StackTrace::PrintCurrentStack()
{
    std::stringstream ss;
    ss << "========================================\n";
    ss << "Current Stack Trace:\n";
    ss << "========================================\n";
    
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    
    CONTEXT context;
    RtlCaptureContext(&context);
    
    STACKFRAME64 stackFrame = {};
    stackFrame.AddrPC.Offset = context.Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context.Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context.Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
    
    SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char));
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = 255;
    
    IMAGEHLP_LINE64* line = (IMAGEHLP_LINE64*)malloc(sizeof(IMAGEHLP_LINE64));
    line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    
    int frameCount = 0;
    while (StackWalk64(
        IMAGE_FILE_MACHINE_AMD64,
        process,
        thread,
        &stackFrame,
        &context,
        nullptr,
        SymFunctionTableAccess64,
        SymGetModuleBase64,
        nullptr))
    {
        if (frameCount > 50)
        {
            ss << "... (truncated)\n";
            break;
        }
        
        ss << "[" << frameCount << "] ";
        
        DWORD64 address = stackFrame.AddrPC.Offset;
        
        if (SymFromAddr(process, address, nullptr, symbol))
        {
            ss << symbol->Name << " (0x" << std::hex << symbol->Address << std::dec << ")";
            
            if (SymGetLineFromAddr64(process, address, nullptr, line))
            {
                ss << "\n    File: " << line->FileName;
                ss << "\n    Line: " << line->LineNumber;
            }
        }
        else
        {
            ss << "0x" << std::hex << address << std::dec;
        }
        
        ss << "\n";
        frameCount++;
    }
    
    free(symbol);
    free(line);
    
    ss << "========================================\n";
    
    Debug::Log(ss.str());
}