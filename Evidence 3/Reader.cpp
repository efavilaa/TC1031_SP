#include <iostream>
using namespace std;

// PART 1The Campus Network: Read a file containing the buildings and their possible cable connections, each with an associated installation cost. Construct a representation of the network using your own Node class or struct. You may choose the graph representation.
class Reader
{
private:
    ifstream in;
    string line;
    int n = 0; // number of buildings
    int e = 0; // number of connections

    bool nextLine()
    {
        while (getline(in, line))
        {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();
            if (line.empty() || line[0] == '#')
                continue;
            return true;
        }
        return false;
    }

public:
    Reader(const string &fileName) : in(fileName) {}

    bool isOpen() const { return in.is_open(); }

    // POINT 1. The campus network
    // first line: number of buildings (n) and connections (e)
    // returns n so main can create the Graph with the right size
    int readSize()
    {
        if (nextLine())
            stringstream(line) >> n >> e;
        return n;
    }

    // then n buildings (name x y) and e connections (a b cost) go into the Graph
    void readNetwork(Graph &g)
    {
        for (int i = 0; i < n; i++)
        {
            nextLine();
            string name;
            int x, y;
            stringstream(line) >> name >> x >> y;
            g.setNode(i, name, x, y);
        }

        for (int i = 0; i < e; i++)
        {
            nextLine();
            string a, b;
            int cost;
            stringstream(line) >> a >> b >> cost;
            g.addEdge(a, b, cost);
        }
    }
};
