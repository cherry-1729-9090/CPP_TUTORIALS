#include <iostream>

using namespace std;

struct Banana{
    int id;
    Banana(int id){
        this->id = id;
        cout << "Banan constructor" << endl;
    }
};

// single producer, single consumer
template<typename T>
class SPSC {
private:
    size_t pushPtr;
    size_t popPtr;

    size_t totalSize;
    // T buffer[totalSize]; no need for this because the size is known at run time because if this is used
    // it will initialize an array of size 0
    T* buffer;
public:
    SPSC(size_t totalEntries) {
        this->totalSize = totalEntries;
        buffer = new T[totalSize];
        // when we allocate memory with new, it also calls the constructor
    }
    void push(){}

    void pop(){}
};

void producer() {

}

void consumer() {

}

int main() {

    SPSC<Banana> g(10000);

    return 0;
}
