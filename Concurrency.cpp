#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <thread>

using namespace std;

struct Node {
	int data;
	Node* next;
	
	Node(int d){
	    this->data = d;
	    this->next = nullptr;
	}
};

Node* head = nullptr;
int count = 0;
    
void pushFwd(int value) {
    Node* n = new Node(value);
    n->next = head;
    head = n;
    count++;
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

There might be some missed nodes here because of the lines
 `
    n->next = head; ---> line 1
    head = n; -----> line 2
`
Actually in the line `n->next = head` no MESI protocol applies because the common or shared cache line 
for both t1 and t2 is the head cache line so there is no shared cache line for this line
let us take examples like t1's n in these lines is 2 and t2's n is 1
t1 contains the cache line that contains 2 and t2 contains 1
so in here, let us take these steps

t1 -> new node = 2 -> 2 is stored in t1's l1 cache
t2 -> new node = 1 -> 1 is stored in t2's l1 cache
t1 -> need head -> brings head cache line to t1's l1 cache
same happens with t2 as well -> head cache line is shared over here -> MESI can happen
t1 -> 2.next = head -> no MESI protocol since only 2 is changing
t2 -> 1.next = head -> same
t1 -> head = 2 -> MESI protocol starts and says to t2 that invalidate your cache, now head is pointing to 2
t2 -> head = 1 
In here, if you see carefully, by the time MESI get started t2 already did 1.next = head
the only instruction left is that head = 1
so t2 takes head which points to and goes head = 1
now we lost 2

In this way from the above program we might actually loose a lot of data

*/

/*
A race condition is a bug that occurs when multiple threads access a shared resource (like your global 
head pointer and count variable) at the same time, and at least one of them modifies the resource. The 
final outcome of the program depends on the unpredictable, non-deterministic order in which the threads
 are scheduled by the operating system.

Analogy: The Shared Whiteboard
Imagine two people are told to update a number on a whiteboard. The current number is 10. The 
instruction is: "Read the number, add 5 to it, and write the new number back."
- Person A walks to the board and reads the number 10.
- Person B walks to the board and also reads the number 10.
- Person A calculates 10 + 5 = 15 and writes 15 on the board.
- Person B, who is working with the old value they read, calculates 10 + 5 = 15 and also writes 15 
on the board.
The final result is 15. It should be 20. An entire update was lost because they raced to update the 
shared resource based on stale information.

*/


/*
Cache Coherence (like the MESI protocol) is a hardware-level guarantee. Its job is to ensure that all 
CPU cores have a consistent view of a single memory location. It guarantees that if Core A writes 100 
to an address, Core B will not read a stale value of 50 from that same address moments later. It makes
 the memory system "coherent."
*/


/*
Cache cohersion (MESI) has nothing to do with the Race condition over here and also both of them operate
at different levels
*/

/*
context switch ->A context switch is the process where a computer's CPU saves the state of a currently 
running process or thread and loads the state of a new one to resume execution later. This is a
fundamental operation in multitasking operating systems that allows the CPU to rapidly switch 
between different tasks, creating the illusion that multiple processes are running at the same 
time by making it seem like it's executing multiple processes simultaneously

In our situation if we use mutex, then there could be a lot of context switches and also cache pollution
*/

/*
mutex -> We can use mutex in here which is same as `synchronized` in java saying that no 2 threads can
access the block at the same time

we can use mutex in the pushfwd function like this
```
mutex.lock();
n->next = head;
head = n;
mutex.unlock();
```
This also can cause a lot of context switches and can cause cache pollution

Mutex is a broader concept of lock that provides mutual exclusion and can be implemented in various
ways, including the spin lock or by putting a thread to sleep
*/

/*
spin lock ->A spinlock is a type of lock where a waiting thread repeatedly checks if the lock is free
in a tight loop, wasting CPU time. It's different from a mutex, which puts the waiting thread to sleep 
to save CPU time. A mutex is the correct solution for your linked list problem.
*/


// conclusion -> We can use both the mutex and also the spin lock to solve the problem but they are 
// operationally too expensive to do and also can cause a lot context switches and cache pollution (not both
// only mutex does but spin lock takes CPUs most of the time which is unoptimal) 

// Atomics is the best solution to this problem like through atomics, we can make multiple operations into
// a single operation


