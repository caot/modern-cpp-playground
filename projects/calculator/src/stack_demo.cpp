/*
$ g++ -g stack_demo.cpp stack_trace.cpp -o stack_demo

$ gdb ./stack_demo
(gdb) catch throw
(gdb) run
(gdb) bt
#0  0x00007ffff7cad7c2 in __cxxabiv1::__cxa_throw (obj=0x418340,
    tinfo=0x404dc0 <typeinfo for std::runtime_error@GLIBCXX_3.4>,
    dest=0x402130 <std::runtime_error::~runtime_error()@plt>) at ../../../../libstdc++-v3/libsupc++/eh_throw.cc:78
#1  0x00000000004023ed in cause_error (numerator=10, denominator=0) at stack_demo.cpp:27
#2  0x000000000040245d in middle_call (a=10, b=0) at stack_demo.cpp:36
#3  0x00000000004024ce in main_logic () at stack_demo.cpp:44
#4  0x0000000000402517 in main () at stack_demo.cpp:51
(gdb)
 */

#include "stack_trace.cpp"
#include <iostream>
#include <stdexcept>
#include <cmath>

// 1. Function that causes the error
double cause_error(double numerator, double denominator) {
    if (denominator == 0.0) {
        // Intentionally throw an exception to show the call stack leading here
        throw std::runtime_error("Attempted to divide by zero.");
    }
    return numerator / denominator;
}

// 2. Middle function in the call stack
void middle_call(double a, double b) {
    std::cout << "Starting middle_call..." << std::endl;
    // Call the function that throws
    double result = cause_error(a, b);
    std::cout << "Result: " << result << std::endl; // This line won't be reached
}

// 3. Top-level function
void main_logic() {
    std::cout << "Starting main_logic..." << std::endl;
    // Set up the values that cause the exception
    middle_call(10.0, 0.0);
    std::cout << "Finished main_logic." << std::endl;
}

int main() {
    std::cout << "--- Stack Trace Demo ---" << std::endl;
    try {
        main_logic();
    } catch (const std::exception& e) {
        std::cerr << "\nCaught Exception: " << e.what() << std::endl;

        // In a real application, you would call your print_stack_trace() here.
        // For a GDB demo, we will use the debugger's built-in printout.

        std::cerr << "To see the full stack trace, please run this executable using GDB/LLDB." << std::endl;
        std::cerr << "Command: gdb ./stack_demo" << std::endl;

        print_stack_trace(); // Call the stack trace function on error.
    }
    return 0;
}
