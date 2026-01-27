// Ring buffer:
// - It is like a clock with 2 hands red and blue
// - Red hand is the producer and blue is the consumer
// - When blue meets the red - buffer is emtpy and consumer needs to wait
// - When red is about to meet blue - buffer is full
// - Red is not allowed to corss blue 
// - To build this, we need exactly 3 components
//     - Storage - T buffer[1024] - we avoid vector because it uses heap memory, we want it
//     to be pre allocated and to avoid page faults
//     - write_index - index at which producer is allowed to modify - consumer can ony read 
//     this to check whether the buffer is empty
//     - read_index - index at which consumer is allowed to take data from - prodcer can only 
//     read this to check if the buffer is full
// - There are no race conditions because they occur only when 2 threads try to access same variable
// at the same time, but in here producer modifies write_index and consumer can only read it and vice versa
// with the read_index.
// - Tradeoffs are
//     - Limited size buffer
//     - if the buffer is empty, consumer sits in there and checks buffer in a loop infinitely
//     which is a high CPU burn, but usually the threads are generally pinned to a specific core
//     so it doesn't slow down the CPU
//     - It is used only for the SPSC and if there are more than one in either producers or consumers
//     we will come back to the locks 

// We use atomics for both the head and tail pointers because using the normal int can lead to compiler optimisation
// where the compiler actually replaces the values of both the pointers to integers and we are done for and 
// by using atomics to them we are specifying that these may change 
// Also atomic introduces memory fence for reads and writes in the core
// By making the head and tail pointers, there can be a pingpong effect("cache thrashing") where both of them are present in the same
// cache line, so we always try to make them fall in different cache lines

#include "../include/RingBuffer.h"
#include <iostream>
#include <string>
#include <thread>

using namespace std;


int main(){
    RingBuffer<string, 1024> rbf;
    
    // start the producer thread
    std::thread producerThread([&](){
        string data = "packet";
        for(int i = 0;i < 100; i++){
            // need to wait until data is pushed
            while(!rbf.push(data + to_string(i))){
                // wait
            }
            cout << "Pushed the data \n";
        }
    });


    // start the consumer thread
    std::thread consumerThread([&](){
        for(int i = 0;i < 100; i++){
            string val;
            // Same with consumer also because we need to wait until there is data
            while(!rbf.pop(val)){
                // wait
            }
            cout << "Consumed the data \n";
        }
    });

    producerThread.join();
    consumerThread.join();

    return 0;
}


