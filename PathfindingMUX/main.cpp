#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>

using namespace std;

// BREADBOARD OR MUX
class Device {
public:
    int num;

    Device(int n) : num(n) {}

    virtual void printConnections() const = 0;
};

class ConnectionNode{
public:
    Device* device;
    int index;
    char connectionType;

    ConnectionNode(Device* d, int i, char type);

    void print() const;
};

class Multiplexer : public Device {
public:
    ConnectionNode* x[16];
    ConnectionNode* y[8];

    Multiplexer(int n);

    void printConnections() const override;
};

class Breadboard : public Device {
public:
    ConnectionNode* pin[64];

    Breadboard(int n);

    void printConnections() const override;
};

ConnectionNode::ConnectionNode(Device* d, int i, char type) : device(d), index(i), connectionType(type) {}

void ConnectionNode::print() const {
    if (dynamic_cast<Multiplexer*>(device)) {
        cout << "MUX_" << device->num << "." << connectionType << "[" << index << "]";
    } else if (dynamic_cast<Breadboard*>(device)) {
        cout << "Breadboard_" << device->num << ".pin[" << index << "]";
    }
}

Multiplexer::Multiplexer(int n) : Device(n) {
    for (auto& xi : x) xi = nullptr;
    for (auto& yi : y) yi = nullptr;
}

Breadboard::Breadboard(int n) : Device(n) {
    for (auto& pin : pin) pin = nullptr;
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

void Breadboard::printConnections() const {
    for (int i = 0; i < 64; ++i) {
        if (pin[i]) {
            cout << "Breadboard_" << num << ".pin[" << i << "] -> ";
            pin[i]->print();  // This will print either MUX or Breadboard connection information
            cout << endl;
        }
    }
}

// check if there is a bidirectional connection between two devices
bool checkBidirectionalConnection(Device& device1, char type1, int index1, Device& device2, char type2, int index2) {
    if (auto mux1 = dynamic_cast<Multiplexer*>(&device1)) {
        if (auto mux2 = dynamic_cast<Multiplexer*>(&device2)) {
            ConnectionNode* node1 = (type1 == 'x') ? mux1->x[index1] : mux1->y[index1];
            ConnectionNode* node2 = (type2 == 'x') ? mux2->x[index2] : mux2->y[index2];

            if (node1 && node2 && node1->device == &device2 && node2->device == &device1 && node1->index == index2 && node2->index == index1) {
                return true;
            }
        } else if (auto breadboard2 = dynamic_cast<Breadboard*>(&device2)) {
            ConnectionNode* node1 = (type1 == 'x') ? mux1->x[index1] : mux1->y[index1];
            ConnectionNode* node2 = breadboard2->pin[index2];

            if (node1 && node2 && node1->device == &device2 && node2->device == &device1 && node1->index == index2 && node2->index == index1) {
                return true;
            }
        }
    } else if (auto breadboard1 = dynamic_cast<Breadboard*>(&device1)) {
        if (auto mux2 = dynamic_cast<Multiplexer*>(&device2)) {
            ConnectionNode* node1 = breadboard1->pin[index1];
            ConnectionNode* node2 = (type2 == 'x') ? mux2->x[index2] : mux2->y[index2];

            if (node1 && node2 && node1->device == &device2 && node2->device == &device1 && node1->index == index2 && node2->index == index1) {
                return true;
            }
        }
    }

    return false;
}

class Graph
{
    int numVertices;
    list<int> *adjLists;
    bool *visited;

public:
    Graph(int vertices) : numVertices(vertices), adjLists(new list<int>[vertices]), visited(new bool[vertices]()) {}

    ~Graph()
    {
        delete[] adjLists;
        delete[] visited;
    }

    void addEdge(int src, int dest)
    {
        adjLists[src].push_back(dest); // Assuming bidirectional for simplicity
        adjLists[dest].push_back(src);
    }

    void DFS(int vertex)
    {
        visited[vertex] = true;
        cout << vertex << " ";

        for (int adjVertex : adjLists[vertex])
        {
            if (!visited[adjVertex])
            {
                DFS(adjVertex);
            }
        }
    }
};

int getGraphVertexID(const Device *device, char type, int pinIndex)
{
    int deviceId = device->num;
    if (auto *mux = dynamic_cast<const Multiplexer *>(device))
    {
        return deviceId * 24 + (type == 'x' ? pinIndex : 16 + pinIndex);
    }
    else if (auto *bb = dynamic_cast<const Breadboard *>(device))
    {
        // Assuming Breadboards come after all Multiplexers in numbering
        return 18 * 24 + (deviceId - 19) * 64 + pinIndex;
    }
    return -1; // Error case
}

int main() {
    Multiplexer mux1(0), mux2(1), mux3(2), mux4(3), mux5(4), mux6(5), mux7(6), mux8(7), mux9(8), mux10(9), mux11(10), mux12(11), mux13(12), mux14(13), mux15(14), mux16(15), mux17(16), mux18(17);
    Breadboard mcu_breadboard(40), main_breadboard(64);

    
    // All bidirectional connections for MUX1
    mux1.x[0] = new ConnectionNode(&mux11, 0, 'x');
    mux11.x[0] = new ConnectionNode(&mux1, 0, 'x');

    mux1.x[1] = new ConnectionNode(&mux12, 0, 'x');
    mux12.x[0] = new ConnectionNode(&mux1, 1, 'x');

    mux1.x[2] = new ConnectionNode(&mux13, 0, 'x');
    mux13.x[0] = new ConnectionNode(&mux1, 2, 'x');

    mux1.x[3] = new ConnectionNode(&mux14, 0, 'x');
    mux14.x[0] = new ConnectionNode(&mux1, 3, 'x');

    mux1.x[4] = new ConnectionNode(&mux15, 0, 'x');
    mux15.x[0] = new ConnectionNode(&mux1, 4, 'x');

    mux1.x[5] = new ConnectionNode(&mux16, 0, 'x');
    mux16.x[0] = new ConnectionNode(&mux1, 5, 'x');

    mux1.x[6] = new ConnectionNode(&mux17, 0, 'x');
    mux17.x[0] = new ConnectionNode(&mux1, 6, 'x');

    mux1.x[7] = new ConnectionNode(&mux18, 0, 'x');
    mux18.x[0] = new ConnectionNode(&mux1, 7, 'x');

    mux1.x[8] = new ConnectionNode(&mux6, 0, 'y');
    mux6.y[0] = new ConnectionNode(&mux1, 8, 'x');

    mux1.x[9] = new ConnectionNode(&mux7, 0, 'y');
    mux7.y[0] = new ConnectionNode(&mux1, 9, 'x');

    mux1.x[10] = new ConnectionNode(&mux8, 0, 'y');
    mux8.y[0] = new ConnectionNode(&mux1, 10, 'x');

    mux1.x[11] = new ConnectionNode(&mux9, 0, 'y');
    mux9.y[0] = new ConnectionNode(&mux1, 11, 'x');

    mux1.x[12] = new ConnectionNode(&mux10, 0, 'y');
    mux10.y[0] = new ConnectionNode(&mux1, 12, 'x');

    mux1.x[13] = new ConnectionNode(&mux6, 5, 'y');
    mux6.y[5] = new ConnectionNode(&mux1, 13, 'x');

    mux1.x[14] = new ConnectionNode(&mux6, 6, 'y');
    mux6.y[6] = new ConnectionNode(&mux1, 14, 'x');

    mux1.x[15] = new ConnectionNode(&mux6, 7, 'y');
    mux6.y[7] = new ConnectionNode(&mux1, 15, 'x');

    mux1.y[0] = new ConnectionNode(&mcu_breadboard, 1, 'p');
    mcu_breadboard.pin[1] = new ConnectionNode(&mux1, 0, 'y');

    mux1.y[1] = new ConnectionNode(&mcu_breadboard, 2, 'p');
    mcu_breadboard.pin[2] = new ConnectionNode(&mux1, 1, 'y');

    mux1.y[2] = new ConnectionNode(&mcu_breadboard, 3, 'p');
    mcu_breadboard.pin[3] = new ConnectionNode(&mux1, 2, 'y');

    mux1.y[3] = new ConnectionNode(&mcu_breadboard, 4, 'p');
    mcu_breadboard.pin[4] = new ConnectionNode(&mux1, 3, 'y');

    mux1.y[4] = new ConnectionNode(&mcu_breadboard, 5, 'p');
    mcu_breadboard.pin[5] = new ConnectionNode(&mux1, 4, 'y');

    mux1.y[5] = new ConnectionNode(&mcu_breadboard, 6, 'p');
    mcu_breadboard.pin[6] = new ConnectionNode(&mux1, 5, 'y');

    mux1.y[6] = new ConnectionNode(&mcu_breadboard, 7, 'p');
    mcu_breadboard.pin[7] = new ConnectionNode(&mux1, 6, 'y');

    mux1.y[7] = new ConnectionNode(&mcu_breadboard, 8, 'p');
    mcu_breadboard.pin[8] = new ConnectionNode(&mux1, 7, 'y');


    // All bidirectional connections for MUX2
    mux2.x[0] = new ConnectionNode(&mux11, 1, 'x');
    mux11.x[1] = new ConnectionNode(&mux2, 0, 'x');

    mux2.x[1] = new ConnectionNode(&mux12, 1, 'x');
    mux12.x[1] = new ConnectionNode(&mux2, 1, 'x');

    mux2.x[2] = new ConnectionNode(&mux13, 1, 'x');
    mux13.x[1] = new ConnectionNode(&mux2, 2, 'x');

    mux2.x[3] = new ConnectionNode(&mux14, 1, 'x');
    mux14.x[1] = new ConnectionNode(&mux2, 3, 'x');

    mux2.x[4] = new ConnectionNode(&mux15, 1, 'x');
    mux15.x[1] = new ConnectionNode(&mux2, 4, 'x');

    mux2.x[5] = new ConnectionNode(&mux16, 1, 'x');
    mux16.x[1] = new ConnectionNode(&mux2, 5, 'x');

    mux2.x[6] = new ConnectionNode(&mux17, 1, 'x');
    mux17.x[1] = new ConnectionNode(&mux2, 6, 'x');

    mux2.x[7] = new ConnectionNode(&mux18, 1, 'x');
    mux18.x[1] = new ConnectionNode(&mux2, 7, 'x');

    mux2.x[8] = new ConnectionNode(&mux6, 1, 'y');
    mux6.y[1] = new ConnectionNode(&mux2, 8, 'x');

    mux2.x[9] = new ConnectionNode(&mux7, 1, 'y');
    mux7.y[1] = new ConnectionNode(&mux2, 9, 'x');

    mux2.x[10] = new ConnectionNode(&mux8, 1, 'y');
    mux8.y[1] = new ConnectionNode(&mux2, 10, 'x');

    mux2.x[11] = new ConnectionNode(&mux9, 1, 'y');
    mux9.y[1] = new ConnectionNode(&mux2, 11, 'x');

    mux2.x[12] = new ConnectionNode(&mux10, 1, 'y');
    mux10.y[1] = new ConnectionNode(&mux2, 12, 'x');

    mux2.x[13] = new ConnectionNode(&mux7, 5, 'y');
    mux7.y[5] = new ConnectionNode(&mux2, 13, 'x');

    mux2.x[14] = new ConnectionNode(&mux7, 6, 'y');
    mux7.y[6] = new ConnectionNode(&mux2, 14, 'x');

    mux2.x[15] = new ConnectionNode(&mux7, 7, 'y');
    mux7.y[7] = new ConnectionNode(&mux2, 15, 'x');

    mux2.y[0] = new ConnectionNode(&mcu_breadboard, 9, 'p');
    mcu_breadboard.pin[9] = new ConnectionNode(&mux2, 0, 'y');

    mux2.y[1] = new ConnectionNode(&mcu_breadboard, 10, 'p');
    mcu_breadboard.pin[10] = new ConnectionNode(&mux2, 1, 'y');

    mux2.y[2] = new ConnectionNode(&mcu_breadboard, 11, 'p');
    mcu_breadboard.pin[11] = new ConnectionNode(&mux2, 2, 'y');

    mux2.y[3] = new ConnectionNode(&mcu_breadboard, 12, 'p');
    mcu_breadboard.pin[12] = new ConnectionNode(&mux2, 3, 'y');

    mux2.y[4] = new ConnectionNode(&mcu_breadboard, 13, 'p');
    mcu_breadboard.pin[13] = new ConnectionNode(&mux2, 4, 'y');

    mux2.y[5] = new ConnectionNode(&mcu_breadboard, 14, 'p');
    mcu_breadboard.pin[14] = new ConnectionNode(&mux2, 5, 'y');

    mux2.y[6] = new ConnectionNode(&mcu_breadboard, 15, 'p');
    mcu_breadboard.pin[15] = new ConnectionNode(&mux2, 6, 'y');

    mux2.y[7] = new ConnectionNode(&mcu_breadboard, 16, 'p');
    mcu_breadboard.pin[16] = new ConnectionNode(&mux2, 7, 'y');


    // All bidirectional connections for MUX3
    mux3.x[0] = new ConnectionNode(&mux11, 2, 'x');
    mux11.x[2] = new ConnectionNode(&mux3, 0, 'x');

    mux3.x[1] = new ConnectionNode(&mux12, 2, 'x');
    mux12.x[2] = new ConnectionNode(&mux3, 1, 'x');

    mux3.x[2] = new ConnectionNode(&mux13, 2, 'x');
    mux13.x[2] = new ConnectionNode(&mux3, 2, 'x');

    mux3.x[3] = new ConnectionNode(&mux14, 2, 'x');
    mux14.x[2] = new ConnectionNode(&mux3, 3, 'x');

    mux3.x[4] = new ConnectionNode(&mux15, 2, 'x');
    mux15.x[2] = new ConnectionNode(&mux3, 4, 'x');

    mux3.x[5] = new ConnectionNode(&mux16, 2, 'x');
    mux16.x[2] = new ConnectionNode(&mux3, 5, 'x');

    mux3.x[6] = new ConnectionNode(&mux17, 2, 'x');
    mux17.x[2] = new ConnectionNode(&mux3, 6, 'x');

    mux3.x[7] = new ConnectionNode(&mux18, 2, 'x');
    mux18.x[2] = new ConnectionNode(&mux3, 7, 'x');

    mux3.x[8] = new ConnectionNode(&mux6, 2, 'y');
    mux6.y[2] = new ConnectionNode(&mux3, 8, 'x');

    mux3.x[9] = new ConnectionNode(&mux7, 2, 'y');
    mux7.y[2] = new ConnectionNode(&mux3, 9, 'x');

    mux3.x[10] = new ConnectionNode(&mux8, 2, 'y');
    mux8.y[2] = new ConnectionNode(&mux3, 10, 'x');

    mux3.x[11] = new ConnectionNode(&mux9, 2, 'y');
    mux9.y[2] = new ConnectionNode(&mux3, 11, 'x');

    mux3.x[12] = new ConnectionNode(&mux10, 2, 'y');
    mux10.y[2] = new ConnectionNode(&mux3, 12, 'x');

    mux3.x[13] = new ConnectionNode(&mux8, 5, 'y');
    mux8.y[5] = new ConnectionNode(&mux3, 13, 'x');

    mux3.x[14] = new ConnectionNode(&mux8, 6, 'y');
    mux8.y[6] = new ConnectionNode(&mux3, 14, 'x');

    mux3.x[15] = new ConnectionNode(&mux8, 7, 'y');
    mux8.y[7] = new ConnectionNode(&mux3, 15, 'x');

    mux3.y[0] = new ConnectionNode(&mcu_breadboard, 17, 'p');
    mcu_breadboard.pin[17] = new ConnectionNode(&mux3, 0, 'y');

    mux3.y[1] = new ConnectionNode(&mcu_breadboard, 18, 'p');
    mcu_breadboard.pin[18] = new ConnectionNode(&mux3, 1, 'y');

    mux3.y[2] = new ConnectionNode(&mcu_breadboard, 19, 'p');
    mcu_breadboard.pin[19] = new ConnectionNode(&mux3, 2, 'y');

    mux3.y[3] = new ConnectionNode(&mcu_breadboard, 20, 'p');
    mcu_breadboard.pin[20] = new ConnectionNode(&mux3, 3, 'y');

    mux3.y[4] = new ConnectionNode(&mcu_breadboard, 21, 'p');
    mcu_breadboard.pin[21] = new ConnectionNode(&mux3, 4, 'y');

    mux3.y[5] = new ConnectionNode(&mcu_breadboard, 22, 'p');
    mcu_breadboard.pin[22] = new ConnectionNode(&mux3, 5, 'y');

    mux3.y[6] = new ConnectionNode(&mcu_breadboard, 23, 'p');
    mcu_breadboard.pin[23] = new ConnectionNode(&mux3, 6, 'y');

    mux3.y[7] = new ConnectionNode(&mcu_breadboard, 24, 'p');
    mcu_breadboard.pin[24] = new ConnectionNode(&mux3, 7, 'y');


    // All bidirectional connections for MUX4
    mux4.x[0] = new ConnectionNode(&mux11, 3, 'x');
    mux11.x[3] = new ConnectionNode(&mux4, 0, 'x');

    mux4.x[1] = new ConnectionNode(&mux12, 3, 'x');
    mux12.x[3] = new ConnectionNode(&mux4, 1, 'x');

    mux4.x[2] = new ConnectionNode(&mux13, 3, 'x');
    mux13.x[3] = new ConnectionNode(&mux4, 2, 'x');

    mux4.x[3] = new ConnectionNode(&mux14, 3, 'x');
    mux14.x[3] = new ConnectionNode(&mux4, 3, 'x');

    mux4.x[4] = new ConnectionNode(&mux15, 3, 'x');
    mux15.x[3] = new ConnectionNode(&mux4, 4, 'x');

    mux4.x[5] = new ConnectionNode(&mux16, 3, 'x');
    mux16.x[3] = new ConnectionNode(&mux4, 5, 'x');

    mux4.x[6] = new ConnectionNode(&mux17, 3, 'x');
    mux17.x[3] = new ConnectionNode(&mux4, 6, 'x');

    mux4.x[7] = new ConnectionNode(&mux18, 3, 'x');
    mux18.x[3] = new ConnectionNode(&mux4, 7, 'x');

    mux4.x[8] = new ConnectionNode(&mux6, 3, 'y');
    mux6.y[3] = new ConnectionNode(&mux4, 8, 'x');

    mux4.x[9] = new ConnectionNode(&mux7, 3, 'y');
    mux7.y[3] = new ConnectionNode(&mux4, 9, 'x');

    mux4.x[10] = new ConnectionNode(&mux8, 3, 'y');
    mux8.y[3] = new ConnectionNode(&mux4, 10, 'x');

    mux4.x[11] = new ConnectionNode(&mux9, 3, 'y');
    mux9.y[3] = new ConnectionNode(&mux4, 11, 'x');

    mux4.x[12] = new ConnectionNode(&mux10, 3, 'y');
    mux10.y[3] = new ConnectionNode(&mux4, 12, 'x');

    mux4.x[13] = new ConnectionNode(&mux9, 5, 'y');
    mux9.y[5] = new ConnectionNode(&mux4, 13, 'x');

    mux4.x[14] = new ConnectionNode(&mux9, 6, 'y');
    mux9.y[6] = new ConnectionNode(&mux4, 14, 'x');

    mux4.x[15] = new ConnectionNode(&mux9, 7, 'y');
    mux9.y[7] = new ConnectionNode(&mux4, 15, 'x');

    mux4.y[0] = new ConnectionNode(&mcu_breadboard, 25, 'p');
    mcu_breadboard.pin[25] = new ConnectionNode(&mux4, 0, 'y');

    mux4.y[1] = new ConnectionNode(&mcu_breadboard, 26, 'p');
    mcu_breadboard.pin[26] = new ConnectionNode(&mux4, 1, 'y');

    mux4.y[2] = new ConnectionNode(&mcu_breadboard, 27, 'p');
    mcu_breadboard.pin[27] = new ConnectionNode(&mux4, 2, 'y');

    mux4.y[3] = new ConnectionNode(&mcu_breadboard, 28, 'p');
    mcu_breadboard.pin[28] = new ConnectionNode(&mux4, 3, 'y');

    mux4.y[4] = new ConnectionNode(&mcu_breadboard, 29, 'p');
    mcu_breadboard.pin[29] = new ConnectionNode(&mux4, 4, 'y');

    mux4.y[5] = new ConnectionNode(&mcu_breadboard, 30, 'p');
    mcu_breadboard.pin[30] = new ConnectionNode(&mux4, 5, 'y');

    mux4.y[6] = new ConnectionNode(&mcu_breadboard, 31, 'p');
    mcu_breadboard.pin[31] = new ConnectionNode(&mux4, 6, 'y');

    mux4.y[7] = new ConnectionNode(&mcu_breadboard, 32, 'p');
    mcu_breadboard.pin[32] = new ConnectionNode(&mux4, 7, 'y');


    // All bidirectional connections for MUX5
    mux5.x[0] = new ConnectionNode(&mux11, 4, 'x');
    mux11.x[4] = new ConnectionNode(&mux5, 0, 'x');

    mux5.x[1] = new ConnectionNode(&mux12, 4, 'x');
    mux12.x[4] = new ConnectionNode(&mux5, 1, 'x');

    mux5.x[2] = new ConnectionNode(&mux13, 4, 'x');
    mux13.x[4] = new ConnectionNode(&mux5, 2, 'x');

    mux5.x[3] = new ConnectionNode(&mux14, 4, 'x');
    mux14.x[4] = new ConnectionNode(&mux5, 3, 'x');

    mux5.x[4] = new ConnectionNode(&mux15, 4, 'x');
    mux15.x[4] = new ConnectionNode(&mux5, 4, 'x');

    mux5.x[5] = new ConnectionNode(&mux16, 4, 'x');
    mux16.x[4] = new ConnectionNode(&mux5, 5, 'x');

    mux5.x[6] = new ConnectionNode(&mux17, 4, 'x');
    mux17.x[4] = new ConnectionNode(&mux5, 6, 'x');

    mux5.x[7] = new ConnectionNode(&mux18, 4, 'x');
    mux18.x[4] = new ConnectionNode(&mux5, 7, 'x');

    mux5.x[8] = new ConnectionNode(&mux6, 4, 'y');
    mux6.y[4] = new ConnectionNode(&mux5, 8, 'x');

    mux5.x[9] = new ConnectionNode(&mux7, 4, 'y');
    mux7.y[4] = new ConnectionNode(&mux5, 9, 'x');

    mux5.x[10] = new ConnectionNode(&mux8, 4, 'y');
    mux8.y[4] = new ConnectionNode(&mux5, 10, 'x');

    mux5.x[11] = new ConnectionNode(&mux9, 4, 'y');
    mux9.y[4] = new ConnectionNode(&mux5, 11, 'x');

    mux5.x[12] = new ConnectionNode(&mux10, 4, 'y');
    mux10.y[4] = new ConnectionNode(&mux5, 12, 'x');

    mux5.x[13] = new ConnectionNode(&mux10, 5, 'y');
    mux10.y[5] = new ConnectionNode(&mux5, 13, 'x');

    mux5.x[14] = new ConnectionNode(&mux10, 6, 'y');
    mux10.y[6] = new ConnectionNode(&mux5, 14, 'x');

    mux5.x[15] = new ConnectionNode(&mux10, 7, 'y');
    mux10.y[7] = new ConnectionNode(&mux5, 15, 'x');

    mux5.y[0] = new ConnectionNode(&mcu_breadboard, 33, 'p');
    mcu_breadboard.pin[33] = new ConnectionNode(&mux5, 0, 'y');

    mux5.y[1] = new ConnectionNode(&mcu_breadboard, 34, 'p');
    mcu_breadboard.pin[34] = new ConnectionNode(&mux5, 1, 'y');

    mux5.y[2] = new ConnectionNode(&mcu_breadboard, 35, 'p');
    mcu_breadboard.pin[35] = new ConnectionNode(&mux5, 2, 'y');

    mux5.y[3] = new ConnectionNode(&mcu_breadboard, 36, 'p');
    mcu_breadboard.pin[36] = new ConnectionNode(&mux5, 3, 'y');

    mux5.y[4] = new ConnectionNode(&mcu_breadboard, 37, 'p');
    mcu_breadboard.pin[37] = new ConnectionNode(&mux5, 4, 'y');

    mux5.y[5] = new ConnectionNode(&mcu_breadboard, 38, 'p');
    mcu_breadboard.pin[38] = new ConnectionNode(&mux5, 5, 'y');

    mux5.y[6] = new ConnectionNode(&mcu_breadboard, 39, 'p');
    mcu_breadboard.pin[39] = new ConnectionNode(&mux5, 6, 'y');

    mux5.y[7] = new ConnectionNode(&mcu_breadboard, 40, 'p');
    mcu_breadboard.pin[40] = new ConnectionNode(&mux5, 7, 'y');
    

    // All bidirectional connections for MUX6
    mux6.x[0] = new ConnectionNode(&mux11, 5, 'x');
    mux11.x[5] = new ConnectionNode(&mux6, 0, 'x');

    mux6.x[1] = new ConnectionNode(&mux12, 5, 'x');
    mux12.x[5] = new ConnectionNode(&mux6, 1, 'x');

    mux6.x[2] = new ConnectionNode(&mux13, 5, 'x');
    mux13.x[5] = new ConnectionNode(&mux6, 2, 'x');

    mux6.x[3] = new ConnectionNode(&mux14, 5, 'x');
    mux14.x[5] = new ConnectionNode(&mux6, 3, 'x');

    mux6.x[4] = new ConnectionNode(&mux15, 5, 'x');
    mux15.x[5] = new ConnectionNode(&mux6, 4, 'x');

    mux6.x[5] = new ConnectionNode(&mux16, 5, 'x');
    mux16.x[5] = new ConnectionNode(&mux6, 5, 'x');

    mux6.x[6] = new ConnectionNode(&mux17, 5, 'x');
    mux17.x[5] = new ConnectionNode(&mux6, 6, 'x');

    mux6.x[7] = new ConnectionNode(&mux18, 5, 'x');
    mux18.x[5] = new ConnectionNode(&mux6, 7, 'x');

    mux6.x[8] = new ConnectionNode(&mux11, 10, 'x');
    mux11.x[10] = new ConnectionNode(&mux6, 8, 'x');

    mux6.x[9] = new ConnectionNode(&mux11, 11, 'x');
    mux11.x[11] = new ConnectionNode(&mux6, 9, 'x');

    mux6.x[10] = new ConnectionNode(&mux11, 12, 'x');
    mux11.x[12] = new ConnectionNode(&mux6, 10, 'x');

    mux6.x[11] = new ConnectionNode(&mux12, 10, 'x');
    mux12.x[10] = new ConnectionNode(&mux6, 11, 'x');

    mux6.x[12] = new ConnectionNode(&mux12, 11, 'x');
    mux12.x[11] = new ConnectionNode(&mux6, 12, 'x');

    // mux6.y[0] = new ConnectionNode(&mux1, 8, 'x');
    // mux1.x[8] = new ConnectionNode(&mux6, 0, 'y');

    // mux6.y[1] = new ConnectionNode(&mux2, 8, 'x');
    // mux2.x[8] = new ConnectionNode(&mux6, 1, 'y');

    // mux6.y[2] = new ConnectionNode(&mux3, 8, 'x');
    // mux3.x[8] = new ConnectionNode(&mux6, 2, 'y');

    // mux6.y[3] = new ConnectionNode(&mux4, 8, 'x');
    // mux4.x[8] = new ConnectionNode(&mux6, 3, 'y');

    // mux6.y[4] = new ConnectionNode(&mux5, 8, 'x');
    // mux5.x[8] = new ConnectionNode(&mux6, 4, 'y');

    // mux6.y[5] = new ConnectionNode(&mux1, 13, 'x');
    // mux1.x[13] = new ConnectionNode(&mux6, 5, 'y');

    // mux6.y[6] = new ConnectionNode(&mux1, 14, 'x');
    // mux1.x[14] = new ConnectionNode(&mux6, 6, 'y');

    // mux6.y[7] = new ConnectionNode(&mux1, 15, 'x');
    // mux1.x[15] = new ConnectionNode(&mux6, 7, 'y');


    // All bidirectional connections for MUX7
    mux7.x[0] = new ConnectionNode(&mux11, 6, 'x');
    mux11.x[6] = new ConnectionNode(&mux7, 0, 'x');

    mux7.x[1] = new ConnectionNode(&mux12, 6, 'x');
    mux12.x[6] = new ConnectionNode(&mux7, 1, 'x');

    mux7.x[2] = new ConnectionNode(&mux13, 6, 'x');
    mux13.x[6] = new ConnectionNode(&mux7, 2, 'x');

    mux7.x[3] = new ConnectionNode(&mux14, 6, 'x');
    mux14.x[6] = new ConnectionNode(&mux7, 3, 'x');

    mux7.x[4] = new ConnectionNode(&mux15, 6, 'x');
    mux15.x[6] = new ConnectionNode(&mux7, 4, 'x');

    mux7.x[5] = new ConnectionNode(&mux16, 6, 'x');
    mux16.x[6] = new ConnectionNode(&mux7, 5, 'x');

    mux7.x[6] = new ConnectionNode(&mux17, 6, 'x');
    mux17.x[6] = new ConnectionNode(&mux7, 6, 'x');

    mux7.x[7] = new ConnectionNode(&mux18, 6, 'x');
    mux18.x[6] = new ConnectionNode(&mux7, 7, 'x');

    mux7.x[8] = new ConnectionNode(&mux12, 12, 'x');
    mux12.x[12] = new ConnectionNode(&mux7, 8, 'x');

    mux7.x[9] = new ConnectionNode(&mux13, 10, 'x');
    mux13.x[10] = new ConnectionNode(&mux7, 9, 'x');

    mux7.x[10] = new ConnectionNode(&mux13, 11, 'x');
    mux13.x[11] = new ConnectionNode(&mux7, 10, 'x');

    mux7.x[11] = new ConnectionNode(&mux13, 12, 'x');
    mux13.x[12] = new ConnectionNode(&mux7, 11, 'x');

    mux7.x[12] = new ConnectionNode(&mux14, 10, 'x');
    mux14.x[10] = new ConnectionNode(&mux7, 12, 'x');


    // All bidirectional connections for MUX8
    mux8.x[0] = new ConnectionNode(&mux11, 7, 'x');
    mux11.x[7] = new ConnectionNode(&mux8, 0, 'x');

    mux8.x[1] = new ConnectionNode(&mux12, 7, 'x');
    mux12.x[7] = new ConnectionNode(&mux8, 1, 'x');

    mux8.x[2] = new ConnectionNode(&mux13, 7, 'x');
    mux13.x[7] = new ConnectionNode(&mux8, 2, 'x');

    mux8.x[3] = new ConnectionNode(&mux14, 7, 'x');
    mux14.x[7] = new ConnectionNode(&mux8, 3, 'x');

    mux8.x[4] = new ConnectionNode(&mux15, 7, 'x');
    mux15.x[7] = new ConnectionNode(&mux8, 4, 'x');

    mux8.x[5] = new ConnectionNode(&mux16, 7, 'x');
    mux16.x[7] = new ConnectionNode(&mux8, 5, 'x');

    mux8.x[6] = new ConnectionNode(&mux17, 7, 'x');
    mux17.x[7] = new ConnectionNode(&mux8, 6, 'x');

    mux8.x[7] = new ConnectionNode(&mux18, 7, 'x');
    mux18.x[7] = new ConnectionNode(&mux8, 7, 'x');

    mux8.x[8] = new ConnectionNode(&mux14, 11, 'x');
    mux14.x[11] = new ConnectionNode(&mux8, 8, 'x');

    mux8.x[9] = new ConnectionNode(&mux14, 12, 'x');
    mux14.x[12] = new ConnectionNode(&mux8, 9, 'x');

    mux8.x[10] = new ConnectionNode(&mux15, 10, 'x');
    mux15.x[10] = new ConnectionNode(&mux8, 10, 'x');

    mux8.x[11] = new ConnectionNode(&mux15, 11, 'x');
    mux15.x[11] = new ConnectionNode(&mux8, 11, 'x');

    mux8.x[12] = new ConnectionNode(&mux15, 12, 'x');
    mux15.x[12] = new ConnectionNode(&mux8, 12, 'x');


    // All bidirectional connections for MUX9
    mux9.x[0] = new ConnectionNode(&mux11, 8, 'x');
    mux11.x[8] = new ConnectionNode(&mux9, 0, 'x');

    mux9.x[1] = new ConnectionNode(&mux12, 8, 'x');
    mux12.x[8] = new ConnectionNode(&mux9, 1, 'x');

    mux9.x[2] = new ConnectionNode(&mux13, 8, 'x');
    mux13.x[8] = new ConnectionNode(&mux9, 2, 'x');

    mux9.x[3] = new ConnectionNode(&mux14, 8, 'x');
    mux14.x[8] = new ConnectionNode(&mux9, 3, 'x');

    mux9.x[4] = new ConnectionNode(&mux15, 8, 'x');
    mux15.x[8] = new ConnectionNode(&mux9, 4, 'x');

    mux9.x[5] = new ConnectionNode(&mux16, 8, 'x');
    mux16.x[8] = new ConnectionNode(&mux9, 5, 'x');

    mux9.x[6] = new ConnectionNode(&mux17, 8, 'x');
    mux17.x[8] = new ConnectionNode(&mux9, 6, 'x');

    mux9.x[7] = new ConnectionNode(&mux18, 8, 'x');
    mux18.x[8] = new ConnectionNode(&mux9, 7, 'x');

    mux9.x[8] = new ConnectionNode(&mux16, 10, 'x');
    mux16.x[10] = new ConnectionNode(&mux9, 8, 'x');

    mux9.x[9] = new ConnectionNode(&mux16, 11, 'x');
    mux16.x[11] = new ConnectionNode(&mux9, 9, 'x');

    mux9.x[10] = new ConnectionNode(&mux16, 12, 'x');
    mux16.x[12] = new ConnectionNode(&mux9, 10, 'x');

    mux9.x[11] = new ConnectionNode(&mux17, 10, 'x');
    mux17.x[10] = new ConnectionNode(&mux9, 11, 'x');

    mux9.x[12] = new ConnectionNode(&mux17, 11, 'x');
    mux17.x[11] = new ConnectionNode(&mux9, 12, 'x');


    // All bidirectional connections for MUX10
    mux10.x[0] = new ConnectionNode(&mux11, 9, 'x');
    mux11.x[9] = new ConnectionNode(&mux10, 0, 'x');

    mux10.x[1] = new ConnectionNode(&mux12, 9, 'x');
    mux12.x[9] = new ConnectionNode(&mux10, 1, 'x');

    mux10.x[2] = new ConnectionNode(&mux13, 9, 'x');
    mux13.x[9] = new ConnectionNode(&mux10, 2, 'x');

    mux10.x[3] = new ConnectionNode(&mux14, 9, 'x');
    mux14.x[9] = new ConnectionNode(&mux10, 3, 'x');

    mux10.x[4] = new ConnectionNode(&mux15, 9, 'x');
    mux15.x[9] = new ConnectionNode(&mux10, 4, 'x');

    mux10.x[5] = new ConnectionNode(&mux16, 9, 'x');
    mux16.x[9] = new ConnectionNode(&mux10, 5, 'x');

    mux10.x[6] = new ConnectionNode(&mux17, 9, 'x');
    mux17.x[9] = new ConnectionNode(&mux10, 6, 'x');

    mux10.x[7] = new ConnectionNode(&mux18, 9, 'x');
    mux18.x[9] = new ConnectionNode(&mux10, 7, 'x');

    mux10.x[8] = new ConnectionNode(&mux17, 12, 'x');
    mux17.x[12] = new ConnectionNode(&mux10, 8, 'x');

    mux10.x[9] = new ConnectionNode(&mux18, 10, 'x');
    mux18.x[10] = new ConnectionNode(&mux10, 9, 'x');

    mux10.x[10] = new ConnectionNode(&mux18, 11, 'x');
    mux18.x[11] = new ConnectionNode(&mux10, 10, 'x');

    mux10.x[11] = new ConnectionNode(&mux18, 12, 'x');
    mux18.x[12] = new ConnectionNode(&mux10, 11, 'x');
    

    // All bidirectional connections for MUX11
    mux11.y[0] = new ConnectionNode(&main_breadboard, 1, 'p');
    main_breadboard.pin[1] = new ConnectionNode(&mux11, 0, 'y');

    mux11.y[1] = new ConnectionNode(&main_breadboard, 2, 'p');
    main_breadboard.pin[2] = new ConnectionNode(&mux11, 1, 'y');

    mux11.y[2] = new ConnectionNode(&main_breadboard, 3, 'p');
    main_breadboard.pin[3] = new ConnectionNode(&mux11, 2, 'y');

    mux11.y[3] = new ConnectionNode(&main_breadboard, 4, 'p');
    main_breadboard.pin[4] = new ConnectionNode(&mux11, 3, 'y');

    mux11.y[4] = new ConnectionNode(&main_breadboard, 5, 'p');
    main_breadboard.pin[5] = new ConnectionNode(&mux11, 4, 'y');

    mux11.y[5] = new ConnectionNode(&main_breadboard, 6, 'p');
    main_breadboard.pin[6] = new ConnectionNode(&mux11, 5, 'y');

    mux11.y[6] = new ConnectionNode(&main_breadboard, 7, 'p');
    main_breadboard.pin[7] = new ConnectionNode(&mux11, 6, 'y');

    mux11.y[7] = new ConnectionNode(&main_breadboard, 8, 'p');
    main_breadboard.pin[8] = new ConnectionNode(&mux11, 7, 'y');


    // All bidirectional connections for MUX12
    mux12.y[0] = new ConnectionNode(&main_breadboard, 9, 'p');
    main_breadboard.pin[9] = new ConnectionNode(&mux12, 0, 'y');
    
    mux12.y[1] = new ConnectionNode(&main_breadboard, 10, 'p');
    main_breadboard.pin[10] = new ConnectionNode(&mux12, 1, 'y');

    mux12.y[2] = new ConnectionNode(&main_breadboard, 11, 'p');
    main_breadboard.pin[11] = new ConnectionNode(&mux12, 2, 'y');

    mux12.y[3] = new ConnectionNode(&main_breadboard, 12, 'p');
    main_breadboard.pin[12] = new ConnectionNode(&mux12, 3, 'y');

    mux12.y[4] = new ConnectionNode(&main_breadboard, 13, 'p');
    main_breadboard.pin[13] = new ConnectionNode(&mux12, 4, 'y');

    mux12.y[5] = new ConnectionNode(&main_breadboard, 14, 'p');
    main_breadboard.pin[14] = new ConnectionNode(&mux12, 5, 'y');

    mux12.y[6] = new ConnectionNode(&main_breadboard, 15, 'p');
    main_breadboard.pin[15] = new ConnectionNode(&mux12, 6, 'y');

    mux12.y[7] = new ConnectionNode(&main_breadboard, 16, 'p');
    main_breadboard.pin[16] = new ConnectionNode(&mux12, 7, 'y');


    // All bidirectional connections for MUX13
    mux13.y[0] = new ConnectionNode(&main_breadboard, 17, 'p');
    main_breadboard.pin[17] = new ConnectionNode(&mux13, 0, 'y');

    mux13.y[1] = new ConnectionNode(&main_breadboard, 18, 'p');
    main_breadboard.pin[18] = new ConnectionNode(&mux13, 1, 'y');

    mux13.y[2] = new ConnectionNode(&main_breadboard, 19, 'p');
    main_breadboard.pin[19] = new ConnectionNode(&mux13, 2, 'y');

    mux13.y[3] = new ConnectionNode(&main_breadboard, 20, 'p');
    main_breadboard.pin[20] = new ConnectionNode(&mux13, 3, 'y');

    mux13.y[4] = new ConnectionNode(&main_breadboard, 21, 'p');
    main_breadboard.pin[21] = new ConnectionNode(&mux13, 4, 'y');

    mux13.y[5] = new ConnectionNode(&main_breadboard, 22, 'p');
    main_breadboard.pin[22] = new ConnectionNode(&mux13, 5, 'y');

    mux13.y[6] = new ConnectionNode(&main_breadboard, 23, 'p');
    main_breadboard.pin[23] = new ConnectionNode(&mux13, 6, 'y');

    mux13.y[7] = new ConnectionNode(&main_breadboard, 24, 'p');
    main_breadboard.pin[24] = new ConnectionNode(&mux13, 7, 'y');


    // All bidirectional connections for MUX14
    mux14.y[0] = new ConnectionNode(&main_breadboard, 25, 'p');
    main_breadboard.pin[25] = new ConnectionNode(&mux14, 0, 'y');

    mux14.y[1] = new ConnectionNode(&main_breadboard, 26, 'p');
    main_breadboard.pin[26] = new ConnectionNode(&mux14, 1, 'y');

    mux14.y[2] = new ConnectionNode(&main_breadboard, 27, 'p');
    main_breadboard.pin[27] = new ConnectionNode(&mux14, 2, 'y');

    mux14.y[3] = new ConnectionNode(&main_breadboard, 28, 'p');
    main_breadboard.pin[28] = new ConnectionNode(&mux14, 3, 'y');

    mux14.y[4] = new ConnectionNode(&main_breadboard, 29, 'p');
    main_breadboard.pin[29] = new ConnectionNode(&mux14, 4, 'y');

    mux14.y[5] = new ConnectionNode(&main_breadboard, 30, 'p');
    main_breadboard.pin[30] = new ConnectionNode(&mux14, 5, 'y');

    mux14.y[6] = new ConnectionNode(&main_breadboard, 31, 'p');
    main_breadboard.pin[31] = new ConnectionNode(&mux14, 6, 'y');

    mux14.y[7] = new ConnectionNode(&main_breadboard, 32, 'p');
    main_breadboard.pin[32] = new ConnectionNode(&mux14, 7, 'y');


    // All bidirectional connections for MUX15
    mux15.y[0] = new ConnectionNode(&main_breadboard, 33, 'p');
    main_breadboard.pin[33] = new ConnectionNode(&mux15, 0, 'y');

    mux15.y[1] = new ConnectionNode(&main_breadboard, 34, 'p');
    main_breadboard.pin[34] = new ConnectionNode(&mux15, 1, 'y');

    mux15.y[2] = new ConnectionNode(&main_breadboard, 35, 'p');
    main_breadboard.pin[35] = new ConnectionNode(&mux15, 2, 'y');

    mux15.y[3] = new ConnectionNode(&main_breadboard, 36, 'p');
    main_breadboard.pin[36] = new ConnectionNode(&mux15, 3, 'y');

    mux15.y[4] = new ConnectionNode(&main_breadboard, 37, 'p');
    main_breadboard.pin[37] = new ConnectionNode(&mux15, 4, 'y');

    mux15.y[5] = new ConnectionNode(&main_breadboard, 38, 'p');
    main_breadboard.pin[38] = new ConnectionNode(&mux15, 5, 'y');

    mux15.y[6] = new ConnectionNode(&main_breadboard, 39, 'p');
    main_breadboard.pin[39] = new ConnectionNode(&mux15, 6, 'y');

    mux15.y[7] = new ConnectionNode(&main_breadboard, 40, 'p');
    main_breadboard.pin[40] = new ConnectionNode(&mux15, 7, 'y');


    // All bidirectional connections for MUX16
    mux16.y[0] = new ConnectionNode(&main_breadboard, 41, 'p');
    main_breadboard.pin[41] = new ConnectionNode(&mux16, 0, 'y');

    mux16.y[1] = new ConnectionNode(&main_breadboard, 42, 'p');
    main_breadboard.pin[42] = new ConnectionNode(&mux16, 1, 'y');

    mux16.y[2] = new ConnectionNode(&main_breadboard, 43, 'p');
    main_breadboard.pin[43] = new ConnectionNode(&mux16, 2, 'y');

    mux16.y[3] = new ConnectionNode(&main_breadboard, 44, 'p');
    main_breadboard.pin[44] = new ConnectionNode(&mux16, 3, 'y');

    mux16.y[4] = new ConnectionNode(&main_breadboard, 45, 'p');
    main_breadboard.pin[45] = new ConnectionNode(&mux16, 4, 'y');

    mux16.y[5] = new ConnectionNode(&main_breadboard, 46, 'p');
    main_breadboard.pin[46] = new ConnectionNode(&mux16, 5, 'y');

    mux16.y[6] = new ConnectionNode(&main_breadboard, 47, 'p');
    main_breadboard.pin[47] = new ConnectionNode(&mux16, 6, 'y');

    mux16.y[7] = new ConnectionNode(&main_breadboard, 48, 'p');
    main_breadboard.pin[48] = new ConnectionNode(&mux16, 7, 'y');


    // All bidirectional connections for MUX17
    mux17.y[0] = new ConnectionNode(&main_breadboard, 49, 'p');
    main_breadboard.pin[49] = new ConnectionNode(&mux17, 0, 'y');

    mux17.y[1] = new ConnectionNode(&main_breadboard, 50, 'p');
    main_breadboard.pin[50] = new ConnectionNode(&mux17, 1, 'y');

    mux17.y[2] = new ConnectionNode(&main_breadboard, 51, 'p');
    main_breadboard.pin[51] = new ConnectionNode(&mux17, 2, 'y');

    mux17.y[3] = new ConnectionNode(&main_breadboard, 52, 'p');
    main_breadboard.pin[52] = new ConnectionNode(&mux17, 3, 'y');

    mux17.y[4] = new ConnectionNode(&main_breadboard, 53, 'p');
    main_breadboard.pin[53] = new ConnectionNode(&mux17, 4, 'y');

    mux17.y[5] = new ConnectionNode(&main_breadboard, 54, 'p');
    main_breadboard.pin[54] = new ConnectionNode(&mux17, 5, 'y');

    mux17.y[6] = new ConnectionNode(&main_breadboard, 55, 'p');
    main_breadboard.pin[55] = new ConnectionNode(&mux17, 6, 'y');

    mux17.y[7] = new ConnectionNode(&main_breadboard, 56, 'p');
    main_breadboard.pin[56] = new ConnectionNode(&mux17, 7, 'y');

    
    // All bidirectional connections for MUX18
    mux18.y[0] = new ConnectionNode(&main_breadboard, 57, 'p');
    main_breadboard.pin[57] = new ConnectionNode(&mux18, 0, 'y');

    mux18.y[1] = new ConnectionNode(&main_breadboard, 58, 'p');
    main_breadboard.pin[58] = new ConnectionNode(&mux18, 1, 'y');

    mux18.y[2] = new ConnectionNode(&main_breadboard, 59, 'p');
    main_breadboard.pin[59] = new ConnectionNode(&mux18, 2, 'y');

    mux18.y[3] = new ConnectionNode(&main_breadboard, 60, 'p');
    main_breadboard.pin[60] = new ConnectionNode(&mux18, 3, 'y');

    mux18.y[4] = new ConnectionNode(&main_breadboard, 61, 'p');
    main_breadboard.pin[61] = new ConnectionNode(&mux18, 4, 'y');

    mux18.y[5] = new ConnectionNode(&main_breadboard, 62, 'p');
    main_breadboard.pin[62] = new ConnectionNode(&mux18, 5, 'y');

    mux18.y[6] = new ConnectionNode(&main_breadboard, 63, 'p');
    main_breadboard.pin[63] = new ConnectionNode(&mux18, 6, 'y');

    mux18.y[7] = new ConnectionNode(&main_breadboard, 64, 'p');
    main_breadboard.pin[64] = new ConnectionNode(&mux18, 7, 'y');
    

    cout << "MUX1 connections:\n";
    mux1.printConnections();
    cout << "\n";

    cout << "MUX2 connections:\n";
    mux2.printConnections();
    cout << "\n";

    cout << "Breadboard1 connections:\n";
    main_breadboard.printConnections();
    cout << "\n";

    cout << "Checking bidirectional connection between mux1.y[0] and breadboard1.pin[10]: ";
    cout << (checkBidirectionalConnection(mux1, 'y', 0, main_breadboard, 'p', 10) ? "true" : "false") << endl;

    // MCU breadbaord pin 1->main breadboard pin 1
    // 1. MUX1: setconnection ( Y0 - X0 )
    // 2. MUX11 : setconnection(Y0 - X0)

    // MCU breadbaord pin 1->main breadboard pin 2
    // 1. MUX1: setconnection ( Y0 - X0 )
    
    // MCU breadbaord pin 1->main breadboard pin 9
    // 1. MUX1: setconnection ( Y0 - X1 )
    // 2. MUX12 : setconnection(Y0 - X0)

    // Example: Connecting MUX1's y[0] to Breadboard1's pin[10]
    int numVertices = 18 * 24 + 1 * 64;
    Graph g(numVertices);                        
    int srcVertex1 = getGraphVertexID(&mux1, 'x', 0);
    int destVertex1 = getGraphVertexID(&mux1, 'y', 0); // 'p' is arbitrary since Breadboard doesn't use 'x' or 'y'
    
    int srcVertex2 = getGraphVertexID(&mux1, 'x', 0);
    int destVertex2 = getGraphVertexID(&mux1, 'y', 1);

    int srcVertex3 = getGraphVertexID(&mux1, 'x', 0);
    int destVertex3 = getGraphVertexID(&mux1, 'y', 2);

    int srcVertex4 = getGraphVertexID(&mux1, 'x', 0);
    int destVertex4 = getGraphVertexID(&mux1, 'y', 3);

    int srcVertex5 = getGraphVertexID(&mux1, 'x', 0);
    int destVertex5 = getGraphVertexID(&main_breadboard, 'y', 4);

    g.addEdge(srcVertex1, destVertex1);
    g.addEdge(srcVertex2, destVertex2);
    g.addEdge(srcVertex3, destVertex3);
    g.addEdge(srcVertex4, destVertex4);
    g.addEdge(srcVertex5, destVertex5);

    g.DFS(srcVertex1);
    
    return 0;
}


