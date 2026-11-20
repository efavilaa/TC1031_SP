#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <climits>
using namespace std;
#include "Graph.cpp"
#include "Reader.cpp"
#include "Expansion.cpp"

int main()
{

    // Part 1
    string fileName = "test_fix.txt";
    Reader reader(fileName);
    if (!reader.isOpen())
    {
        cout << "Could not open " << fileName << endl;
        return 1;
    }

    Graph g(reader.readSize(), true);
    reader.readNetwork(g);

    cout << "1. The Campus Network" << endl;
    g.printNodes();
    g.printMatrix();

    // Part 2

    // krukal
    g.kruskalMST();
    g.displayMST();

    // Part 3
    int start, dest;
    g.getBuildingInput(start, dest);

    int *parent = new int[g.size()];
    int *path = new int[g.size()];

    // BFS
    g.BFS(start, dest, parent);
    int length = g.reconstructPath(parent, start, dest, path);
    g.displayPath(path, length);

    delete[] parent;
    delete[] path;

    // Part 4
    // we save the original network before the failure so we can compare it later
    Stage original, restored, expanded;
    saveStage(g, original, "Original network");

    int failA, failB;
    if (g.getFailedCableInput(failA, failB))
    {
        g.failCable(failA, failB);
        g.displayFailure(failA, failB);

        // Part 5
        g.restoreNetwork();
        g.displayRestoration(original.net);
    }
    saveStage(g, restored, "After failure and restoration");

    // Part 6
    NewBuilding building;
    readNewBuilding(fileName, building);
    if (expandCampus(g, building))
    {
        saveStage(g, expanded, "After adding " + building.name);
        displayExpansion(g, restored, expanded);
    }

    // Part 7
    compareNetworks(g, original, restored, expanded);

    return 0;
}