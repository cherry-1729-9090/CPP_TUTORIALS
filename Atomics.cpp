#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <thread>
#include <atomic>

using namespace std;

struct Node {
	int data;
	Node* next;
	
	Node(int d){
	    this->data = d;
	    this->next = nullptr;
	}
};

atomic<Node*> head = nullptr;
int count = 0;
    
void pushFwd(int value) {
    Node* n = new Node(value);

    // head.load() and head.store() are individually atomic but they can cause race too so we need a lock
    //  In here we chose spin lock
    // Node* currentNode = head.load();
    // if(head == currentNode) {
        //     // if reading a variable, need to use the .load() method similar to volatile
        //     n->next = head.load();
        //     head.store(n);
    //     // head = n; instead of this we use .store() method
    //     count++;
    // }
    // else {
    //     // Retry
    //     head = currentNode.load();
    // }
    /*
        first load the current head
        in the middle someone might actually change the head
        so check if the current head we got is the actual head
        then perform the operation
        and if it is not then retry it
        This can actually work because we using the keyword atomic
        which is similar to volatile in java    
    */

    Node* oldNode;
    do {
        oldNode = head.load();
        n->next = oldNode;
        // head = n; -> we are doing this in the compare_exchange_strong itself
    }while(!head.compare_exchange_strong(oldNode, head));

    // val.compare_exchange_strong(expected, value_you_wanted_to_put)

}

void pushEven(int n) {
    for(int i = 0; i <= n; i = i + 2){
        pushFwd(i);
    }   
}

void pushOdd(int n) {
    for(int i = 1; i <= n; i = i + 2){
        pushFwd(i);
    }   
}

void verify(int n) {
    unordered_map<int, int> mp;
    Node* temp = head;
    
    while(temp) {
        mp[temp->data]++;
        temp = temp->next;
    }
    
    cout << "Total nodes counted in the list : " << count << endl;
    bool ok = true;
    
    for(int i = 0;i <= n; i++){
        if(mp.find(i) ==  mp.end()){
            cout << "Missing : " << i << endl;
            ok = false;
        }else if(mp[i] > 1){
            cout << "Duplicate : " << i << endl;
            ok = false;
        }
    }
    
    if(ok) {
        cout << "Verification passed" << endl;
    }else{
        cout << "Verification failed" << endl;
    }
}


int main() {
    int n = 1000;
    
    thread t1(pushEven, n);
    thread t2(pushOdd, n);
    
    t1.join();
    t2.join();
    
    verify(n);
    
    return 0;
}


/*
Atomics are hardware level 
std::atomic in C++ is a request to the compiler to use special, uninterruptable machine instructions
that are built only on CPU
atomic -> saying all the cores that the current core is using that cache line so no other core should
use it for writing/reading
MESI protocol occurs
kind of serialization but very much optimal than mutex/spin lock
*/

/*
An atomic operation is an operation that is guaranteed by the CPU hardware to be indivisible and uninterruptible. From the perspective of all other threads, it happens all at once, or not at all.

Important Points
- Hardware, Not Software: Atomics are special machine instructions (e.g., LOCK ADD). They are not a 
software concept like a mutex. Because of this, they are called "lock-free."
- The Analogy: A mutex is like asking the OS to put a guard in front of a room (a block of code).4
An atomic is like having a special, secure mailbox where depositing a letter (updating a variable) is 
an instantaneous and unbreakable action.
- Scope: Atomics protect a single operation (like an increment, add, or swap) on a single variable. 
They cannot protect a large block of code with multiple variables.

Optimization over Mutex and Spinlock :
Atomics avoid the massive overhead of software locks.

vs. Mutex: When a thread has to wait for a mutex, it usually goes to sleep, causing an expensive
context switch (OS overhead + cache pollution). An atomic operation just makes the CPU stall for a
few nanoseconds. It's thousands of times faster than a context switch.

vs. Spinlock: When a thread waits for a spinlock, it burns 100% of its CPU core in a busy-wait loop. 
An atomic operation has no loop; the hardware handles the contention efficiently for a few cycles.

*/

/*
How to Handle Large Code Blocks with Atomics
This is your second excellent question. The answer is simple: you don't.
- Atomics are for protecting a single operation on a single variable. They are the building blocks for 
lock-free programming.Mutexes are for protecting a block of code (a critical section) that might 
involve multiple variables or complex logic.
- If your operation requires you to modify two separate shared variables in a consistent way (like head 
and a non-atomic count), a single atomic operation is not enough. The entire block needs to be protected.

* The Rule:
- If your critical section is just one simple action on one variable (increment, set a flag, swap 
a pointer), use std::atomic.
- If your critical section involves multiple variables, multiple steps, or any potentially slow 
operations (like I/O or heap allocation), you must use a lock like std::mutex.

Trying to build complex, multi-variable operations using only atomics is the domain of expert-level 
lock-free programming and is incredibly difficult to get right. For almost all application code, a 
mutex is the correct and safer choice for protecting a block of code.

*/