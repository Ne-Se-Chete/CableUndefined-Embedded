#include <iostream>
using namespace std;

// Function to print the binary representation of an integer
void printBits(int n) {
    for (int i = 7; i >= 0; i--) {
        int bit = (n >> i) & 1;
        cout << bit;
    }
}

int main() {
    // a = 5(00000101), b = 9(00001001)
    int a = 5, b = 9;

    cout << "a = ";
    printBits(a);
    cout << ", b = ";
    printBits(b);
    cout << endl;

    // The & (bitwise AND) in C or C++ takes two numbers as operands and does AND on every bit of two numbers. The result of AND is 1 only if both bits are 1.
    // Result: 00000001
    cout << "a & b = ";
    printBits(a & b);
    cout << endl;

    // The | (bitwise OR) in C or C++ takes two numbers as operands and does OR on every bit of two numbers. The result of OR is 1 if any of the two bits is 1.
    // Result: 00001101
    cout << "a | b = ";
    printBits(a | b);
    cout << endl;

    // The ^ (bitwise XOR) in C or C++ takes two numbers as operands and does XOR on every bit of two numbers. The result of XOR is 1 if the two bits are different.
    // Result: 00001100
    cout << "a ^ b = ";
    printBits(a ^ b);
    cout << endl;

    // The << (left shift) in C or C++ takes two numbers, the left shifts the bits of the first operand, and the second operand decides the number of places to shift.
    // Result: 00000010
    cout << "a >> 1 = ";
    printBits(a >> 1);
    cout << endl;

    // The >> (right shift) in C or C++ takes two numbers, right shifts the bits of the first operand, and the second operand decides the number of places to shift.
    // Result: 00001010
    cout << "a << 1 = ";
    printBits(a << 1);
    cout << endl;

    // The ~ (bitwise NOT) in C or C++ takes one number and inverts all bits of it.
    // Result: 11111010
    cout << "~a = ";
    printBits(~a);
    cout << endl;

    return 0;
}
