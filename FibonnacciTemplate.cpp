#include <iostream>

using namespace std;

template <int N>
struct fib {
	static constexpr int val = fib<N-1>::val + fib<N-2>::val;
};

template <>
struct fib<1> {
	static constexpr int val = 1;
};

template <>
struct fib<0>{
	static constexpr int val = 0;
};

int main()
{
	cout << fib<10>::val << endl;

}






