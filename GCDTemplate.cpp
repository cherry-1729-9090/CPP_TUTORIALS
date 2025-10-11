#include <cstdio>
#include <iostream>

using namespace std;

template <int A, int B>
struct GCD {
	static constexpr int val = GCD<B, A%B>::val;
};

template <int A>
struct GCD <A, 0>{
	static constexpr int val = A;
};

int main()
{
  std::cout << GCD<5, 4>::val << endl;
}


// method 2

// template <int A, int B>
// struct GCD {
// 	static constexpr int val = (B == 0) ? A : GCD<B, A % B>::val;
// };

// int main()
// {
//     // Use std::cout and std::endl from <iostream>
//     std::cout << GCD<5, 4>::val << std::endl; 
// }
