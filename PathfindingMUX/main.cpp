#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Multiplexer;

class ConnectionNode {
public:
    Multiplexer* mux;
    int index;
    char connectionType;

    ConnectionNode(Multiplexer* m, int i, char type);

    void print() const;
};

class Multiplexer {
public:
    int num;
    ConnectionNode* x[16];
    ConnectionNode* y[8];

    Multiplexer(int n);

    void printConnections() const;
};

ConnectionNode::ConnectionNode(Multiplexer* m, int i, char type) : mux(m), index(i), connectionType(type) {}

void ConnectionNode::print() const {
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

bool checkBidirectionalConnection(Multiplexer& mux1, char type1, int index1, Multiplexer& mux2, char type2, int index2) {
    ConnectionNode* node1 = (type1 == 'x') ? mux1.x[index1] : mux1.y[index1];
    ConnectionNode* node2 = (type2 == 'x') ? mux2.x[index2] : mux2.y[index2];

    if (node1 && node2 && node1->mux == &mux2 && node2->mux == &mux1 && node1->index == index2 && node2->index == index1) {
        return true;
    }
    return false;
}

int main() {
    Multiplexer mux1(1), mux2(2), mux3(3);

    mux3.x[1] = new ConnectionNode(&mux2, 3, 'x');
    mux1.y[2] = new ConnectionNode(&mux2, 4, 'y');

    mux2.x[3] = new ConnectionNode(&mux3, 1, 'x');
    mux2.y[4] = new ConnectionNode(&mux1, 2, 'y');

    mux1.printConnections();
    cout << "\n"; 
    mux2.printConnections();
    cout << "\n";
    mux3.printConnections();
    cout << "\n";

    if (checkBidirectionalConnection(mux3, 'x', 1, mux2, 'x', 3)) {
        cout << "Bidirectional connection between MUX3.x[1] and MUX2.x[3]" << endl;
    } else {
        cout << "No bidirectional connection between MUX3.x[1] and MUX2.x[3]" << endl;
    }

    if (checkBidirectionalConnection(mux1, 'y', 2, mux2, 'y', 4)) {
        cout << "Bidirectional connection between MUX1.y[2] and MUX2.y[4]" << endl;
    } else {
        cout << "No bidirectional connection between MUX1.y[2] and MUX2.y[4]" << endl;
    }


    return 0;
}
