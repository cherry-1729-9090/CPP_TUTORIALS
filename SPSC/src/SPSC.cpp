// A producer and a consumer run concurrently.
// The producer generates items continuously and places them onto a conveyor belt (buffer) of fixed capacity.
// The consumer removes items from the belt and consumes them.

// Constraints:
// - The producer must block if the belt is full.
// - The consumer must block if the belt is empty.
// - The system must be thread-safe and free from race conditions.
// - No busy-waiting is allowed.

// Classic producer–consumer solutions use:
// - Mutexes
// - Semaphores
// - Condition variables
// - Blocking

// All of these:
// - Cause context switches
// - Involve kernel scheduling
// - Introduce unbounded latency

// HFT requirements (non-negotiable):
// - No syscalls in the hot path
// - No locks
// - No blocking
// - Predictable latency (no jitter)
// - Cache-line friendliness
// - False-sharing avoidance
// - Minimal memory fences

// So we need to design a data structure that actually solves this problem of giving us lock free environment

// The solution is to develop a ring buffer

#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include "../include/producer.h"
#include "../include/consumer.h"

using namespace std;

std::mutex buffer_mutex; 

int producer::produce(vector<string>* v, int size){
    std::lock_guard<std::mutex> lock(buffer_mutex);
    if(v->size() == size)
        return 1;
    std::cout << "Produced\n";
    v->push_back("Object");
    return 0;
}

int consumer::consume(vector<string>* v,int size){
    std::lock_guard<std::mutex> lock(buffer_mutex);
    if(v->size()  < 1){
        // donot consume if the size of the buffer is 0
        return 1;
    }
    std::cout << "Consumed\n";
    v->pop_back();
    return 0;   
}

int main(){
    std::vector<string> buffer; // let us keep the size of the buffer to be 100
    
    producer p;
    consumer c;

    // Threads need function objects, lambdas or function pointers
    std::thread producerThread([&](){
        for(int i = 0; i < 100; i++){
            p.produce(&buffer, 100);
        }
    });

    std::thread consumerThread([&](){
        for(int i = 0; i < 100; i++){
            c.consume(&buffer, 100);
        }
    });


    // join() is used for a thread to block the calling thread (e.g., the main thread) until the joined thread finishes its execution
    producerThread.join();
    consumerThread.join();

    std::cout << buffer.size() << "\n";

    return 0;
}