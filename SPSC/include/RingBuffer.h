#include <iostream>
#include <vector>

#ifndef RBF
#define RBF

using namespace std;

template<typename T, int size>
class RingBuffer{

    // The Buffer
    T buffer[size];

    // Initialize both the head and tail with 64 bit alignment to avoid cache thrashing(ping pong effect) 
    alignas(64) atomic<size_t> head;
    alignas(64) atomic<size_t> tail;

    public:
    RingBuffer() : head(0), tail(0) {}

    bool push(const T& val){ // not sure about the type of val we get and making it const is used because the data we are going to put is confirmed
        // making the reads from the core's store buffer to avoid memory reordering problem
        size_t currentTail = tail.load(std::memory_order_relaxed);

        // calculate the next tail
        size_t nextTail = (currentTail + 1) % size;

        // need to check if the buffer is full
        // As per our intuition if the tail is just behind the head, then buffer is full
        if(nextTail == head.load(std::memory_order_relaxed)){
            return false;
        }

        // Write the data safely
        buffer[currentTail] = val;

        // Safely flush the data from core's store buffer to enable MESI protocol
        tail.store(nextTail, std::memory_order_relaxed);

        return true;
    };

    bool pop(T& val){
        // making the reads from the core's store buffer to avoid memory reordering problem
        size_t currentHead = head.load(std::memory_order_relaxed);

        // if the current head = current tail -> buffer is empty
        // We need to wait
        if(currentHead == tail.load(std::memory_order_relaxed)){
            return false;
        }

        // Read the data from the current head
        val = buffer[currentHead];

        // calculate the next head
        size_t nextHead = (currentHead + 1) % size;

        // write it safely
        head.store(nextHead, std::memory_order_relaxed);

        return true;
    };
};

#endif