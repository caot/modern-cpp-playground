#include <iostream>
#include <memory> // Include for std::unique_ptr, a standard RAII smart pointer

// RAII Class: SmartArray
// Manages a dynamically allocated integer array.
class SmartArray {
private:
    int* array_ptr;
    size_t size;

public:
    // Constructor: Acquires the resource (memory)
    // The resource is acquired in the constructor, which is called on object creation.
    explicit SmartArray(size_t s) : size(s) {  // The `explicit` keyword prevents implicit conversions.
        std::cout << "SmartArray constructor: Allocating " << size << " integers." << std::endl;
        array_ptr = new int[size];
        for (size_t i = 0; i < size; ++i) {
            array_ptr[i] = i * 10;
        }
    }

    // Destructor: Releases the resource (memory)
    // The destructor is automatically called when the object goes out of scope,
    // ensuring the memory is deallocated.
    ~SmartArray() {
        std::cout << "SmartArray destructor: Deallocating memory." << std::endl;
        delete[] array_ptr;
        array_ptr = nullptr; // Good practice to nullify the pointer.
    }

    // Member function to access the element
    int& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds.");
        }
        return array_ptr[index];
    }

    // Disable copy constructor and copy assignment to prevent double-free errors.
    // Modern C++ practice is to use = delete.
    SmartArray(const SmartArray&) = delete;
    SmartArray& operator=(const SmartArray&) = delete;
};

// Function demonstrating RAII in action
void raii_example_function() {
    std::cout << "--- Starting raii_example_function() ---" << std::endl;

    // An object of the RAII class is created on the stack.
    // This acquires the memory resource.
    try {
        SmartArray my_array(5);

        // Use the array
        std::cout << "Accessing my_array[2]: " << my_array[2] << std::endl;

        // Simulate an error or an early return.
        // Even if an exception is thrown here, the destructor will be called.
        // my_array[10] = 99; // This would throw an exception.

        std::cout << "--- Function is about to end. ---" << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }

    // The `my_array` object goes out of scope here.
    // Its destructor is automatically called, releasing the memory.
    std::cout << "--- raii_example_function() finished. The object is out of scope. ---" << std::endl;
}

// Modern C++ best practice: Using std::unique_ptr for RAII
void modern_raii_example() {
    std::cout << "\n--- Starting modern_raii_example() with std::unique_ptr ---" << std::endl;

    // std::unique_ptr is a standard library smart pointer that provides RAII.
    // It automatically manages the lifetime of a dynamically allocated object.
    std::unique_ptr<int[]> unique_array = std::make_unique<int[]>(3);

    unique_array[0] = 100;
    unique_array[1] = 200;
    unique_array[2] = 300;

    std::cout << "Accessing unique_array[1]: " << unique_array[1] << std::endl;

    // The unique_ptr automatically deallocates the memory when it goes out of scope.
    std::cout << "--- The unique_ptr is about to go out of scope. ---" << std::endl;

    // No explicit `delete` is needed.
}

int main() {
    raii_example_function();
    modern_raii_example();

    return 0;
}

/*
Explanation

    SmartArray Class: This custom class encapsulates a raw pointer to a dynamically allocated array. The constructor (SmartArray(size_t s)) acquires the resource by calling new int[size]. The destructor (~SmartArray()) ensures the resource is released by calling delete[] array_ptr.

    RAII in Action: In raii_example_function(), the my_array object is created on the stack. When the function finishes, regardless of whether it completes normally or exits early due to an exception (the try...catch block), the my_array object's destructor is guaranteed to be called. This automatically frees the allocated memory, preventing memory leaks.

    Modern C++: While creating custom RAII classes is instructive, the C++ Standard Library provides a robust solution with smart pointers like std::unique_ptr and std::shared_ptr. The modern_raii_example() function shows how std::unique_ptr offers a simpler, safer, and more idiomatic way to manage dynamically allocated memory. It automatically handles cleanup, eliminating the need for manual delete calls.
 */
