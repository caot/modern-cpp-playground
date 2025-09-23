#include <iostream>
#include <vector>

// Function overloads
void f(int* p) {
    std::cout << "Pointer overload called\n";
}

void f(int x) {
    std::cout << "Int overload called\n";
}

int main() {
    // --- nullptr ---
    f(nullptr);  // Calls the pointer overload (unambiguous)

    // --- auto ---
    auto x = 42;           // deduces int
    auto y = 3.14;         // deduces double
    auto z = "Hello";      // deduces const char*

    std::cout << "x=" << x << ", y=" << y << ", z=" << z << "\n";

    auto str = std::string("World"); // std::string

    std::cout << "z=" << z << ", str=" << typeid(str).name() << "\n";


    // --- range-for ---
    std::vector<int> nums = {1, 2, 3, 4, 5};

    // Iterate by value
    for (auto n : nums) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // Iterate by reference (modifies elements)
    for (auto& n : nums) {
        n *= 2;

        std::cout << n << " ";
    }

    // Print again
    for (auto n : nums) {
        std::cout << n << " ";
    }

    std::cout << "\n";
}

/*
 * It's a small but complete C++ example showing nullptr, auto, and range-for (all introduced in C++11).
 */
