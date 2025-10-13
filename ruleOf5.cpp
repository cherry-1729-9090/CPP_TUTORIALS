#include <iostream>
#include <cstring>
using namespace std;

class MyString {
private:
    char* data;
    size_t size;

public:
    // Regular Constructor
    MyString(const char* str = "") {
        size = strlen(str);
        data = new char[size + 1];
        strcpy(data, str);
        cout << "Constructor: Created \"" << data << "\"" << endl;
    }

    // 1. DESTRUCTOR
    ~MyString() {
        cout << "Destructor: Deleting \"" << (data ? data : "null") << "\"" << endl;
        delete[] data;
    }

    // 2. COPY CONSTRUCTOR
    MyString(const MyString& other) {
        size = other.size;
        data = new char[size + 1];
        strcpy(data, other.data);
        cout << "Copy Constructor: Copied \"" << data << "\"" << endl;
    }

    // 3. COPY ASSIGNMENT OPERATOR
    MyString& operator=(const MyString& other) {
        cout << "Copy Assignment: ";
        
        if (this != &other) {  // Self-assignment check
            // Clean up existing resource
            delete[] data;
            
            // Copy from other
            size = other.size;
            data = new char[size + 1];
            strcpy(data, other.data);
            cout << "Assigned \"" << data << "\"";
        } else {
            cout << "Self-assignment detected";
        }
        cout << endl;
        return *this;
    }

    // 4. MOVE CONSTRUCTOR (C++11)
    MyString(MyString&& other) noexcept {
        // "Steal" the resource from other
        data = other.data;
        size = other.size;
        
        // Leave other in valid but empty state
        other.data = nullptr;
        other.size = 0;
        
        cout << "Move Constructor: Moved \"" << (data ? data : "null") << "\"" << endl;
    }

    // 5. MOVE ASSIGNMENT OPERATOR (C++11)
    MyString& operator=(MyString&& other) noexcept {
        cout << "Move Assignment: ";
        
        if (this != &other) {
            // Clean up existing resource
            delete[] data;
            
            // "Steal" from other
            data = other.data;
            size = other.size;
            
            // Leave other in valid state
            other.data = nullptr;
            other.size = 0;
            
            cout << "Moved \"" << (data ? data : "null") << "\"";
        } else {
            cout << "Self-move detected";
        }
        cout << endl;
        return *this;
    }

    // Utility functions
    const char* c_str() const { return data ? data : ""; }
    size_t length() const { return size; }
    
    void print() const {
        cout << "String: \"" << c_str() << "\" (size: " << size << ")" << endl;
    }
};

// Helper function to create temporary objects
MyString createString(const char* str) {
    cout << "Creating temporary string..." << endl;
    return MyString(str);
}

int main() {
    cout << "=== RULE OF 5 DEMONSTRATION ===" << endl;
    
    cout << "\n1. Regular Construction:" << endl;
    MyString s1("Hello");
    s1.print();
    
    cout << "\n2. Copy Constructor:" << endl;
    MyString s2 = s1;  // Copy constructor
    s2.print();
    
    cout << "\n3. Copy Assignment:" << endl;
    MyString s3("World");
    s3.print();
    s3 = s1;  // Copy assignment
    s3.print();
    
    cout << "\n4. Move Constructor:" << endl;
    MyString s4 = createString("Temporary");  // Move constructor (from temporary)
    s4.print();
    
    cout << "\n5. Move Assignment:" << endl;
    MyString s5("Initial");
    s5.print();
    s5 = createString("Moved");  // Move assignment (from temporary)
    s5.print();
    
    cout << "\n6. Self-assignment test:" << endl;
    s5 = s5;  // Self-assignment
    
    cout << "\n=== All objects about to be destroyed ===" << endl;
    return 0;
}


/*
* Rule of 5
- The Rule of 5 is a fundamental guideline in modern C++ that states: If you need to define any one of 
the following 5 special member functions, you probably need to define all of them.
*/
