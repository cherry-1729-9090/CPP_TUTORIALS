#include <cstdio>
#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

class Order {
public:
	int32_t id;

	bool cancelOrder() {
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

	// cout << sizeof(GTC) << endl;

	
	
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