#include <iostream>

using namespace std;

struct DB {
    int id;
    std::string cName;
    char buffer[2*1024*1024];
    
    // DEFAULT CONSTRUCTOR
    DB() {
        this->cName = "DEFAULT CONSTRUCTOR";
        cout << "DEFAULT CONSTRUCTOR" << endl;
    }
    
    // COPY CONSTRUCTOR 
    DB(const DB& other) {
        this->cName = "COPY CONSTRUCTOR";
        cout << "COPY CONSTRUCTOR" << endl;
    }
    
    // COPY ASSIGNMENT
    DB& operator = (const DB& other) {
        cout << "COPY ASSIGNMENT" << endl;
        return *this;
    }
};

struct DB2 {
    int id;
    std::string cName;
    char buffer[2*1024*1024];
    
    // DEFAULT CONSTRUCTOR
    DB2() {
       this->id = 12;
       this->cName = "Something";
       this->buffer = new char[2*1021*1021];
    }
    
    // COPY CONSTRUCTOR 
    DB2(const DB& other) {
        this->id = other.id;
        this->cName = other.cName;
        // this->buffer = other.buffer -> here it doesn't actually create a new memory, it refers to the others' buffer itself
        this->buffer = memccpy(other.buffer, this->buffer, 2*1024*1024);
        // by this it creates a new buffer array and points the buffer of current DB2 to a new buffer but copied values of others' buffer
        return *this;
    }
    
    // COPY ASSIGNMENT
    DB2& operator = (const DB2& other) {
        // the old buffer is from heap, so it should be deleted first
        delete this->buffer
        this = DB2(other);
        return *this;
    }
};


DB getConnection() {
    DB d;
    return d;
}


int main()
{   
    
    DB con = getConnection();
    // O/P : DEFAULT CONSTRUCTOR
    // In here actually, first default constructor is called in the function and then a COPY CONSTRUCTOR 
    // should have been called but modern compilers follow something called `Return Value Optimization`
    


    // DB a;
    // DB b = a;
    // DB c;
    // c = a;
    
    // O/P :
    // DEFAULT CONSTRUCTOR
    // COPY CONSTRUCTOR
    // DEFAULT CONSTRUCTOR
    // COPY ASSIGNMENT
    // c = a -> this is called copy assignment
    
        
    // DB d;
    // DB other = d;
    // cout << d.cName << endl;
    // cout << other.cName << endl;
    
    // d is lvalue here and when other is created from an lvalue so C++ calls the copy constructor
    return 0;
}