#include <iostream>

using namespace std;

template<int N>
struct factorial {
  	// made the value static because we are going to access it from class and not an instance of class
  	// used constexpr because we have the value in main function to which we want to calc fact so using the compile time advantage
	static constexpr int value = factorial<N-1>::value * N;
};

template<>
struct factorial <0>{
	static constexpr int value = 1;
};

int main(){
    cout << factorial<10>::value << endl;
}


// constexpr -> In here, it says to compiler not to wait for the memory allocation in stack, just  compute it
// static -> to avoid creating the object itself cause it's done during the run time, so to avoid it we call it on the object rather than creating
// instance of the object

// https://cppinsights.io/

