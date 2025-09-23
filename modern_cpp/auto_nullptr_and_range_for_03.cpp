#include <iostream>
#include <vector>
#include <string>

// A function that takes a pointer to an integer.
void process_pointer(int* ptr) {
    // We can use nullptr to check if the pointer is null.
    if (ptr != nullptr) {
        std::cout << "The pointer is not null. It points to the value: " << *ptr << std::endl;
    } else {
        std::cout << "The pointer is null." << std::endl;
    }
}

int main() {
    // --- Demonstrating `auto` ---
    // The compiler deduces 'name' is a std::string.
    auto name = "John Doe";
    // The compiler deduces 'age' is an int.
    auto age = 30;
    // The compiler deduces 'pi' is a double.
    auto pi = 3.14159;

    std::cout << "--- Using `auto` for type deduction ---" << std::endl;
    std::cout << "Name (type deduced as std::string): " << name << std::endl;
    std::cout << "Age (type deduced as int): " << age << std::endl;
    std::cout << "Pi (type deduced as double): " << pi << std::endl;
    std::cout << std::endl;

    // --- Demonstrating `nullptr` ---
    int my_number = 10;
    int* valid_pointer = &my_number;
    int* null_pointer = nullptr; // Initialize a null pointer using nullptr.

    std::cout << "--- Using `nullptr` for null pointer initialization and check ---" << std::endl;
    // Calling the function with a valid pointer.
    process_pointer(valid_pointer);
    // Calling the function with a null pointer, which is correctly identified.
    process_pointer(null_pointer);
    std::cout << std::endl;

    // --- Demonstrating range-for loop ---
    // A vector of integers to iterate over.
    std::vector<int> numbers = {10, 20, 30, 40, 50};
    std::cout << "--- Using a range-for loop to iterate through a vector ---" << std::endl;

    // A simple range-for loop to print each element.
    std::cout << "Original numbers: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Another range-for loop to modify each element (by reference).
    std::cout << "Modified numbers (multiplied by 2): ";
    for (auto& num : numbers) { // Using 'auto' with a reference to modify the original vector.
        num *= 2;
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}


/*

* To provide a comprehensive C++ code example for nullptr, auto, and the range-for loop, here is a single program that demonstrates all three concepts.

nullptr

Before C++11, a null pointer was represented by NULL, which was often a macro defined as 0 or (void*)0. This could lead to ambiguity and errors, such as a function call being overloaded for an integer type, causing the NULL to be interpreted as a number instead of a null pointer. The nullptr keyword was introduced as a distinct type that can only be converted to a pointer type, eliminating this ambiguity.

auto

The auto keyword allows the compiler to automatically deduce the type of a variable from its initializer. This can simplify code, especially when dealing with complex template types, iterators, or lambda functions. It helps to reduce redundancy and makes the code cleaner.

Range-for loop

The range-for loop, introduced in C++11, provides a concise way to iterate over the elements of a container (like an array or a std::vector) without explicitly managing iterators or indices. It makes the code more readable and less error-prone.


* How the Code Works

    auto: In the main function, the name, age, and pi variables are declared using auto. The compiler infers their types as const char*, int, and double respectively, based on their initial values. In the second range-for loop, auto& num is used to deduce the type of each element (int) and get a reference to it, allowing for in-place modification of the vector's contents.

    nullptr: The null_pointer is explicitly initialized with nullptr. The process_pointer function demonstrates how ptr != nullptr is the correct and safe way to check if a pointer is valid, avoiding the issues associated with using NULL.

    Range-for loop: The first range-for loop for (int num : numbers) iterates over each element in the numbers vector and copies it into the num variable. The second loop, for (auto& num : numbers), uses a reference & to iterate over the elements directly, which is more efficient for large objects and is necessary if you need to modify the elements within the loop.
 */
