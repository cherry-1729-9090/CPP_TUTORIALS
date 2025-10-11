#include <stdio.h>
#include <iostream>
// #include <cstdin>

using namespace std;

struct SST {
    std::string name{"Devi charan"};
    int marks {10};
};

struct Acad {
    int marks{100};
};

template<typename T>
int func(T student) {
    return student.marks;
}


int main()
{
    SST obj1;
    Acad obj2;
    
    cout << func<SST>(obj1) << endl;
    cout << func<Acad>(obj2) << endl;

    return 0;
}