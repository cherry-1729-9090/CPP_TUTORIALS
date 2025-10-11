/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <cstdint>

using namespace std;

const uint64_t SIZE = 1024 * 1024 * 1024;

struct MemoryPool {
    char buffer[SIZE];
    uint64_t offset{0};
    
    void* getMemory(uint64_t bytesNeeded) {
        // if required byte size is greater than limit then returning a null pointer
        if((offset + bytesNeeded) > SIZE) 
            return nullptr;
            
        // getting the pointer of the offset    
        void* ptrOffSet = static_cast<void*>(&(buffer[offset]));
        
        // move the offset forward
        offset += bytesNeeded;
        
        return ptrOffSet;
    }
};

struct Box {
    uint64_t length;
    uint64_t size;
};

int main()
{
    MemoryPool* mPool = new MemoryPool();
    Box* box1 = reinterpret_cast<Box*>(mPool->getMemory(sizeof(Box)));
    box1->size = 10;
    
    cout << box1->size << endl;
    return 0;
}