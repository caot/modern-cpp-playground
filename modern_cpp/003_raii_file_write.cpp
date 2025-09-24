#include <cstdio>
#include <stdexcept>
#include <iostream>

class FileRAII {
    FILE* file_ptr;

public:
    FileRAII(const char* name, const char* mode) {
        file_ptr = std::fopen(name, mode);
        if (!file_ptr) {
            throw std::runtime_error("Could not open file.");
        }
    }

    ~FileRAII() {
        if (file_ptr) {
            std::fclose(file_ptr);
        }
    }

    // Disable copy to ensure only one object manages the resource
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator=(const FileRAII&) = delete;

    // Member function to use the resource
    void write(const char *data) const {
        if (file_ptr) {
            std::fputs(data, file_ptr);
        }
    }
};

void write_file_with_raii() {
    try {
        FileRAII myfile("out.txt", "w");
        myfile.write("RAII saves the day - 02!");
    } catch (const std::exception& e) {
        // Exception caught here
    }
    // The destructor for 'myfile' is called automatically,
    // ensuring the file is closed.
}

int main(int argc, char* argv[]) {
    std::cout << "=== MODERN C++ FEATURES DEMONSTRATION ===" << std::endl;

    write_file_with_raii();

    std::cout << "\n=== DEMONSTRATION COMPLETE ===" << std::endl;
}


// RAII (Resource Acquisition Is Initialization) is a C++ programming idiom that ties the lifetime of a resource to the lifetime of an object. The core principle is that a resource is acquired in an object's constructor and released in its destructor. This ensures that the resource is properly managed, even in the presence of exceptions or early returns.
// Key principles of RAII:
//
//     Resource Acquisition in Constructor:
//
// The object's constructor is responsible for acquiring the resource (e.g., opening a file, allocating memory, locking a mutex). If the acquisition fails, the constructor should throw an exception.
// Resource Release in Destructor:
// The object's destructor is responsible for releasing the acquired resource. C++ guarantees that destructors are called automatically when an object goes out of scope, whether due to normal execution or an exception being thrown.
// Ownership:
//
//     The RAII object takes ownership of the resource, meaning it is solely responsible for its management.
//
// Benefits of RAII:
//
//     Automatic Resource Management:
//     Eliminates the need for manual resource cleanup, reducing the likelihood of memory leaks or other resource-related bugs.
//     Exception Safety:
//     Guarantees that resources are always released, even if exceptions are thrown during the execution of the code.
//     Improved Readability and Maintainability:
//     Encapsulates resource management logic within the class, making the code cleaner and easier to understand.
//     Clear Ownership:
//     Clearly defines which object is responsible for managing a particular resource.
//
// Examples of RAII in C++:
//
//     Smart Pointers:
//
// std::unique_ptr, std::shared_ptr, and std::weak_ptr are prime examples of RAII for memory management. They automatically deallocate memory when the smart pointer object goes out of scope.
// File I/O:
// Classes that manage file handles, where the constructor opens the file and the destructor closes it.
// Mutexes:
// std::lock_guard and std::unique_lock use RAII to ensure mutexes are properly locked and unlocked, even in the face of exceptions.