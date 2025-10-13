#include <iostream>
#include <vector>

using namespace std;

vector<int> getVector (){
    vector<int> a = {1,2,3};
    cout << &a << endl;
    return a;
}


int main()
{
    vector<int> b = getVector();
    cout << &b << endl;
    vector<int> c;
    c = b; // copy assignment
    return 0;
}


/*
In here address of both a and b will be same

Actually what should happen according the previous rule like cpp creates deep copy for everything.
These are the following steps are that should have happend :

Stack memory                                                Heap memory

b is created                                               
    in temporary value space(r value space)
    a is created        ---------------------------------> {1, 2, 3}
getVector() returns an r value
b  getVector creates a new copy    ----------------------> {1, 2, 3}

now after the `;` in b = getVector(), the temporary space should be erased

Bus instead of all these the compiler/CPU optimizes this

c is created
a new copy of {1, 2, 3} is created in heap space
c   ------------------------------------------------------> {1, 2, 3}


The Logical (Unoptimized) Process
Without any optimizations, the C++ standard logically implies a series of expensive copies when returning an object by value.
For the line vector<int> b = getVector();, the naive steps would be:
- Inside getVector: A local vector a is created on the function's stack. This allocates heap memory for the elements {1, 2, 3}.
- On return: A temporary vector object is created, and the contents of a are deep-copied into it (copy constructor).
- In main: The vector b is created by deep-copying the contents of the temporary object (copy constructor).
- End of statement: The temporary object is destroyed.
- This involves two slow, deep copies of the heap data. Modern C++ avoids this entirely.

The C++11/14 Optimization: Move Semantics 
In C++11, move semantics was introduced to solve the problem of expensive copies from temporary objects. Return Value Optimization (RVO) was also a common but optional optimization. If RVO wasn't applied, the compiler would use move semantics instead of copying.
What happens:
- Inside getVector, vector a is created on the stack with its data on the heap. Its stack address is printed.
- getVector() returns. The compiler treats a as a temporary (an "rvalue") because it's about to be destroyed.
- The line vector<int> b = getVector(); is executed. Because b is being initialized from a temporary, C++ calls the std::vector's move constructor.
- The move constructor is fast. Instead of copying the heap data, b steals the pointer to the heap memory from a. a's internal pointer is set to nullptr.
- The temporary object from getVector() is destroyed, which is trivial as it no longer owns any memory.
- The vector object b now exists on main's stack at a different address than a was, but it points to the same heap data a originally created.
Result in C++11/14 (without RVO): The expensive deep copy is avoided, replaced by a fast pointer transfer. The stack addresses of a and b are different.

The C++17+ Optimization: Guaranteed Copy Elision 
What actually happens in C++17 and later:
- The compiler sees that main needs a vector<int> named b and that it will be initialized by the return value of getVector().
- It allocates the memory for b on main's stack frame.
- It calls getVector() but secretly passes the memory address of b. It effectively tells the function, "The object you intend to return... just construct it directly in b's memory location."
- Inside getVector, the variable a is no longer treated as a separate object. It's just a name or an alias for the memory location of b.
- cout << &a; prints the memory address of this object.
- When the function returns, nothing needs to be copied or moved. The object is already constructed in its final destination.
- cout << &b; prints the memory address of the exact same object.
Result in C++17+: There is no copy and no move. The object is constructed in place once. This is the ultimate optimization, and it's why you see the same address.

*/


class Vector {
    int* ptr;
    int size;

    public:
    Vector(int si) {

        cout << "Constructor called" << endl;

        this->size = si;
        this->ptr = new int[size];
    }

    // copy constructor of a vector
    // Creates a NEW object by copying from an existing object
    public:
    Vector(const Vector& v){
    // const -> the actual object shouldn't be modified

        cout << "Cop yConstructor called"<< endl;

        this->size = v.size;
        this->ptr = new int[size];
        for(int i = 0;i < v.size; i++) {
            this->ptr[i] = v.ptr[i];
        }
    }

    // copy assignment
    // Modifies an EXISTING object by copying from another existing object
    // x = y -----> x.operator = (y)
    public:
    Vector& operator = (const Vector& v) {
    // const -> the actual object shouldn't be modified

        cout<< "Copy assignment called" << endl;

        // deleting the heap memory
        // delete *this->ptr; -> incorrect because *this->ptr is pointing to an integer instead of the sequence
        // delete this->ptr; -> incorrect because the memory was allocated as an array
        delete[] this->ptr;
        this->size = v.size;
        this->ptr = new int[v.size];
        for(int i = 0;i < v.size; i++) {
            this->ptr[i] = v.ptr[i];
        }
        return *this;
    }


    // move constructor
    // Take the location of the temporary memory and point it to the current object and delete that temporary data
    public:
    Vector(Vector&& v) {
    // used <T>&& because it is the universal holder and also can hold the temporary objects/r-values

        cout<< "move constructor called" << endl;

        this->size = v.size;
        this->ptr = v.ptr;

        // v will be destroyed but just for safety
        delete[] v->ptr;
        v.size = 0;
    }

    // Move assignment operator
    // Used to equate the temporary object
    // like
    /*
        Vector v {3};
        v = getVector(); // getVector() -> returns us a vector
    */
    Vector& operator=(Vector&& v) {
        if(this != &v){
            delete[] this->ptr;
            this->ptr = other.ptr;
            this->size = other.size;
        }

        return *this;
    }

    // Deconstructor
    ~Vector() {
        cout << "Destructor called !!" << endl;
        delete[] this->ptr;
        this->size = 0;
    }


};

/*
int main()
{   
    // int* p = {1, 2, 3};
    Vector v {2};
    Vector c = v; // should call the copy constructor in old code 
    Vector d(6); // Constructor 
    d = c; // copy assignment
    Vector m = getVector(); // constructor + destructor + move constructor fold old code but the thing is Guaranteed collision happens
    
    // destructor for 4 times -> v, c, d, m after }
}    
*/

