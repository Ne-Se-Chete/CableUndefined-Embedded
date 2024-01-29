#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Multiplexer;  // Forward declaration

class Node {
public:
    Multiplexer* mux;
    int index;
    char connectionType;

    Node(Multiplexer* m, int i, char type);

    void print() const;
};

class Multiplexer {
    public:
        int num;
        Node* x[16];
        Node* y[8];

        Multiplexer(int n);

        void printConnections() const;
    };

    Node::Node(Multiplexer* m, int i, char type) : mux(m), index(i), connectionType(type) {}

    void Node::print() const {
        cout << "MUX_" << mux->num << "." << connectionType << "[" << index << "]";
    }

    Multiplexer::Multiplexer(int n) : num(n) {
        for (auto& xi : x) xi = nullptr;
        for (auto& yi : y) yi = nullptr;
    }

    void Multiplexer::printConnections() const {
        for (int i = 0; i < 16; ++i) {
            if (x[i]) {
                cout << "MUX_" << num << ".x[" << i << "] -> ";
                x[i]->print();
                cout << endl;
            }
        }
        for (int i = 0; i < 8; ++i) {
            if (y[i]) {
                cout << "MUX_" << num << ".y[" << i << "] -> ";
                y[i]->print();
                cout << endl;
            }
        }
}

int main() {
    Multiplexer mux1(1), mux2(2), mux3(3);

    mux3.x[1] = new Node(&mux2, 3, 'x');
    mux1.y[2] = new Node(&mux2, 4, 'y');

    // Print connections
    mux1.printConnections();
    cout << "-------------------"<< endl;
    mux3.printConnections();


    // Clean up memory (deallocate Nodes)
    // delete mux1.x[1];
    // delete mux1.y[2];

    return 0;
}
