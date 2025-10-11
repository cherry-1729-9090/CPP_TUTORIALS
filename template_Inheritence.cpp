#include <iostream>
#include <cstdint>

using namespace std;

template<typename Derived>
class Order {

    int32_t ID;

    public:
    void cancel() {
        static_cast<Derived*>(this)->cancel();
    }

};

class IOC: public Order<IOC> {
    public:
    void cancel() {
        cout << "Order cancelled" << endl;
    }
};


int main() {
    Order<IOC> ioc;
    ioc.cancel();    
}


/*

*** This is not an inheritence - we are just doing a typecast


CRTP :
- The current method of using the template to typecaste the given object instead of using the virtual
keyword is called CRTP(Curiously recurring template pattern).
- This is a powerful template technique that mimics the virtual function at compile time
- Algo:
    You make the base class a template that takes the derived class as a template parameter.
    The base class can then safely static_cast the base class pointer(this) to child class and then 
    call the function.

- Con : We cannot store a different CRTP objects like (GTC/IOC) in a common container like 
std::vector<Order*>

*/












