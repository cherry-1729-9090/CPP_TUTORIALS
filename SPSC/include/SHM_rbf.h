#ifndef SHM_RBF_H
#define SHM_RBF_H

#include <iostream>
#include <atomic>
#include <string>
#include <cstring>
struct packet{
    char data[64];
};

using namespace std;

template<int size>
class SHM_RBF{
    alignas(64) std::atomic<size_t> head;
    alignas(64) std::atomic<size_t> tail;

    packet buffer[size];

    public:
    SHM_RBF() : head(0), tail(0) {}


    // prefer memory_order_acquire for read operations
    // prefer memory_order_release for write operations

    bool produce(const char* msg){
        // // Create file if not there or open if there in RAM
        // int fd = shm_open(path->c_str(), O_RDWR, 0777);

        // // strech the given file with the size of SHM_RBF object
        // ftruncate(fd, sizeof(SHM_RBF));

        // // use mmap to make the file in the given path as a shared one
        // void* ptr = mmap(0, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        // convert the pointer to our RingBuffer's pointer
        // SHM_RBF* rbf = new (ptr) SHM_RBF(); // force the constructor to run and convert our void ptr

        // ----- All the above steps are done in main ------
        // here comes our logic for producer
        size_t currentTail = tail.load(std::memory_order_acquire);

        size_t nextTail = (currentTail + 1) & (size - 1);

        if(nextTail == head.load(std::memory_order_acquire)){
            return false; // buffer is full
        }

        // rbf->buffer[currentTail] = val;
        std::strncpy(buffer[currentTail].data, msg, 63); // copy data


        tail.store(nextTail, std::memory_order_release);
        
        return true;
    }
    bool consume(char* out_buffer){
        // // Create file if not there or open if there in RAM
        // int fd = shm_open(path->c_str(), O_RDWR, 0777);

        // // no need to use ftruncate over here

        // // use mmap
        // void* ptr = mmap(0, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        // // convert the pointer to our RingBuffer's pointer
        // SHM_RBF* rbf = new (ptr) SHM_RBF(); // fore the constructor to run and convert our void ptr
        
        // ----- All the above steps are done in main ------
        // here comes our logic for consumer 
        size_t currentHead = head.load(std::memory_order_acquire);

        if(currentHead == tail.load(std::memory_order_acquire)){
            return false; // Buffer is empty
        }

        // val = rbf->buffer[currentHead];
        std::strncpy(out_buffer, buffer[currentHead].data, 63); // copy data

        size_t nextHead = (currentHead + 1) & (size - 1);

        head.store(nextHead, std::memory_order_release);

        return true;
    }
};

#endif