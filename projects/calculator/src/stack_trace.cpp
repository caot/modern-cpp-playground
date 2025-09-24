// --- Platform-Specific Stack Trace Implementation ---

#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#include <tchar.h>

// Link with Dbghelp.lib
#pragma comment(lib, "Dbghelp.lib")

void print_stack_trace() {
    std::cerr << "--- Stack Trace (Windows) ---" << std::endl;
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);

    void* stack[100];
    USHORT frames = CaptureStackBackTrace(0, 100, stack, NULL);

    SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(TCHAR), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    for (USHORT i = 0; i < frames; i++) {
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        std::cerr << "  " << i << ": " << symbol->Name << std::endl;
    }

    free(symbol);
    SymCleanup(process);
}

#else // Linux and macOS
#include <execinfo.h>
#include <cxxabi.h>
// #include <stdlib.h>
#include <iostream>

inline void print_stack_trace() {
    std::cerr << "--- Stack Trace (Linux/macOS) ---" << std::endl;
    const int MAX_FRAMES = 128;
    void* callstack[MAX_FRAMES];
    int frames = backtrace(callstack, MAX_FRAMES);
    char** symbols = backtrace_symbols(callstack, frames);

    if (symbols) {
        for (int i = 0; i < frames; ++i) {
            std::string symbol_name = symbols[i];
            size_t start = symbol_name.find('(');
            size_t end = symbol_name.find('+');

            if (start != std::string::npos && end != std::string::npos) {
                std::string mangled_name = symbol_name.substr(start + 1, end - start - 1);
                int status;
                char* demangled_name = abi::__cxa_demangle(mangled_name.c_str(), NULL, NULL, &status);
                if (status == 0) {
                    std::cerr << "  " << i << ": " << demangled_name << std::endl;
                    free(demangled_name);
                } else {
                    std::cerr << "  " << i << ": " << symbols[i] << std::endl;
                }
            } else {
                std::cerr << "  " << i << ": " << symbols[i] << std::endl;
            }
        }
        free(symbols);
    }
}
#endif
