#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <climits>
using namespace std;
#include "Graph.cpp"

int main()
{
    string fileName = "test+.txt";
    ifstream in(fileName);
    if (!in)
    {
        cout << "Could not open " << fileName << endl;
        return 1;
    }

    string line;

    int n, e;
    nextLine(in, line);
    stringstream(line) >> n >> e;

    Graph g(n, false, true);

    for (int i = 0; i < n; i++)
    {
        nextLine(in, line);
        string name;
        int x, y;
        stringstream(line) >> name >> x >> y;
        g.setNode(i, name, x, y);
    }

    for (int i = 0; i < e; i++)
    {
        nextLine(in, line);
        string a, b;
        int cost;
        stringstream(line) >> a >> b >> cost;
        g.addEdge(a, b, cost);
    }

    cout << "Initial network" << endl;
    g.printNodes();
    g.printMatrix();

    string failA, failB;
    if (nextLine(in, line))
    {
        stringstream(line) >> failA >> failB;
        g.removeEdge(failA, failB);
        cout << "After failure " << failA << "-" << failB << " " << endl;
        g.printMatrix();
    }

    if (nextLine(in, line))
    {
        string name;
        int x, y;
        stringstream(line) >> name >> x >> y;
        g.addNode(name, x, y);

        int k = 0;
        if (nextLine(in, line))
            stringstream(line) >> k;
        for (int i = 0; i < k; i++)
        {
            nextLine(in, line);
            string a, b;
            int cost;
            stringstream(line) >> a >> b >> cost;
            g.addEdge(a, b, cost);
        }
        cout << "=== After adding " << name << " ===" << endl;
        g.printNodes();
        g.printMatrix();
    }

    return 0;
}
