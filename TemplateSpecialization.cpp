#include <iostream>
#include <cstdint>

using namespace std;

struct St{
	std::string id;
};

struct Cl{
	std::string id;
};

template <typename T>
void appendH(T&& obj){
	// obj.id += "Hello";
}

int main() {
	
	St s1{"3"};
  	const Cl c1{"50"};
	
  	appendH(s1);
  	appendH(c1);
  
  	St* sptr {&s1};
  	appendH(sptr);
  
  	appendH(St{"5"});
  
  	cout << s1.id << endl;
  	cout << c1.id << endl;
}