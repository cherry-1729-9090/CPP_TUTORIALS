#include <cstdio>
#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

class Order {
public:
	int32_t id;

	// won't work because without specifying the function without `virtual` keyword, calling this function
	// on the order pointer which can refer to it's child objects, it decided the function call on the type
	// of pointer object
	// bool cancelOrder() {
	// 	cout << "NOT KNOWN" << endl;
	// 	return false;
	// }

	virtual bool cancelOrder() {
		cout << "NOT KNOWN" << endl;
		return false;
	}

};

// GTC -> bid is at 20 and ask is changes from 30 to 31 so it goes back to the order book instead of trading
// similar to extending in java
// everything we define in a class is private by default
// everything we define in a struct is publuic by default
class GTC: public Order {
	int canelTime;
public:
	bool cancelOrder() {
		std::cout << "Yes cancelled" << endl;
		return true;
	}
};

// IOC -> it is like if the bid is at 20 and ask is at 30 and if we ask for 31 then it kills the order and not send it to order book
class IOC: public Order {
	int fillTime;
public:
	bool cancelOrder() {
		std::cout << "Sorry not cancellable" << endl;
		return false;
	}
};

int main()
{
	GTC gtc;
	IOC ioc;

	cout << sizeof(GTC) << endl;

	Order* o = &gtc;

	o->cancelOrder();
	
	
	// won't work because of the 
	// Order g = gtc;
	// vector<Order> orders = {gtc, ioc};
	// for(auto i: orders) {
	// 	cout << i.cancelOrder() << endl;
	// }

}


/*	
Object Slicing :
- Object slicing is the process where the extra data and behavior of a derived (child) class are lost 
when it's copied or assigned to an object of the base (parent) class.
	
	GTC gtc;      // Create a large "GTC" brick.
	Order order = gtc; // Create a small "Order" brick from the GTC brick.

The compiler must create a new Order object named order. To do this, it looks at the GTC object gtc 
and copies only Order part of GTC into the variable order variable. The GTC specific parts like cancelTime
has nowhere to go in the smaller Order brick, so they are sliced off and discarded.

The new order variable is now a pure Order object. It has no memory of being GTC.

	vector<Order> orders = {gtc, ioc};
	for(auto i: orders) {
		cout << i.cancelOrder() << endl;
	}
	
	O/P : 
		NOT KNOWN
		NOT KNOWN


This is exactly what happens with your vector<Order>. A vector<Order> is a contiguous block of memory 
containing only Order-sized bricks. When you execute orders.push_back(gtc), the vector allocates a new 
Order-sized slot and copies the Order part of your gtc object into it, slicing off the rest.

So instead of storing Order objects, we can use the pointers/references to the GTC/IOC objects

*/


/*
`virtual` -> Using this to a function tells that the function call shouldn't call the implementation based
on the type of the pointer it is calling on, it needs to check the type of the reference which the given 
pointer is pointing to.

- Making a function virtual adds an array of function pointers to the class -> this is completely hidden and only
compiler can see it
- So when I call the function with virtual keyword on a pointer, it first checks the reference to which the
pointer is pointing to and then it decides the implementation based on that
- A function that is virtual in a base class is automatically virtual in all derived classes.

- Think of the virtual keyword in the base class (Order) as establishing a "contract." 
It says, "Any class that inherits from me may provide its own version of cancelOrder, and the program 
must be able to choose the correct version at runtime."

*/

/*

Using virtual can cause very unoptimality in the code.
Memory :
- When we made a class polymorphic using the virtual keyword, the compiler adds a hidden member (V pointer)
to every object of that class
- On a 64 bit os, the v pointer takes 8 bytes 
- So if we create multiple instance of these objects, it can add up to a lot of memory and can potentially
impact on CPU cache.

Time :
- When a normal function is called, it is resolved at compile time because the compiler knows the exact
memory address of the function to call. 
- It generates a direct call <address> instruction. This is a single direct jump.
- While when a virtual function gets called, it generates code that performs a v-table lookup at run-time :
	- Access the object's virtual pointer. (first memory read)
	- Follow the pointer to the class's v table.
	- Lookup for the correct function pointer at fixed offset in the v-table. (second memory read)
	- Call the function using address retrieved from the vtable. (indirect jump)
*/