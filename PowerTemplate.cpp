#include <cstdint>
#include <iostream>

using namespace std;


int findPow(int m, int n) {
	if(n == 0)
      	return 1;
	int temp = findPow(m, n/2);	
  	if(! (n & 1)) {
      // Mod/Divison operations causes 20 CPU cycles	
      // so avoid n % 2
      // use n & 1
      return temp * temp;
    }
	return temp * temp * m;
}

template <int m, int n>
struct Power {
  	// my solution
  	static constexpr int temp = Power<m, n/2>::val;
	static constexpr int val = !(n & 1) ? Power<m, n/2>::val*Power<m, n/2>::val : Power<m, n/2>::val*Power<m, n/2>::val*m; 
	
  	// correction
	// static constexpr int temp = Power<m, n / 2>::val;
    // static constexpr int val = temp * temp * ((n & 1) ? m : 1);
};

template <int m>
struct Power<m, 0> {
	static constexpr int val = 1;
};

int main() {
	cout << Power<3, 15>::val << endl;
}




