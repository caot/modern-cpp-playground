#include <iostream>
#include <vector>
#include <utility> // For std::move

/**
 * @brief A simple class demonstrating Copy and Move semantics.
 * It holds a resource (a dynamically allocated array) to make the difference
 * between copying and moving tangible (move is cheap, copy is expensive).
 */
class ResourceHolder {
private:
    int* data;
    size_t size;

public:
    // Constructor (Initializes the resource)
    ResourceHolder(size_t s) : size(s) {
        data = new int[size];
        std::cout << "[CONSTRUCTOR] Resource created (Size: " << size << ")" << std::endl;
    }

    // Destructor (Cleans up the resource)
    ~ResourceHolder() {
        if (data != nullptr) {
            delete[] data;
            std::cout << "[DESTRUCTOR] Resource deleted." << std::endl;
        } else {
            std::cout << "[DESTRUCTOR] Resource was moved, nothing to delete." << std::endl;
        }
    }

    // 1. Copy Constructor (DEEP COPY - EXPENSIVE)
    // Takes an Lvalue reference (const ResourceHolder& other).
    ResourceHolder(const ResourceHolder& other) : size(other.size) {
        std::cout << "[COPY CONSTRUCTOR] Performing deep copy..." << std::endl;
        data = new int[size];
        // Deep copy: actually copying the contents of the array
        std::copy(other.data, other.data + size, data);
    }

    // 2. Move Constructor (SHALLOW COPY + NULL - CHEAP)
    // Takes an Rvalue reference (ResourceHolder&& other).
    ResourceHolder(ResourceHolder&& other) noexcept
        : data(other.data), size(other.size) {

        std::cout << "[MOVE CONSTRUCTOR] Stealing resource and nulling source." << std::endl;
        // Steal the pointer and set the source's pointer to null
        // This prevents the source's destructor from deleting the resource twice.
        other.data = nullptr;
        other.size = 0;
    }

    // (Rule of Five: Copy/Move Assignment operators omitted for brevity)

    void print_status(const std::string& name) const {
        std::cout << "  - " << name << " status: Data pointer is "
                  << (data != nullptr ? "VALID" : "NULL")
                  << ", Size: " << size << std::endl;
    }
};

/**
 * @brief Generic function template that processes a resource.
 * Because the argument is taken by value (T obj), the copy or move constructor
 * of the ResourceHolder class (T) is invoked upon entry to create the 'obj' variable.
 */
template <typename T>
void process_resource(T obj, const std::string& type) {
    std::cout << "\n--- Inside " << type << " (by value) ---" << std::endl;
    obj.print_status("Internal copy"); // Show the status of the local object
    std::cout << "--- Exiting " << type << " ---" << std::endl;
}

int main() {
    std::cout << "=== Demo: Move vs. Copy Semantics ===\n" << std::endl;

    // --- 1. LVALUE CALL (FORCES COPY) ---
    // 'local_holder' is a named variable (Lvalue).
    // When passed to process_resource, the copy constructor is called.
    std::cout << "--- 1. LVALUE TEST (FORCES COPY) ---" << std::endl;
    ResourceHolder local_holder(1000); // Initial Construction
    local_holder.print_status("local_holder");

    // Call the template function with the Lvalue
    process_resource(local_holder, "Lvalue Call");

    // Check original object after copy
    std::cout << "\nAfter Lvalue Call (Original is safe):" << std::endl;
    local_holder.print_status("local_holder");

    // --- 2. RVALUE CALL (ALLOWS MOVE) ---
    // A temporary object (Rvalue) is created and passed directly.
    // When passed to process_resource, the move constructor is called.
    std::cout << "\n\n--- 2. RVALUE TEST (ALLOWS MOVE) ---" << std::endl;

    // The expression creates a temporary Rvalue, triggering construction + move
    process_resource(ResourceHolder(50), "Rvalue Call");

    // An alternative Rvalue call using std::move to convert an Lvalue to Rvalue
    std::cout << "\n--- 3. STD::MOVE TEST (FORCES MOVE) ---" << std::endl;
    ResourceHolder original_movable(200);
    original_movable.print_status("original_movable");

    // Call the template function with Rvalue reference created by std::move
    process_resource(std::move(original_movable), "std::move Call");

    // Check original object after move - it should be empty/null
    std::cout << "\nAfter std::move Call (Original is stolen):" << std::endl;
    original_movable.print_status("original_movable");

    std::cout << "\n=== Program End (Remaining objects destructed) ===" << std::endl;

    return 0;
}
