// Ring buffer lives in the processes' heap but shared memory is like it's a file that both processes map
// their RAM to.
// Once we get the RingBuffer* then the code is identical, the CPU doesn't know that the memory
// came from a file

// Important fileDescriptor and other syscalls :
// - shm_open("<path to the file>", <permissions like read write>, <permissions in number like 0666, etc>)
//     Like standard open sys call to a file
//     The difference is that if we use open("<file_path>", ...), the OS creates a real file on our
//     hard drive which is very slow
//     shm_open() creates file in a special OS foler(usually /dev/shm in linux) and the folder
//     lives inside the RAM
//     It gives us the file descriptor for the lookup and guarantees that we never actually touch the
//     slow hard drive
// - ftruncate(<file_descriptor>, <sizeOfObject>)
//     Even when we create a file in RAM it is still 0 bytes
//     ftruncate forcefully streches the size of the file to be given size of bytes and then the OS
//     reserves the physical RAM for us
//     If we forget this, mmap will fail(show some bus error), because we are trying to map a file that
//     has no contents
// - mmap(<address hint for virtual memory (usually kept 0)>, <size of the bytes we want to map to>, <Read write permissions>, 
// MAP_SHARED, <file descritptor>, <offset>)
//     Address hint(usually 0) - to indicate the address of virtual memory and by putting 0, you are telling your OS 
//     you don't care about where your virtual memory is and asking OS to put this in a free space
//     PROT_READ | PROT_WRITE: "I intend to read and write to this memory." (If you only asked for READ, 
//         writing would cause a Segfault)
//     MAP_SHARED (Crucial): This is the switch.
//         MAP_PRIVATE: "Copy on Write". If I write, keep it secret. (Used for loading DLLs).
//         MAP_SHARED: "If I write, let everyone else who mapped this file see it instantly." 
//     fd - file descriptor from shm_open()
//     offset - set the mapping from the offset of the file

// Also in the shared memory, we never use any kind of data structure that will br present in the heap of the thread
// We use normal array instead of vector for and char array for string

#include <iostream>
#include <string>
#include <thread>
#include "../include/SHM_rbf.h"
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>          /* For ftruncate */
#include <atomic>
#include <cstring>

using namespace std;

int main(){

    const char* filePath = "./src/";
    const int SIZE = sizeof(SHM_RBF<1024>);

    // file descriptor
    int fd = shm_open(filePath, O_CREAT | O_RDWR, 0666);

    // ftruncate
    ftruncate(fd, SIZE);

    // use mmap
    void* ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    SHM_RBF<1024>* rbf = new (ptr) SHM_RBF<1024>();


    std::thread producerThread([&](){
        for(int i = 0; i < 100; i++){
            string data = "packet";
            data = data + to_string(i);
            while(!rbf->produce(data.c_str())){
            }
            std::cout << "Produced \n";
        }
    });

    std::thread consumerThread([&](){
        for(int i = 0; i < 100; i++){
            char received_data[64];  // Buffer to receive consumed data
            while(!rbf->consume(received_data)){
            }
            std::cout << "Consumed: " << received_data << "\n";
        }
    });

    producerThread.join();
    consumerThread.join();

    // Cleanup shared memory
    munmap(ptr, SIZE);
    close(fd);
    shm_unlink(filePath);

    return 0;
}



