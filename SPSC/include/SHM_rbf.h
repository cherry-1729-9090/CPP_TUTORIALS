#ifndef SHM_RBF
#define SHM_RBF

#include <iostream>
#include <atomic>
#include <string>

using namespace std;

template<typename T, int size>
class SHM_RBF{
    alignas(64) std::atomic<size_t> head;
    alignas(64) std::atomic<size_t> tail;

    T arr[size];

    public:
    SHM_RBF() : head(0), tail(0) {}

    bool produce(const T& val, string* path);
    bool consume(T& val, string* path);
};

#endif