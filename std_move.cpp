#include <chrono>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
using namespace std;

class Resource {
private:
    string name;
    vector<int> data;

public:
    // Constructor
    Resource(const string& n, int size) : name(n) {
        data.resize(size, 42);
        cout << "Created Resource: " << name << " (size: " << size << ")" << endl;
    }

    // Copy Constructor
    Resource(const Resource& other) : name(other.name + "_copy"), data(other.data) {
        cout << "COPY Constructor: " << name << " (expensive copy of " << data.size() << " elements)" << endl;
    }

    // Move Constructor
    Resource(Resource&& other) noexcept : name(move(other.name)), data(move(other.data)) {
        cout << "MOVE Constructor: " << name << " (cheap move, source now empty)" << endl;
    }

    // Copy Assignment
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            name = other.name + "_assigned";
            data = other.data;
            cout << "COPY Assignment: " << name << " (expensive copy)" << endl;
        }
        return *this;
    }

    // Move Assignment
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            name = move(other.name);
            data = move(other.data);
            cout << "MOVE Assignment: " << name << " (cheap move)" << endl;
        }
        return *this;
    }

    // Destructor
    ~Resource() {
        cout << "Destroyed: " << name << " (had " << data.size() << " elements)" << endl;
    }

    // Utility functions
    void print() const {
        cout << "Resource " << name << " has " << data.size() << " elements" << endl;
    }
};

void performanceTest() {
    const int size = 1000000;
    std::vector<std::vector<int>> source;
    
    // Fill source with large vectors
    for (int i = 0; i < 100; ++i) {
        source.emplace_back(size, i);
    }
    
    // Test COPY
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> copied;
    for (auto& vec : source) {
        copied.push_back(vec);  // Copy
    }
    auto copyTime = std::chrono::high_resolution_clock::now() - start;
    
    // Test MOVE
    start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> moved;
    for (auto& vec : source) {
        moved.push_back(std::move(vec));  // Move
    }
    auto moveTime = std::chrono::high_resolution_clock::now() - start;
    
    std::cout << "Copy time: " << copyTime.count() << " ns" << std::endl;
    std::cout << "Move time: " << moveTime.count() << " ns" << std::endl;
    std::cout << "Move is " << (copyTime.count() / moveTime.count()) << "x faster" << std::endl;
}

int main() {
    cout << "=== STD::MOVE DEMONSTRATION ===" << endl;

    cout << "\n1. Creating original resource:" << endl;
    Resource r1("Original", 100000);
    r1.print();

    cout << "\n2. WITHOUT std::move (COPY):" << endl;
    Resource r2 = r1;  // Copy constructor - expensive!
    r2.print();
    r1.print();  // r1 still has its data

    cout << "\n3. WITH std::move (MOVE):" << endl;
    Resource r3 = move(r1);  // Move constructor - cheap!
    r3.print();
    r1.print();  // r1 is now empty/moved-from

    cout << "\n4. Move assignment:" << endl;
    Resource r4("ToBeReplaced", 10);
    r4.print();
    r4 = move(r2);  // Move assignment
    r4.print();
    r2.print();  // r2 is now empty

    cout << "\n5. Moving into container:" << endl;
    vector<Resource> resources;
    resources.push_back(move(r3));  // Move into vector
    resources.push_back(move(r4));  // Move into vector
    
    cout << "After moving into vector:" << endl;
    r3.print();  // Empty
    r4.print();  // Empty
    
    cout << "\n6. Performance test:" << endl;
    performanceTest();

    cout << "\n=== All objects about to be destroyed ===" << endl;
    return 0;
}

/*
- std::move is a cast, not a move operation
- It enables move semantics by converting lvalue to rvalue reference
    eg :
        std::move

- Use it when transferring ownership or when done with an object
- Moved-from objects are in valid but unspecified state
- Don't use moved-from objects (except for destruction/assignment)
- Move semantics can dramatically improve performance for expensive-to-copy objects
*/