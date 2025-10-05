#include <iostream>
#include <vector>
#include <chrono>
#include <memory>
#include <cstring>

// A class that manages a large resource (simulating expensive operations)
class HeavyResource {
private:
    size_t size_;
    int* data_;

public:
    // Constructor
    explicit HeavyResource(size_t size = 1000000) : size_(size) {
        data_ = new int[size_];
        // Simulate expensive initialization
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = static_cast<int>(i);
        }
        std::cout << "  [Constructor] Created resource with " << size_ << " elements\n";
    }

    // Copy constructor (EXPENSIVE)
    HeavyResource(const HeavyResource& other) : size_(other.size_) {
        data_ = new int[size_];
        std::memcpy(data_, other.data_, size_ * sizeof(int));
        std::cout << "  [Copy Constructor] Copied " << size_ << " elements (EXPENSIVE!)\n";
    }

    // Move constructor (CHEAP)
    HeavyResource(HeavyResource&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        std::cout << "  [Move Constructor] Moved resource (CHEAP!)\n";
    }

    // Copy assignment operator
    HeavyResource& operator=(const HeavyResource& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new int[size_];
            std::memcpy(data_, other.data_, size_ * sizeof(int));
            std::cout << "  [Copy Assignment] Copied " << size_ << " elements (EXPENSIVE!)\n";
        }
        return *this;
    }

    // Move assignment operator
    HeavyResource& operator=(HeavyResource&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
            std::cout << "  [Move Assignment] Moved resource (CHEAP!)\n";
        }
        return *this;
    }

    // Destructor
    ~HeavyResource() {
        delete[] data_;
    }

    size_t getSize() const { return size_; }
};

// Generic function template that accepts by value (can use move semantics)
template<typename T>
void processByValue(T obj) {
    std::cout << "  Processing object...\n";
}

// Generic function template that accepts by rvalue reference (move)
template<typename T>
void processByMove(T&& obj) {
    T local = std::move(obj);
    std::cout << "  Processing moved object...\n";
}

// Generic function template that accepts by const reference (no copy)
template<typename T>
void processByConstRef(const T& obj) {
    std::cout << "  Processing by const reference (no copy)...\n";
}

// Helper function to measure time
template<typename Func>
auto measureTime(const std::string& label, Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "  TIME: " << duration.count() << " microseconds\n";
    return duration;
}

int main() {
    std::cout << "=== DEMONSTRATION: Move vs Copy Semantics ===\n\n";
    constexpr size_t LARGE_RESOURCE_SIZE = 100000000;

    // Test 1: Copy construction
    std::cout << "1. COPY CONSTRUCTION (Expensive):\n";
    auto copyTime = measureTime("Copy", []() {
        HeavyResource original(LARGE_RESOURCE_SIZE);
        HeavyResource copy = original;  // Copy constructor called
    });
    std::cout << "\n";

    // Test 2: Move construction
    std::cout << "2. MOVE CONSTRUCTION (Cheap):\n";
    auto moveTime = measureTime("Move", []() {
        HeavyResource original(LARGE_RESOURCE_SIZE);
        HeavyResource moved = std::move(original);  // Move constructor called
    });
    std::cout << "\n";

    // Test 3: Vector operations - push_back with copy
    std::cout << "3. VECTOR PUSH_BACK - Copy (Expensive):\n";
    auto vectorCopyTime = measureTime("Vector Copy", []() {
        std::vector<HeavyResource> vec;
        HeavyResource resource(LARGE_RESOURCE_SIZE/2);
        vec.push_back(resource);  // Copy
    });
    std::cout << "\n";

    // Test 4: Vector operations - push_back with move
    std::cout << "4. VECTOR PUSH_BACK - Move (Cheap):\n";
    auto vectorMoveTime = measureTime("Vector Move", []() {
        std::vector<HeavyResource> vec;
        HeavyResource resource(LARGE_RESOURCE_SIZE/2);
        vec.push_back(std::move(resource));  // Move
    });
    std::cout << "\n";

    // Test 5: Function template with copy
    std::cout << "5. FUNCTION TEMPLATE - Pass by value (Copy):\n";
    auto funcCopyTime = measureTime("Function Copy", []() {
        HeavyResource resource(LARGE_RESOURCE_SIZE/2);
        processByValue(resource);  // Copy into function
    });
    std::cout << "\n";

    // Test 6: Function template with move
    std::cout << "6. FUNCTION TEMPLATE - Pass with move:\n";
    auto funcMoveTime = measureTime("Function Move", []() {
        HeavyResource resource(LARGE_RESOURCE_SIZE/2);
        processByMove(std::move(resource));  // Move into function
    });
    std::cout << "\n";

    // Test 7: Function template with const reference (no copy/move)
    std::cout << "7. FUNCTION TEMPLATE - Pass by const reference:\n";
    auto funcRefTime = measureTime("Function Reference", []() {
        HeavyResource resource(LARGE_RESOURCE_SIZE/2);
        processByConstRef(resource);  // No copy or move
    });
    std::cout << "\n";

    // Summary
    std::cout << "=== PERFORMANCE SUMMARY ===\n";
    std::cout << "Copy construction:        " << copyTime.count() << " µs\n";
    std::cout << "Move construction:        " << moveTime.count() << " µs\n";
    std::cout << "Speedup (move vs copy):   " << (double)copyTime.count() / moveTime.count() << "x faster\n\n";

    std::cout << "Vector copy push_back:    " << vectorCopyTime.count() << " µs\n";
    std::cout << "Vector move push_back:    " << vectorMoveTime.count() << " µs\n";
    std::cout << "Speedup (move vs copy):   " << (double)vectorCopyTime.count() / vectorMoveTime.count() << "x faster\n\n";

    std::cout << "Function copy:            " << funcCopyTime.count() << " µs\n";
    std::cout << "Function move:            " << funcMoveTime.count() << " µs\n";
    std::cout << "Function const ref:       " << funcRefTime.count() << " µs\n";

    return 0;
}
