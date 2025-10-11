#include <iostream>

using namespace std;

template <typename T>
struct List
{
	T obj;
};

int main()
{
  int x = 12;
  int* xptr = &x;
  List<int*> list;
  int y = 15;
  list.obj = &y;
  std::cout << list.obj << endl;
  std::cout << x << endl;
  std::cout << y << endl;
  
  return 0;
}
