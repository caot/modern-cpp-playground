/*
 * In C++, “printing the type of a variable” isn’t as direct as in Python (type(x)) because type information is
 * mostly a compile-time concept. But there are some common tricks
 *
 * typeid(expr).name() gives a compiler-specific mangled name.
 * On GCC/Clang: "i" for int, "d" for double, etc.
 * On MSVC: it often prints human-readable names.
 */

#include <iostream>
#include <typeinfo>
#include <cxxabi.h>
#include <memory>
#include <boost/type_index.hpp>  // dnf install boost-devel

// Use typeid (with RTTI)
void print_type_by_typeid() {
    int x = 42;
    std::cout << typeid(x).name() << "\n";

    auto y = 42.5;
    std::cout << typeid(y).name() << "\n";

    auto z = "42.5";
    std::cout << typeid(z).name() << "\n";

    auto s = std::string("World"); // std::string
    std::cout << typeid(s).name() << "\n";
}

// To get demangled names on GCC/Clang
template <typename T>
std::string type_name() {
    int status;
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status),
        std::free
    };
    return (status == 0) ? res.get() : typeid(T).name();
}


void get_demangled_names() {
    double y = 3.14;
    std::cout << type_name<decltype(y)>() << "\n";  // prints "double"
}

// At compile time (debugging with templates)
template <typename T>
void print_type(T&&) {}

void print_type_at_compile_time() {
    auto x = 42;
    print_type(x);
}


// Use helper libraries
using boost::typeindex::type_id_with_cvr;

void print_type_by_helper_libraries() {
    int x = 5;
    std::cout << type_id_with_cvr<decltype(x)>().pretty_name() << "\n";
}


int main() {
    std::cout << "Use typeid (with RTTI)" << "\n" << std::endl;
    print_type_by_typeid();

    std::cout << "\nTo get demangled names on GCC/Clang:" << "\n" << std::endl;
    get_demangled_names();

    std::cout << "\nAt compile time (debugging with templates)" << "\n" << std::endl;
    print_type_at_compile_time();

    std::cout << "\nUse helper libraries" << "\n" << std::endl;
    print_type_by_helper_libraries();

    std::cout << "\n=== DEMONSTRATION COMPLETE ===" << std::endl;
    return 0;
}
