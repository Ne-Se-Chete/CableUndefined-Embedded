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
class Device
{
public:
    int num;

    Device(int n) : num(n) {}

    virtual void printConnections() const = 0;
};

class ConnectionNode
{
public:
    Device *device;
    int index;
    char connectionType;

    ConnectionNode(Device *d, int i, char type);

    void print() const;
};

class Multiplexer : public Device
{
public:
    ConnectionNode *x[16];
    ConnectionNode *y[8];

    Multiplexer(int n);

    void printConnections() const override;
};

class Breadboard : public Device
{
public:
    ConnectionNode *pin[24];

    Breadboard(int n);

    void printConnections() const override;
};

ConnectionNode::ConnectionNode(Device *d, int i, char type) : device(d), index(i), connectionType(type) {}

void ConnectionNode::print() const
{
    if (dynamic_cast<Multiplexer *>(device))
    {
        // cout << "MUX_" << device->num << "." << connectionType << "[" << index << "]";
    }
    else if (dynamic_cast<Breadboard *>(device))
    {
        // cout << "Breadboard_" << device->num << ".pin[" << index << "]";
    }
}

Multiplexer::Multiplexer(int n) : Device(n)
{
    for (auto &xi : x)
        xi = nullptr;
    for (auto &yi : y)
        yi = nullptr;
}

Breadboard::Breadboard(int n) : Device(n)
{
    for (auto &pin : pin)
        pin = nullptr;
}

void Multiplexer::printConnections() const
{
    for (int i = 0; i < 16; ++i)
    {
        if (x[i])
        {
            // cout << "MUX_" << num << ".x[" << i << "] -> ";
            x[i]->print();
            // cout << endl;
        }
    }
    for (int i = 0; i < 8; ++i)
    {
        if (y[i])
        {
            // cout << "MUX_" << num << ".y[" << i << "] -> ";
            y[i]->print();
            // cout << endl;
        }
    }
}

void Breadboard::printConnections() const
{
    for (int i = 0; i < 64; ++i)
    {
        if (pin[i])
        {
            // cout << "Breadboard_" << num << ".pin[" << i << "] -> ";
            pin[i]->print(); // This will print either MUX or Breadboard connection information
            // cout << endl;
        }
    }
}

// check if there is a bidirectional connection between two devices
bool checkBidirectionalConnection(Device &device1, char type1, int index1, Device &device2, char type2, int index2)
{
    if (auto mux1 = dynamic_cast<Multiplexer *>(&device1))
    {
        if (auto mux2 = dynamic_cast<Multiplexer *>(&device2))
        {
            ConnectionNode *node1 = (type1 == 'x') ? mux1->x[index1] : mux1->y[index1];
            ConnectionNode *node2 = (type2 == 'x') ? mux2->x[index2] : mux2->y[index2];

            if (node1 && node2 && node1->device == &device2 && node2->device == &device1 && node1->index == index2 && node2->index == index1)
            {
                return true;
            }
        }
        else if (auto breadboard2 = dynamic_cast<Breadboard *>(&device2))
        {
            ConnectionNode *node1 = (type1 == 'x') ? mux1->x[index1] : mux1->y[index1];
            ConnectionNode *node2 = breadboard2->pin[index2];

            if (node1 && node2 && node1->device == &device2 && node2->device == &device1 && node1->index == index2 && node2->index == index1)
            {
                return true;
            }
        }
    }
    else if (auto breadboard1 = dynamic_cast<Breadboard *>(&device1))
    {
        if (auto mux2 = dynamic_cast<Multiplexer *>(&device2))
        {
            ConnectionNode *node1 = breadboard1->pin[index1];
            ConnectionNode *node2 = (type2 == 'x') ? mux2->x[index2] : mux2->y[index2];

            if (node1 && node2 && node1->device == &device2 && node2->device == &device1 && node1->index == index2 && node2->index == index1)
            {
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
        int mainBreadboardStart = 2 * 24, mainBreadboardEnd = 2 * 24 + 23;     // Adjust these based on actual ranges
        int mcuBreadboardStart = 2 * 24 + 24, mcuBreadboardEnd = 2 * 24 + 31; // Adjust these based on actual ranges

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
        if (deviceId == 3) // Main breadboard
        {
            return 2 * 24 + pinIndex;
        }

        if (deviceId == 4) // MCU breadboard
        {
            return 2 * 24 + 24 + pinIndex;
        }
    }
    return -1; // Error case
}

string printDeviceSpecifications(int vertexID)
{
    int numMultiplexers = 2;    // Assuming there are 18 multiplexers before the breadboards
    int multiplexerPins = 24;    // Assuming each multiplexer has 24 pins
    int mainBreadboardPins = 24; // Assuming the main breadboard has 64 pins

    if (vertexID < numMultiplexers * multiplexerPins)
    {
        // It's a multiplexer
        int deviceId = vertexID / multiplexerPins;
        int pinIndex = vertexID % multiplexerPins;
        char type = pinIndex < 16 ? 'x' : 'y';
        pinIndex = pinIndex < 16 ? pinIndex : pinIndex - 16; // Adjust pinIndex for 'y' type
        //cout << " -> MUX" << deviceId + 1 << " " << type << pinIndex;
        return " -> MUX" + to_string(deviceId + 1) + " " + type + to_string(pinIndex);
    }
    else if (vertexID < numMultiplexers * multiplexerPins + mainBreadboardPins)
    {
        // It's the main breadboard
        int pinIndex = vertexID - (numMultiplexers * multiplexerPins);
        // cout << " -> MainBreadboard " << pinIndex + 1; // Adjust pinIndex to start from 1 for better readability
        return " -> MainBreadboard " + to_string(pinIndex);
    }
    else
    {
        // It's the MCU breadboard
        int pinIndex = vertexID - (numMultiplexers * multiplexerPins + mainBreadboardPins);
        // cout << " -> MCUBreadboard " << pinIndex + 1; // Adjust pinIndex to start from 1 for better readability
        return " -> MCUBreadboard " + to_string(pinIndex);
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
        ofstream found_paths_file("mini_scheme_found_paths.txt", ios::app);
        if (!found_paths_file)
        {
            cerr << "Error: unable to open output file" << endl;
            return -1;
        }

        string from = printDeviceSpecifications(startVertex);
        string to = printDeviceSpecifications(endVertex);

        found_paths_file << "Path from " << from << " to " << to << " is: \n"; 
        for (int vertex : path)
        {
            string device = printDeviceSpecifications(vertex);
            found_paths_file << device;
        }
        found_paths_file << "\n\n";
        found_paths_file.close();

        cout << "\nPath from " << from << " to " << to << " is: \n"; // this is ok, double print is before this line ^^^
        for (int vertex : path)
        {
            string device = printDeviceSpecifications(vertex);
            cout << device;
        }
        cout << "\n\n";
        return 1;
    }
    else
    {
        // Print the not found path in a txt file and in the console
        ofstream not_found_paths_file("mini_scheme_not_found_paths.txt", ios::app);
        if (!not_found_paths_file)
        {
            cerr << "Error: unable to open output file" << endl;
            return -1;
        }

        string from = printDeviceSpecifications(startVertex);
        string to = printDeviceSpecifications(endVertex);

        not_found_paths_file << "No path found from " << from << " to " << to << ".\n\n";
        cout << "\nNo path found from " << from << " to " << to << ".\n\n";
        return 0;
    }
}


int main() // initPathfinding
{
    Multiplexer mux1(0), mux2(1);
    Breadboard main_breadboard(3), mcu_breadboard(4);

    Multiplexer all_muxes[2] = {mux1, mux2};

    int numVertices = 2 * 24 + 1 * 24 + 1 * 8;
    Graph g(numVertices);

    // Add edges to the graph every X to Y connection in the muxes
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                int srcVertex = getGraphVertexID(&all_muxes[i], 'x', j);
                int destVertex = getGraphVertexID(&all_muxes[i], 'y', k);
                g.addEdge(srcVertex, destVertex);
            }
        }
    }

    // MUX1 pins edge connections FIXED
    g.addEdge(getGraphVertexID(&mux1, 'x', 0), getGraphVertexID(&mux2, 'y', 0));
    g.addEdge(getGraphVertexID(&mux1, 'x', 1), getGraphVertexID(&mux2, 'y', 1));
    g.addEdge(getGraphVertexID(&mux1, 'x', 2), getGraphVertexID(&mux2, 'y', 2));
    g.addEdge(getGraphVertexID(&mux1, 'x', 3), getGraphVertexID(&mux2, 'y', 3));
    g.addEdge(getGraphVertexID(&mux1, 'x', 4), getGraphVertexID(&mux2, 'y', 4));
    g.addEdge(getGraphVertexID(&mux1, 'x', 5), getGraphVertexID(&mux2, 'y', 5));
    g.addEdge(getGraphVertexID(&mux1, 'x', 6), getGraphVertexID(&mux2, 'y', 6));
    g.addEdge(getGraphVertexID(&mux1, 'x', 7), getGraphVertexID(&mux2, 'y', 7));
    g.addEdge(getGraphVertexID(&mux1, 'x', 8), getGraphVertexID(&main_breadboard, 'p', 13));
    g.addEdge(getGraphVertexID(&mux1, 'x', 9), getGraphVertexID(&main_breadboard, 'p', 14));
    g.addEdge(getGraphVertexID(&mux1, 'x', 10), getGraphVertexID(&main_breadboard, 'p', 15));
    g.addEdge(getGraphVertexID(&mux1, 'x', 11), getGraphVertexID(&main_breadboard, 'p', 16));
    g.addEdge(getGraphVertexID(&mux1, 'x', 12), getGraphVertexID(&main_breadboard, 'p', 17));
    g.addEdge(getGraphVertexID(&mux1, 'x', 13), getGraphVertexID(&main_breadboard, 'p', 18));
    g.addEdge(getGraphVertexID(&mux1, 'x', 14), getGraphVertexID(&main_breadboard, 'p', 19));
    g.addEdge(getGraphVertexID(&mux1, 'x', 15), getGraphVertexID(&main_breadboard, 'p', 20));

    g.addEdge(getGraphVertexID(&mux1, 'y', 0), getGraphVertexID(&mcu_breadboard, 'p', 1));
    g.addEdge(getGraphVertexID(&mux1, 'y', 1), getGraphVertexID(&mcu_breadboard, 'p', 2));
    g.addEdge(getGraphVertexID(&mux1, 'y', 2), getGraphVertexID(&mcu_breadboard, 'p', 3));
    g.addEdge(getGraphVertexID(&mux1, 'y', 3), getGraphVertexID(&mcu_breadboard, 'p', 4));
    g.addEdge(getGraphVertexID(&mux1, 'y', 4), getGraphVertexID(&mcu_breadboard, 'p', 5));
    g.addEdge(getGraphVertexID(&mux1, 'y', 5), getGraphVertexID(&mcu_breadboard, 'p', 6));
    g.addEdge(getGraphVertexID(&mux1, 'y', 6), getGraphVertexID(&mcu_breadboard, 'p', 7));
    g.addEdge(getGraphVertexID(&mux1, 'y', 7), getGraphVertexID(&mcu_breadboard, 'p', 8));

    // MUX2 pins edge connections FIXED
    g.addEdge(getGraphVertexID(&mux2, 'x', 0), getGraphVertexID(&main_breadboard, 'p', 21));
    g.addEdge(getGraphVertexID(&mux2, 'x', 1), getGraphVertexID(&main_breadboard, 'p', 22));
    g.addEdge(getGraphVertexID(&mux2, 'x', 2), getGraphVertexID(&main_breadboard, 'p', 23));
    g.addEdge(getGraphVertexID(&mux2, 'x', 3), getGraphVertexID(&main_breadboard, 'p', 24));
    g.addEdge(getGraphVertexID(&mux2, 'x', 4), getGraphVertexID(&main_breadboard, 'p', 1));
    g.addEdge(getGraphVertexID(&mux2, 'x', 5), getGraphVertexID(&main_breadboard, 'p', 2));
    g.addEdge(getGraphVertexID(&mux2, 'x', 6), getGraphVertexID(&main_breadboard, 'p', 3));
    g.addEdge(getGraphVertexID(&mux2, 'x', 7), getGraphVertexID(&main_breadboard, 'p', 4));
    g.addEdge(getGraphVertexID(&mux2, 'x', 8), getGraphVertexID(&main_breadboard, 'p', 5));
    g.addEdge(getGraphVertexID(&mux2, 'x', 9), getGraphVertexID(&main_breadboard, 'p', 6));
    g.addEdge(getGraphVertexID(&mux2, 'x', 10), getGraphVertexID(&main_breadboard, 'p', 7));
    g.addEdge(getGraphVertexID(&mux2, 'x', 11), getGraphVertexID(&main_breadboard, 'p', 8));
    g.addEdge(getGraphVertexID(&mux2, 'x', 12), getGraphVertexID(&main_breadboard, 'p', 9));
    g.addEdge(getGraphVertexID(&mux2, 'x', 13), getGraphVertexID(&main_breadboard, 'p', 10));
    g.addEdge(getGraphVertexID(&mux2, 'x', 14), getGraphVertexID(&main_breadboard, 'p', 11));
    g.addEdge(getGraphVertexID(&mux2, 'x', 15), getGraphVertexID(&main_breadboard, 'p', 12));


    vector<PathRequest> requests;
    
    int mcu_pin, main_pin;
    std::cout << "Enter MCU pin: ";
    std::cin >> mcu_pin;
    std::cout << "Enter MAIN breadboard pin: ";
    std::cin >> main_pin;

    PathRequest request(&main_breadboard, 'p', main_pin, &mcu_breadboard, 'p', mcu_pin);
    int path_found = findAndPrintPath(g, request);

    std::cout << "Number of paths found: " << path_found << std::endl;

    return 0;
}