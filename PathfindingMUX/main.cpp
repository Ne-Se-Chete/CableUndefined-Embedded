#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

typedef struct Multiplexer {
    int num;  // Identifier for the multiplexer
    struct Node* x[16];
    struct Node* y[8];
    
    Multiplexer(int n) : num(n) {
        for (auto& xi : x) xi = nullptr;
        for (auto& yi : y) yi = nullptr;
    }
} Multiplexer;

struct Node {
    Multiplexer* mux;
    int index;
    bool isX;

    Node(Multiplexer* m, int i, bool x) : mux(m), index(i), isX(x) {}

    // Helper function to print node information
    void print() {
        cout << "MUX" << mux->num << (isX ? ".x[" : ".y[") << index << "]";
    }
};
