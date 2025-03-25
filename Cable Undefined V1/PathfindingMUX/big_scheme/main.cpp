#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <string>


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
    unordered_set<int> globalUsedPins; // Global tracking of used pins

public:
    Graph(int vertices) : numVertices(vertices), adjLists(new list<int>[vertices]), visited(new bool[vertices]()) {}

    ~Graph()
    {
        delete[] adjLists;
        delete[] visited;
    }

    void addEdge(int src, int dest)
    {
        adjLists[src].push_back(dest);
        adjLists[dest].push_back(src);
    }

    bool isSpecialPin(int pin)
    {
        // Assuming MainBreadboard and MCUBreadboard have specific ID ranges based on your setup
        int mainBreadboardStart = 18 * 24, mainBreadboardEnd = 18 * 24 + 63;     // Adjust these based on actual ranges
        int mcuBreadboardStart = 18 * 24 + 64, mcuBreadboardEnd = 18 * 24 + 127; // Adjust these based on actual ranges

        return (pin >= mainBreadboardStart && pin <= mainBreadboardEnd) ||
               (pin >= mcuBreadboardStart && pin <= mcuBreadboardEnd);
    }

    vector<int> findPathBFS(int startVertex, int endVertex)
    {
        fill(visited, visited + numVertices, false); // Reset visited status
        queue<int> q;
        unordered_map<int, int> parent; // To store the path
        vector<int> path;

        visited[startVertex] = true;
        q.push(startVertex);
        bool found = false;

        while (!q.empty() && !found)
        {
            int current = q.front();
            q.pop();

            for (int adjVertex : adjLists[current])
            {
                if (!visited[adjVertex] && (globalUsedPins.find(adjVertex) == globalUsedPins.end() || isSpecialPin(adjVertex)))
                {
                    parent[adjVertex] = current; // Track the path
                    visited[adjVertex] = true;
                    q.push(adjVertex);

                    if (adjVertex == endVertex)
                    {
                        found = true;
                        break; // Stop BFS when endVertex is found
                    }
                }
            }
        }

        if (!found)
        {
            cout << "No path found between " << startVertex << " and " << endVertex << endl;
            return path; // Empty if no path found
        }

        // Reconstruct and reserve the path
        for (int at = endVertex; at != startVertex; at = parent[at])
        {
            path.push_back(at);
            if (!isSpecialPin(at))
            {
                globalUsedPins.insert(at); // Mark as used globally, excluding special pins
            }
        }
        path.push_back(startVertex); // Add start vertex at the end
        if (!isSpecialPin(startVertex))
        {
            globalUsedPins.insert(startVertex); // Mark as used globally, excluding special pins
        }

        reverse(path.begin(), path.end()); // Reverse to get the correct order from start to end
        return path;
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
        if (deviceId == 19) // Main breadboard
        {
            return 18 * 24 + pinIndex;
        }

        if (deviceId == 20) // MCU breadboard
        {
            return 18 * 24 + 64 + pinIndex;
        }
    }
    return -1; // Error case
}

string printDeviceSpecifications(int vertexID)
{
    int numMultiplexers = 18;    // Assuming there are 18 multiplexers before the breadboards
    int multiplexerPins = 24;    // Assuming each multiplexer has 24 pins
    int mainBreadboardPins = 64; // Assuming the main breadboard has 64 pins

    if (vertexID < numMultiplexers * multiplexerPins)
    {
        // It's a multiplexer
        int deviceId = vertexID / multiplexerPins;
        int pinIndex = vertexID % multiplexerPins;
        char type = pinIndex < 16 ? 'x' : 'y';
        pinIndex = pinIndex < 16 ? pinIndex : pinIndex - 16; // Adjust pinIndex for 'y' type
        cout << " -> MUX" << deviceId + 1 << " " << type << pinIndex;
        return " -> MUX" + to_string(deviceId + 1) + " " + type + to_string(pinIndex);
    }
    else if (vertexID < numMultiplexers * multiplexerPins + mainBreadboardPins)
    {
        // It's the main breadboard
        int pinIndex = vertexID - (numMultiplexers * multiplexerPins);
        cout << " -> MainBreadboard " << pinIndex + 1; // Adjust pinIndex to start from 1 for better readability
        return " -> MainBreadboard " + to_string(pinIndex + 1);
    }
    else
    {
        // It's the MCU breadboard
        int pinIndex = vertexID - (numMultiplexers * multiplexerPins + mainBreadboardPins);
        cout << " -> MCUBreadboard " << pinIndex + 1; // Adjust pinIndex to start from 1 for better readability
        return " -> MCUBreadboard " + to_string(pinIndex + 1);
    }
}

struct PathRequest
{
    Device *startDevice;
    char startType;
    int startPin;
    Device *endDevice;
    char endType;
    int endPin;

    PathRequest(Device *startDevice, char startType, int startPin, Device *endDevice, char endType, int endPin)
        : startDevice(startDevice), startType(startType), startPin(startPin), endDevice(endDevice), endType(endType), endPin(endPin) {}
};

int findAndPrintPath(Graph &graph, const PathRequest &request)
{
    int startVertex = getGraphVertexID(request.startDevice, request.startType, request.startPin);
    int endVertex = getGraphVertexID(request.endDevice, request.endType, request.endPin);

    vector<int> path = graph.findPathBFS(startVertex, endVertex);

    if (!path.empty())
    {
        // Print the path in a txt file and in the console
        ofstream found_paths_file("found_paths.txt", ios::app);
        if (!found_paths_file)
        {
            cerr << "Error: unable to open output file" << endl;
            return -1;
        }
        found_paths_file << "Path from ";
        string from = printDeviceSpecifications(startVertex);
        found_paths_file << from;

        found_paths_file << " to ";
        string to = printDeviceSpecifications(endVertex);
        found_paths_file << to;

        found_paths_file << " is: \n";
        for (int vertex : path)
        {
            string device = printDeviceSpecifications(vertex);
            found_paths_file << device;
        }
        found_paths_file << "\n\n";
        found_paths_file.close();

        cout << "\nPath from ";
        from  = printDeviceSpecifications(startVertex);
        cout << " to ";
        to = printDeviceSpecifications(endVertex);
        cout << " is: \n";
        for (int vertex : path)
        {
            string device = printDeviceSpecifications(vertex);
        }
        cout << "\n\n";
        return 1;
    }
    else
    {
        // Print the not found path in a txt file and in the console
        ofstream not_found_paths_file("not_found_paths.txt", ios::app);
        if (!not_found_paths_file)
        {
            cerr << "Error: unable to open output file" << endl;
            return -1;
        }
        not_found_paths_file << "No path found from ";
        string from = printDeviceSpecifications(startVertex);
        not_found_paths_file << from;
        not_found_paths_file << " to ";
        string to = printDeviceSpecifications(endVertex);
        not_found_paths_file << to;
        not_found_paths_file << ".\n\n";
        
        
        cout << "\nNo path found from ";
        from = printDeviceSpecifications(startVertex);
        cout << " to ";
        to = printDeviceSpecifications(endVertex);
        cout << ".\n\n";
        return 0;
    }
}

int main() {
    Multiplexer mux1(0), mux2(1), mux3(2), mux4(3), mux5(4), mux6(5), mux7(6), mux8(7), mux9(8), mux10(9), mux11(10), mux12(11), mux13(12), mux14(13), mux15(14), mux16(15), mux17(16), mux18(17);
    Breadboard main_breadboard(19), mcu_breadboard(20);

    Multiplexer all_muxes[18] = {mux1, mux2, mux3, mux4, mux5, mux6, mux7, mux8, mux9, mux10, mux11, mux12, mux13, mux14, mux15, mux16, mux17, mux18};
    

    cout << "Checking bidirectional connection between mcu_breadboard[0] and mux1.y[0] ";
    cout << (checkBidirectionalConnection(mcu_breadboard, 'p', 0, mux1, 'y', 0) ? "true" : "false") << endl;

    // Example: Connecting MUX1's y[0] to Breadboard1's pin[10]
    int numVertices = 18 * 24 + 1 * 64 + 1 * 40;
    Graph g(numVertices);                        

    // Add edges to the graph every X to Y connection in the muxes
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 16; j++){
            for(int k = 0; k < 8; k++){
                int srcVertex = getGraphVertexID(&all_muxes[i], 'x', j);
                int destVertex = getGraphVertexID(&all_muxes[i], 'y', k);
                g.addEdge(srcVertex, destVertex);
            }
        }
    }

    // MUX1 pins edge connections
    g.addEdge(getGraphVertexID(&mux1, 'x', 0), getGraphVertexID(&mux11, 'x', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 1), getGraphVertexID(&mux12, 'x', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 2), getGraphVertexID(&mux13, 'x', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 3), getGraphVertexID(&mux14, 'x', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 4), getGraphVertexID(&mux15, 'x', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 5), getGraphVertexID(&mux16, 'x', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 6), getGraphVertexID(&mux17, 'x', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 7), getGraphVertexID(&mux18, 'x', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 8), getGraphVertexID(&mux6, 'y', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 9), getGraphVertexID(&mux7, 'y', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 10), getGraphVertexID(&mux8, 'y', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 11), getGraphVertexID(&mux9, 'y', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 12), getGraphVertexID(&mux10, 'y', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 13), getGraphVertexID(&mux6, 'y', 5));
    g.addEdge(getGraphVertexID(&mux1, 'x', 14), getGraphVertexID(&mux6, 'y', 6));
    g.addEdge(getGraphVertexID(&mux1, 'x', 15), getGraphVertexID(&mux6, 'y', 7));
    g.addEdge(getGraphVertexID(&mux1, 'y', 0), getGraphVertexID(&mcu_breadboard, 'p', 0));
    g.addEdge(getGraphVertexID(&mux1, 'y', 1), getGraphVertexID(&mcu_breadboard, 'p', 1));
    g.addEdge(getGraphVertexID(&mux1, 'y', 2), getGraphVertexID(&mcu_breadboard, 'p', 2));
    g.addEdge(getGraphVertexID(&mux1, 'y', 3), getGraphVertexID(&mcu_breadboard, 'p', 3));
    g.addEdge(getGraphVertexID(&mux1, 'y', 4), getGraphVertexID(&mcu_breadboard, 'p', 4));
    g.addEdge(getGraphVertexID(&mux1, 'y', 5), getGraphVertexID(&mcu_breadboard, 'p', 5));
    g.addEdge(getGraphVertexID(&mux1, 'y', 6), getGraphVertexID(&mcu_breadboard, 'p', 6));
    g.addEdge(getGraphVertexID(&mux1, 'y', 7), getGraphVertexID(&mcu_breadboard, 'p', 7));

    // MUX2 pins edge connections
    g.addEdge(getGraphVertexID(&mux2, 'x', 0), getGraphVertexID(&mux11, 'x', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 1), getGraphVertexID(&mux12, 'x', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 2), getGraphVertexID(&mux13, 'x', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 3), getGraphVertexID(&mux14, 'x', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 4), getGraphVertexID(&mux15, 'x', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 5), getGraphVertexID(&mux16, 'x', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 6), getGraphVertexID(&mux17, 'x', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 7), getGraphVertexID(&mux18, 'x', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 8), getGraphVertexID(&mux6, 'y', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 9), getGraphVertexID(&mux7, 'y', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 10), getGraphVertexID(&mux8, 'y', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 11), getGraphVertexID(&mux9, 'y', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 12), getGraphVertexID(&mux10, 'y', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 13), getGraphVertexID(&mux7, 'y', 5));
    g.addEdge(getGraphVertexID(&mux2, 'x', 14), getGraphVertexID(&mux7, 'y', 6));
    g.addEdge(getGraphVertexID(&mux2, 'x', 15), getGraphVertexID(&mux7, 'y', 7));
    g.addEdge(getGraphVertexID(&mux2, 'y', 0), getGraphVertexID(&mcu_breadboard, 'p', 8));
    g.addEdge(getGraphVertexID(&mux2, 'y', 1), getGraphVertexID(&mcu_breadboard, 'p', 9));
    g.addEdge(getGraphVertexID(&mux2, 'y', 2), getGraphVertexID(&mcu_breadboard, 'p', 10));
    g.addEdge(getGraphVertexID(&mux2, 'y', 3), getGraphVertexID(&mcu_breadboard, 'p', 11));
    g.addEdge(getGraphVertexID(&mux2, 'y', 4), getGraphVertexID(&mcu_breadboard, 'p', 12));
    g.addEdge(getGraphVertexID(&mux2, 'y', 5), getGraphVertexID(&mcu_breadboard, 'p', 13));
    g.addEdge(getGraphVertexID(&mux2, 'y', 6), getGraphVertexID(&mcu_breadboard, 'p', 14));
    g.addEdge(getGraphVertexID(&mux2, 'y', 7), getGraphVertexID(&mcu_breadboard, 'p', 15));

    // MUX3 pins edge connections
    g.addEdge(getGraphVertexID(&mux3, 'x', 0), getGraphVertexID(&mux11, 'x', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 1), getGraphVertexID(&mux12, 'x', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 2), getGraphVertexID(&mux13, 'x', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 3), getGraphVertexID(&mux14, 'x', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 4), getGraphVertexID(&mux15, 'x', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 5), getGraphVertexID(&mux16, 'x', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 6), getGraphVertexID(&mux17, 'x', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 7), getGraphVertexID(&mux18, 'x', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 8), getGraphVertexID(&mux6, 'y', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 9), getGraphVertexID(&mux7, 'y', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 10), getGraphVertexID(&mux8, 'y', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 11), getGraphVertexID(&mux9, 'y', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 12), getGraphVertexID(&mux10, 'y', 2));
    g.addEdge(getGraphVertexID(&mux3, 'x', 13), getGraphVertexID(&mux8, 'y', 5));
    g.addEdge(getGraphVertexID(&mux3, 'x', 14), getGraphVertexID(&mux8, 'y', 6));
    g.addEdge(getGraphVertexID(&mux3, 'x', 15), getGraphVertexID(&mux8, 'y', 7));
    g.addEdge(getGraphVertexID(&mux3, 'y', 0), getGraphVertexID(&mcu_breadboard, 'p', 16));
    g.addEdge(getGraphVertexID(&mux3, 'y', 1), getGraphVertexID(&mcu_breadboard, 'p', 17));
    g.addEdge(getGraphVertexID(&mux3, 'y', 2), getGraphVertexID(&mcu_breadboard, 'p', 18));
    g.addEdge(getGraphVertexID(&mux3, 'y', 3), getGraphVertexID(&mcu_breadboard, 'p', 19));
    g.addEdge(getGraphVertexID(&mux3, 'y', 4), getGraphVertexID(&mcu_breadboard, 'p', 20));
    g.addEdge(getGraphVertexID(&mux3, 'y', 5), getGraphVertexID(&mcu_breadboard, 'p', 21));
    g.addEdge(getGraphVertexID(&mux3, 'y', 6), getGraphVertexID(&mcu_breadboard, 'p', 22));
    g.addEdge(getGraphVertexID(&mux3, 'y', 7), getGraphVertexID(&mcu_breadboard, 'p', 23));

    // MUX4 pins edge connections
    g.addEdge(getGraphVertexID(&mux4, 'x', 0), getGraphVertexID(&mux11, 'x', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 1), getGraphVertexID(&mux12, 'x', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 2), getGraphVertexID(&mux13, 'x', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 3), getGraphVertexID(&mux14, 'x', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 4), getGraphVertexID(&mux15, 'x', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 5), getGraphVertexID(&mux16, 'x', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 6), getGraphVertexID(&mux17, 'x', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 7), getGraphVertexID(&mux18, 'x', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 8), getGraphVertexID(&mux6, 'y', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 9), getGraphVertexID(&mux7, 'y', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 10), getGraphVertexID(&mux8, 'y', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 11), getGraphVertexID(&mux9, 'y', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 12), getGraphVertexID(&mux10, 'y', 3));
    g.addEdge(getGraphVertexID(&mux4, 'x', 13), getGraphVertexID(&mux9, 'y', 5));
    g.addEdge(getGraphVertexID(&mux4, 'x', 14), getGraphVertexID(&mux9, 'y', 6));
    g.addEdge(getGraphVertexID(&mux4, 'x', 15), getGraphVertexID(&mux9, 'y', 7));
    g.addEdge(getGraphVertexID(&mux4, 'y', 0), getGraphVertexID(&mcu_breadboard, 'p', 24));
    g.addEdge(getGraphVertexID(&mux4, 'y', 1), getGraphVertexID(&mcu_breadboard, 'p', 25));
    g.addEdge(getGraphVertexID(&mux4, 'y', 2), getGraphVertexID(&mcu_breadboard, 'p', 26));
    g.addEdge(getGraphVertexID(&mux4, 'y', 3), getGraphVertexID(&mcu_breadboard, 'p', 27));
    g.addEdge(getGraphVertexID(&mux4, 'y', 4), getGraphVertexID(&mcu_breadboard, 'p', 28));
    g.addEdge(getGraphVertexID(&mux4, 'y', 5), getGraphVertexID(&mcu_breadboard, 'p', 29));
    g.addEdge(getGraphVertexID(&mux4, 'y', 6), getGraphVertexID(&mcu_breadboard, 'p', 30));
    g.addEdge(getGraphVertexID(&mux4, 'y', 7), getGraphVertexID(&mcu_breadboard, 'p', 31));

    // MUX5 pins edge connections
    g.addEdge(getGraphVertexID(&mux5, 'x', 0), getGraphVertexID(&mux11, 'x', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 1), getGraphVertexID(&mux12, 'x', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 2), getGraphVertexID(&mux13, 'x', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 3), getGraphVertexID(&mux14, 'x', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 4), getGraphVertexID(&mux15, 'x', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 5), getGraphVertexID(&mux16, 'x', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 6), getGraphVertexID(&mux17, 'x', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 7), getGraphVertexID(&mux18, 'x', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 8), getGraphVertexID(&mux6, 'y', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 9), getGraphVertexID(&mux7, 'y', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 10), getGraphVertexID(&mux8, 'y', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 11), getGraphVertexID(&mux9, 'y', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 12), getGraphVertexID(&mux10, 'y', 4));
    g.addEdge(getGraphVertexID(&mux5, 'x', 13), getGraphVertexID(&mux10, 'y', 5));
    g.addEdge(getGraphVertexID(&mux5, 'x', 14), getGraphVertexID(&mux10, 'y', 6));
    g.addEdge(getGraphVertexID(&mux5, 'x', 15), getGraphVertexID(&mux10, 'y', 7));
    g.addEdge(getGraphVertexID(&mux5, 'y', 0), getGraphVertexID(&mcu_breadboard, 'p', 32));
    g.addEdge(getGraphVertexID(&mux5, 'y', 1), getGraphVertexID(&mcu_breadboard, 'p', 33));
    g.addEdge(getGraphVertexID(&mux5, 'y', 2), getGraphVertexID(&mcu_breadboard, 'p', 34));
    g.addEdge(getGraphVertexID(&mux5, 'y', 3), getGraphVertexID(&mcu_breadboard, 'p', 35));
    g.addEdge(getGraphVertexID(&mux5, 'y', 4), getGraphVertexID(&mcu_breadboard, 'p', 36));
    g.addEdge(getGraphVertexID(&mux5, 'y', 5), getGraphVertexID(&mcu_breadboard, 'p', 37));
    g.addEdge(getGraphVertexID(&mux5, 'y', 6), getGraphVertexID(&mcu_breadboard, 'p', 38));
    g.addEdge(getGraphVertexID(&mux5, 'y', 7), getGraphVertexID(&mcu_breadboard, 'p', 39));

    // MUX6 pins edge connections
    g.addEdge(getGraphVertexID(&mux6, 'x', 0), getGraphVertexID(&mux11, 'x', 5));
    g.addEdge(getGraphVertexID(&mux6, 'x', 1), getGraphVertexID(&mux12, 'x', 5));
    g.addEdge(getGraphVertexID(&mux6, 'x', 2), getGraphVertexID(&mux13, 'x', 5));
    g.addEdge(getGraphVertexID(&mux6, 'x', 3), getGraphVertexID(&mux14, 'x', 5));
    g.addEdge(getGraphVertexID(&mux6, 'x', 4), getGraphVertexID(&mux15, 'x', 5));
    g.addEdge(getGraphVertexID(&mux6, 'x', 5), getGraphVertexID(&mux16, 'x', 5));
    g.addEdge(getGraphVertexID(&mux6, 'x', 6), getGraphVertexID(&mux17, 'x', 5));
    g.addEdge(getGraphVertexID(&mux6, 'x', 7), getGraphVertexID(&mux18, 'x', 5));
    g.addEdge(getGraphVertexID(&mux6, 'x', 8), getGraphVertexID(&mux11, 'x', 10));
    g.addEdge(getGraphVertexID(&mux6, 'x', 9), getGraphVertexID(&mux11, 'x', 11));
    g.addEdge(getGraphVertexID(&mux6, 'x', 10), getGraphVertexID(&mux11, 'x', 12));
    g.addEdge(getGraphVertexID(&mux6, 'x', 11), getGraphVertexID(&mux12, 'x', 10));
    g.addEdge(getGraphVertexID(&mux6, 'x', 12), getGraphVertexID(&mux12, 'x', 11));

    // MUX7 pins edge connections
    g.addEdge(getGraphVertexID(&mux7, 'x', 0), getGraphVertexID(&mux11, 'x', 6));
    g.addEdge(getGraphVertexID(&mux7, 'x', 1), getGraphVertexID(&mux12, 'x', 6));
    g.addEdge(getGraphVertexID(&mux7, 'x', 2), getGraphVertexID(&mux13, 'x', 6));
    g.addEdge(getGraphVertexID(&mux7, 'x', 3), getGraphVertexID(&mux14, 'x', 6));
    g.addEdge(getGraphVertexID(&mux7, 'x', 4), getGraphVertexID(&mux15, 'x', 6));
    g.addEdge(getGraphVertexID(&mux7, 'x', 5), getGraphVertexID(&mux16, 'x', 6));
    g.addEdge(getGraphVertexID(&mux7, 'x', 6), getGraphVertexID(&mux17, 'x', 6));
    g.addEdge(getGraphVertexID(&mux7, 'x', 7), getGraphVertexID(&mux18, 'x', 6));
    g.addEdge(getGraphVertexID(&mux7, 'x', 8), getGraphVertexID(&mux12, 'x', 12));
    g.addEdge(getGraphVertexID(&mux7, 'x', 9), getGraphVertexID(&mux13, 'x', 10));
    g.addEdge(getGraphVertexID(&mux7, 'x', 10), getGraphVertexID(&mux13, 'x', 11));
    g.addEdge(getGraphVertexID(&mux7, 'x', 11), getGraphVertexID(&mux13, 'x', 12));
    g.addEdge(getGraphVertexID(&mux7, 'x', 12), getGraphVertexID(&mux14, 'x', 10));

    // MUX8 pins edge connections
    g.addEdge(getGraphVertexID(&mux8, 'x', 0), getGraphVertexID(&mux11, 'x', 7));
    g.addEdge(getGraphVertexID(&mux8, 'x', 1), getGraphVertexID(&mux12, 'x', 7));
    g.addEdge(getGraphVertexID(&mux8, 'x', 2), getGraphVertexID(&mux13, 'x', 7));
    g.addEdge(getGraphVertexID(&mux8, 'x', 3), getGraphVertexID(&mux14, 'x', 7));
    g.addEdge(getGraphVertexID(&mux8, 'x', 4), getGraphVertexID(&mux15, 'x', 7));
    g.addEdge(getGraphVertexID(&mux8, 'x', 5), getGraphVertexID(&mux16, 'x', 7));
    g.addEdge(getGraphVertexID(&mux8, 'x', 6), getGraphVertexID(&mux17, 'x', 7));
    g.addEdge(getGraphVertexID(&mux8, 'x', 7), getGraphVertexID(&mux18, 'x', 7));
    g.addEdge(getGraphVertexID(&mux8, 'x', 8), getGraphVertexID(&mux14, 'x', 11));
    g.addEdge(getGraphVertexID(&mux8, 'x', 9), getGraphVertexID(&mux14, 'x', 12));
    g.addEdge(getGraphVertexID(&mux8, 'x', 10), getGraphVertexID(&mux15, 'x', 10));
    g.addEdge(getGraphVertexID(&mux8, 'x', 11), getGraphVertexID(&mux15, 'x', 11));
    g.addEdge(getGraphVertexID(&mux8, 'x', 12), getGraphVertexID(&mux15, 'x', 12));

    // MUX9 pins edge connections
    g.addEdge(getGraphVertexID(&mux9, 'x', 0), getGraphVertexID(&mux11, 'x', 8));
    g.addEdge(getGraphVertexID(&mux9, 'x', 1), getGraphVertexID(&mux12, 'x', 8));
    g.addEdge(getGraphVertexID(&mux9, 'x', 2), getGraphVertexID(&mux13, 'x', 8));
    g.addEdge(getGraphVertexID(&mux9, 'x', 3), getGraphVertexID(&mux14, 'x', 8));
    g.addEdge(getGraphVertexID(&mux9, 'x', 4), getGraphVertexID(&mux15, 'x', 8));
    g.addEdge(getGraphVertexID(&mux9, 'x', 5), getGraphVertexID(&mux16, 'x', 8));
    g.addEdge(getGraphVertexID(&mux9, 'x', 6), getGraphVertexID(&mux17, 'x', 8));
    g.addEdge(getGraphVertexID(&mux9, 'x', 7), getGraphVertexID(&mux18, 'x', 8));
    g.addEdge(getGraphVertexID(&mux9, 'x', 8), getGraphVertexID(&mux16, 'x', 10));
    g.addEdge(getGraphVertexID(&mux9, 'x', 9), getGraphVertexID(&mux16, 'x', 11));
    g.addEdge(getGraphVertexID(&mux9, 'x', 10), getGraphVertexID(&mux16, 'x', 12));
    g.addEdge(getGraphVertexID(&mux9, 'x', 11), getGraphVertexID(&mux17, 'x', 10));
    g.addEdge(getGraphVertexID(&mux9, 'x', 12), getGraphVertexID(&mux17, 'x', 11));

    // MUX10 pins edge connections
    g.addEdge(getGraphVertexID(&mux10, 'x', 0), getGraphVertexID(&mux11, 'x', 9));
    g.addEdge(getGraphVertexID(&mux10, 'x', 1), getGraphVertexID(&mux12, 'x', 9));
    g.addEdge(getGraphVertexID(&mux10, 'x', 2), getGraphVertexID(&mux13, 'x', 9));
    g.addEdge(getGraphVertexID(&mux10, 'x', 3), getGraphVertexID(&mux14, 'x', 9));
    g.addEdge(getGraphVertexID(&mux10, 'x', 4), getGraphVertexID(&mux15, 'x', 9));
    g.addEdge(getGraphVertexID(&mux10, 'x', 5), getGraphVertexID(&mux16, 'x', 9));
    g.addEdge(getGraphVertexID(&mux10, 'x', 6), getGraphVertexID(&mux17, 'x', 9));
    g.addEdge(getGraphVertexID(&mux10, 'x', 7), getGraphVertexID(&mux18, 'x', 9));
    g.addEdge(getGraphVertexID(&mux10, 'x', 8), getGraphVertexID(&mux17, 'x', 12));
    g.addEdge(getGraphVertexID(&mux10, 'x', 9), getGraphVertexID(&mux18, 'x', 10));
    g.addEdge(getGraphVertexID(&mux10, 'x', 10), getGraphVertexID(&mux18, 'x', 11));
    g.addEdge(getGraphVertexID(&mux10, 'x', 11), getGraphVertexID(&mux18, 'x', 12));

    // MUX11 pins edge connections
    // Connecting mux11 'y' outputs to main_breadboard 'p' pins, slots 0-7
    g.addEdge(getGraphVertexID(&mux11, 'y', 0), getGraphVertexID(&main_breadboard, 'p', 0));
    g.addEdge(getGraphVertexID(&mux11, 'y', 1), getGraphVertexID(&main_breadboard, 'p', 1));
    g.addEdge(getGraphVertexID(&mux11, 'y', 2), getGraphVertexID(&main_breadboard, 'p', 2));
    g.addEdge(getGraphVertexID(&mux11, 'y', 3), getGraphVertexID(&main_breadboard, 'p', 3));
    g.addEdge(getGraphVertexID(&mux11, 'y', 4), getGraphVertexID(&main_breadboard, 'p', 4));
    g.addEdge(getGraphVertexID(&mux11, 'y', 5), getGraphVertexID(&main_breadboard, 'p', 5));
    g.addEdge(getGraphVertexID(&mux11, 'y', 6), getGraphVertexID(&main_breadboard, 'p', 6));
    g.addEdge(getGraphVertexID(&mux11, 'y', 7), getGraphVertexID(&main_breadboard, 'p', 7));

    // MUX12 pins edge connections
    // Connecting mux12 'y' outputs to main_breadboard 'p' pins, slots 8-15
    g.addEdge(getGraphVertexID(&mux12, 'y', 0), getGraphVertexID(&main_breadboard, 'p', 8));
    g.addEdge(getGraphVertexID(&mux12, 'y', 1), getGraphVertexID(&main_breadboard, 'p', 9));
    g.addEdge(getGraphVertexID(&mux12, 'y', 2), getGraphVertexID(&main_breadboard, 'p', 10));
    g.addEdge(getGraphVertexID(&mux12, 'y', 3), getGraphVertexID(&main_breadboard, 'p', 11));
    g.addEdge(getGraphVertexID(&mux12, 'y', 4), getGraphVertexID(&main_breadboard, 'p', 12));
    g.addEdge(getGraphVertexID(&mux12, 'y', 5), getGraphVertexID(&main_breadboard, 'p', 13));
    g.addEdge(getGraphVertexID(&mux12, 'y', 6), getGraphVertexID(&main_breadboard, 'p', 14));
    g.addEdge(getGraphVertexID(&mux12, 'y', 7), getGraphVertexID(&main_breadboard, 'p', 15));

    // MUX13 pins edge connections
    // Connecting mux13 'y' outputs to main_breadboard 'p' pins, slots 16-23
    g.addEdge(getGraphVertexID(&mux13, 'y', 0), getGraphVertexID(&main_breadboard, 'p', 16));
    g.addEdge(getGraphVertexID(&mux13, 'y', 1), getGraphVertexID(&main_breadboard, 'p', 17));
    g.addEdge(getGraphVertexID(&mux13, 'y', 2), getGraphVertexID(&main_breadboard, 'p', 18));
    g.addEdge(getGraphVertexID(&mux13, 'y', 3), getGraphVertexID(&main_breadboard, 'p', 19));
    g.addEdge(getGraphVertexID(&mux13, 'y', 4), getGraphVertexID(&main_breadboard, 'p', 20));
    g.addEdge(getGraphVertexID(&mux13, 'y', 5), getGraphVertexID(&main_breadboard, 'p', 21));
    g.addEdge(getGraphVertexID(&mux13, 'y', 6), getGraphVertexID(&main_breadboard, 'p', 22));
    g.addEdge(getGraphVertexID(&mux13, 'y', 7), getGraphVertexID(&main_breadboard, 'p', 23));

    // MUX14 pins edge connections
    // Connecting mux14 'y' outputs to main_breadboard 'p' pins, slots 24-31
    g.addEdge(getGraphVertexID(&mux14, 'y', 0), getGraphVertexID(&main_breadboard, 'p', 24));
    g.addEdge(getGraphVertexID(&mux14, 'y', 1), getGraphVertexID(&main_breadboard, 'p', 25));
    g.addEdge(getGraphVertexID(&mux14, 'y', 2), getGraphVertexID(&main_breadboard, 'p', 26));
    g.addEdge(getGraphVertexID(&mux14, 'y', 3), getGraphVertexID(&main_breadboard, 'p', 27));
    g.addEdge(getGraphVertexID(&mux14, 'y', 4), getGraphVertexID(&main_breadboard, 'p', 28));
    g.addEdge(getGraphVertexID(&mux14, 'y', 5), getGraphVertexID(&main_breadboard, 'p', 29));
    g.addEdge(getGraphVertexID(&mux14, 'y', 6), getGraphVertexID(&main_breadboard, 'p', 30));
    g.addEdge(getGraphVertexID(&mux14, 'y', 7), getGraphVertexID(&main_breadboard, 'p', 31));

    // MUX15 pins edge connections
    // Connecting mux15 'y' outputs to main_breadboard 'p' pins, slots 32-39
    g.addEdge(getGraphVertexID(&mux15, 'y', 0), getGraphVertexID(&main_breadboard, 'p', 32));
    g.addEdge(getGraphVertexID(&mux15, 'y', 1), getGraphVertexID(&main_breadboard, 'p', 33));
    g.addEdge(getGraphVertexID(&mux15, 'y', 2), getGraphVertexID(&main_breadboard, 'p', 34));
    g.addEdge(getGraphVertexID(&mux15, 'y', 3), getGraphVertexID(&main_breadboard, 'p', 35));
    g.addEdge(getGraphVertexID(&mux15, 'y', 4), getGraphVertexID(&main_breadboard, 'p', 36));
    g.addEdge(getGraphVertexID(&mux15, 'y', 5), getGraphVertexID(&main_breadboard, 'p', 37));
    g.addEdge(getGraphVertexID(&mux15, 'y', 6), getGraphVertexID(&main_breadboard, 'p', 38));
    g.addEdge(getGraphVertexID(&mux15, 'y', 7), getGraphVertexID(&main_breadboard, 'p', 39));

    // MUX16 pins edge connections
    // Connecting mux16 'y' outputs to main_breadboard 'p' pins, slots 40-47
    // Connecting mux16 'y' outputs to main_breadboard 'p' pins, slots 40-47
    g.addEdge(getGraphVertexID(&mux16, 'y', 0), getGraphVertexID(&main_breadboard, 'p', 40));
    g.addEdge(getGraphVertexID(&mux16, 'y', 1), getGraphVertexID(&main_breadboard, 'p', 41));
    g.addEdge(getGraphVertexID(&mux16, 'y', 2), getGraphVertexID(&main_breadboard, 'p', 42));
    g.addEdge(getGraphVertexID(&mux16, 'y', 3), getGraphVertexID(&main_breadboard, 'p', 43));
    g.addEdge(getGraphVertexID(&mux16, 'y', 4), getGraphVertexID(&main_breadboard, 'p', 44));
    g.addEdge(getGraphVertexID(&mux16, 'y', 5), getGraphVertexID(&main_breadboard, 'p', 45));
    g.addEdge(getGraphVertexID(&mux16, 'y', 6), getGraphVertexID(&main_breadboard, 'p', 46));
    g.addEdge(getGraphVertexID(&mux16, 'y', 7), getGraphVertexID(&main_breadboard, 'p', 47));

    // MUX17 pins edge connections
    // Connecting mux17 'y' outputs to main_breadboard 'p' pins, slots 48-55
    g.addEdge(getGraphVertexID(&mux17, 'y', 0), getGraphVertexID(&main_breadboard, 'p', 48));
    g.addEdge(getGraphVertexID(&mux17, 'y', 1), getGraphVertexID(&main_breadboard, 'p', 49));
    g.addEdge(getGraphVertexID(&mux17, 'y', 2), getGraphVertexID(&main_breadboard, 'p', 50));
    g.addEdge(getGraphVertexID(&mux17, 'y', 3), getGraphVertexID(&main_breadboard, 'p', 51));
    g.addEdge(getGraphVertexID(&mux17, 'y', 4), getGraphVertexID(&main_breadboard, 'p', 52));
    g.addEdge(getGraphVertexID(&mux17, 'y', 5), getGraphVertexID(&main_breadboard, 'p', 53));
    g.addEdge(getGraphVertexID(&mux17, 'y', 6), getGraphVertexID(&main_breadboard, 'p', 54));
    g.addEdge(getGraphVertexID(&mux17, 'y', 7), getGraphVertexID(&main_breadboard, 'p', 55));

    // MUX18 pins edge connections
    // Connecting mux18 'y' outputs to main_breadboard 'p' pins, slots 56-63
    g.addEdge(getGraphVertexID(&mux18, 'y', 0), getGraphVertexID(&main_breadboard, 'p', 56));
    g.addEdge(getGraphVertexID(&mux18, 'y', 1), getGraphVertexID(&main_breadboard, 'p', 57));
    g.addEdge(getGraphVertexID(&mux18, 'y', 2), getGraphVertexID(&main_breadboard, 'p', 58));
    g.addEdge(getGraphVertexID(&mux18, 'y', 3), getGraphVertexID(&main_breadboard, 'p', 59));
    g.addEdge(getGraphVertexID(&mux18, 'y', 4), getGraphVertexID(&main_breadboard, 'p', 60));
    g.addEdge(getGraphVertexID(&mux18, 'y', 5), getGraphVertexID(&main_breadboard, 'p', 61));
    g.addEdge(getGraphVertexID(&mux18, 'y', 6), getGraphVertexID(&main_breadboard, 'p', 62));
    g.addEdge(getGraphVertexID(&mux18, 'y', 7), getGraphVertexID(&main_breadboard, 'p', 63));

    // MCU Breadboard pins edge connections
    g.addEdge(getGraphVertexID(&mux6, 'x', 13), getGraphVertexID(&mux10, 'x', 15));
    g.addEdge(getGraphVertexID(&mux7, 'x', 14), getGraphVertexID(&mux6, 'x', 14));
    g.addEdge(getGraphVertexID(&mux8, 'x', 14), getGraphVertexID(&mux6, 'x', 15));
    g.addEdge(getGraphVertexID(&mux10, 'x', 14), getGraphVertexID(&mux7, 'x', 13));
    g.addEdge(getGraphVertexID(&mux9, 'x', 14), getGraphVertexID(&mux7, 'x', 15));
    g.addEdge(getGraphVertexID(&mux10, 'x', 13), getGraphVertexID(&mux8, 'x', 13));
    g.addEdge(getGraphVertexID(&mux9, 'x', 15), getGraphVertexID(&mux8, 'x', 15));
    g.addEdge(getGraphVertexID(&mux10, 'x', 12), getGraphVertexID(&mux9, 'x', 13));

    vector<PathRequest> requests;
    // Iterate through all the pins on the main breadboard and create a path request for each pin
    for (size_t i = 0; i < 64; i++)
    {
        for (size_t j = 0; j < 40; j++)
        {
            requests.push_back(PathRequest(&main_breadboard, 'p', i, &mcu_breadboard, 'p', j));
        }  
    }

    int find_paths_counter = 0;
    // Process each path request
    for (const auto &request : requests)
    {
        find_paths_counter += findAndPrintPath(g, request);
    }

    cout << "Number of paths found: " << find_paths_counter << "  Out of: " << 64*40 << endl;

    return 0;
}


