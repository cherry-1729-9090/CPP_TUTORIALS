#include <iostream>
using namespace std;

// struct Order {
//     int32_t Price;
//     int32_t quantity;
// };

// struct OrderBook {
//     int bids[3];
//     int asks[3];
// };

// void match(OrderBook* o) {
//     for(int i = 0;i < 3;i++) {
//         if((o->bids[i]) >= (o->asks[i])){
//             o->bids[i] = 0;
//             o->asks[i] = 0;
//         }
//     }
// }


// int getTotalQuantity(Order orders[], int size){
//     int32_t totalQuantity = 0;

//     for(int i = 0;i < size;i++) {
//         totalQuantity = totalQuantity + (orders+i)->quantity;
//     }

//     return 0;
// }

// int* fun(int n) {
//     int arr[n] = {1,2,3};
//     return arr;
// }


// int** createMatrix(int m, int n) {
//     int** matrix = new int*[3];

//     for(int i = 0; i < 3; i++){
//         matrix[i] = new int[3];
//     }

//     return matrix;
// }

// void makeNull(int* n) {
//     n = nullptr;
// }


// Linked List
struct Node {
    int data;
    Node* next;

    // constructor
    Node(int val) {
        data = val;
    }

    // destructor
    ~Node() {
        delete &data;
        delete next;
    }
};

// Node makeLinkedL() {

//     Node* head;
//     head = (Node*) malloc(1000 * 12 * sizeof(int));

//     Node head;
//     head.data = 1;
//     Node next;
//     next.data = 2;
//     head.next = &next;

//     return head;
// }

const int getMyNumber() {
    return int {6};
}

int main() {
    // array of structs Orders
    // Order -> price and quantity

    // Order orders[5] = {{1,1},{1,1},{1,1},{1,1},{1,1}};

    // int* n = new int(3);

    // creating an m*n matrix
    // n elements in each array
    // m arrays

    // array will finally get decayed as a pointer in the end
    // let array be 3*3
    // int** matrix = createMatrix(3,3);

    // cout << matrix << endl;

    // for(int i = 0; i < 3; i++){
    //     for(int j = 0; j < 3; j++){
    //         matrix[i][j] = 0;
    //     }
    // }

    // for(int i = 0; i < 3; i++){
    //     for(int j = 0; j < 3; j++){
    //         cout << matrix[i][j] << " "; 
    //     }
    //     cout << "\n" << endl;
    // }

    // int n = 3;

    // cout << n << " " << &n << endl;

    // makeNull(&n);

    // cout << n << " " << &n << endl;

    // Node head = makeLinkedL();
    // while(head.next != nullptr) {
    //     cout << head.data << endl;
    //     head = *head.next;
    // }

    int num {getMyNumber()};
    std::cout << num << endl;

}


// Hi 👋 C++ Learners ( HFT Batch) 
// I will expect you guys to come prepared with 
// - Template Deductions 
// - ⁠Padding and Alignment
// - ⁠R Value Refs, L values Refs, Universal Refs
// - ⁠Object Slicing 
// - ⁠Template Specialisation 
// - ⁠Template MetaProgramming 

// ( I will try to organise a surprise test/quiz next week) :) 
// Feel free to reach out if you have doubts
