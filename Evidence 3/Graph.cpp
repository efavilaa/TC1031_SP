#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <climits>
using namespace std;

// struct for the nodes and coordinates
struct Node
{
    string name;
    int x, y;
};

// matrix -1=not connected, active= connected but off, weight=cost of connection
struct Cell
{
    int weight;
    bool active;
};

// Added for krukasl
struct Edge
{
    int a, b;
    int cost;
};

class Graph
{
private:
    // matrix
    Cell **matrix;
    // nodes and coordinates
    Node *nodes;
    int N;
    int E;
    bool weighted;

    // Added for kruskal
    Edge *edges;
    int edgeCount;
    Edge *mst;
    int mstCount;
    int mstCost;

    static Cell **newMatrix(int n)
    {
        Cell **m = new Cell *[n];
        for (int i = 0; i < n; i++)
        {
            m[i] = new Cell[n];
            for (int j = 0; j < n; j++)
                m[i][j] = {-1, false};
        }
        return m;
    }

    void freeMatrix()
    {
        for (int i = 0; i < N; i++)
            delete[] matrix[i];
        delete[] matrix;
    }

    // we follow the diagonal /
    void setCell(int a, int b, Cell c)
    {
        matrix[a][b] = c;
        matrix[b][a] = c;
    }

public:
    Graph(const Graph &) = delete;
    Graph &operator=(const Graph &) = delete;

    Graph(int n, bool wei = true)
    {
        N = n;
        E = 0;
        weighted = wei;
        nodes = new Node[N];
        matrix = newMatrix(N);
        edges = nullptr;
        edgeCount = 0;
        mst = nullptr;
        mstCount = 0;
        mstCost = 0;
    }

    ~Graph()
    {
        freeMatrix();
        delete[] nodes;
        delete[] edges;
        delete[] mst;
    }

    int size() const { return N; }

    void setNode(int i, const string &name, int x, int y)
    {
        nodes[i].name = name;
        nodes[i].x = x;
        nodes[i].y = y;
    }

    int indexOf(const string &name) const
    {
        for (int i = 0; i < N; i++)
            if (nodes[i].name == name)
                return i;
        return -1;
    }

    // CHANGE AFTER PROFFESOR: we allow self-loops and multiple edges
    void addEdge(int a, int b, int weight = 1)
    {
        if (!weighted)
            weight = 1;
        if (!matrix[a][b].active)
            E++;
        setCell(a, b, {weight, true});
    }

    void addEdge(const string &a, const string &b, int weight)
    {
        addEdge(indexOf(a), indexOf(b), weight);
    }

    // CHANGE AFTER PROFESSOR: we don't remove edges, we disconnect them
    void removeEdge(const string &a, const string &b)
    {
        int i = indexOf(a), j = indexOf(b);
        if (matrix[i][j].active)
            E--;
        matrix[i][j].active = false;
        matrix[j][i].active = false;
    }

    // CHANGE AFTER PROFESSOR: we can reconenct edges
    void reconnectEdge(const string &a, const string &b)
    {
        int i = indexOf(a), j = indexOf(b);
        if (!matrix[i][j].active && matrix[i][j].weight != -1)
        {
            matrix[i][j].active = true;
            matrix[j][i].active = true;
            E++;
        }
    }

    bool isConnected(int a, int b) const { return matrix[a][b].active; }

    // CHANGE AFTER PROFESSOR: it expands the matrix

    void addNode(const string &name, int x, int y)
    {
        int n2 = N + 1;
        Cell **m2 = newMatrix(n2);
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                m2[i][j] = matrix[i][j];

        Node *nodes2 = new Node[n2];
        for (int i = 0; i < N; i++)
            nodes2[i] = nodes[i];

        freeMatrix();
        delete[] nodes;

        matrix = m2;
        nodes = nodes2;
        N = n2;
        setNode(N - 1, name, x, y);
    }

    void printNodes() const
    {
        for (int i = 0; i < N; i++)
            cout << i << ": " << nodes[i].name << " (" << nodes[i].x << ", " << nodes[i].y << ")" << endl;
        cout << endl;
    }

    void printMatrix() const
    {
        cout << "\t";
        for (int j = 0; j < N; j++)
            cout << nodes[j].name << "\t";
        cout << endl;
        for (int i = 0; i < N; i++)
        {
            cout << nodes[i].name << "\t";
            for (int j = 0; j < N; j++)
            {
                if (matrix[i][j].active)
                    cout << matrix[i][j].weight << "\t";
                else if (matrix[i][j].weight != -1)
                    cout << "off\t";
                else
                    cout << "-\t";
            }
            cout << endl;
        }
        cout << "Edges: " << E << endl
             << endl;
    }

    // POINT 2. Installation cost, aka Kruskal

    // First, we use only half the matrix (because its undirected and thus its symetrical) then we sort the edges, to begin in the cheapes one.
    void sortEdgesByCost()
    {
        delete[] edges;
        edges = new Edge[E];
        edgeCount = 0;
        for (int i = 0; i < N; i++)
            for (int j = i + 1; j < N; j++)
                if (matrix[i][j].active)
                    edges[edgeCount++] = {i, j, matrix[i][j].weight};

        for (int i = 1; i < edgeCount; i++)
        {
            Edge key = edges[i];
            int j = i - 1;
            while (j >= 0 && edges[j].cost > key.cost)
            {
                edges[j + 1] = edges[j];
                j--;
            }
            edges[j + 1] = key;
        }
    }

    //!!ETHIEL CHECK returns the representative (root) of the set that contains the building; compresses the path so later lookups are faster
    int find(int *parent, int building)
    {
        if (parent[building] != building)
            parent[building] = find(parent, parent[building]);
        return parent[building];
    }

    //!!ETHIEL CHECK joins the sets of two buildings using union by rank; returns false if they were already in the same set (that edge would create a cycle)
    bool unite(int *parent, int *rank, int building1, int building2)
    {
        int r1 = find(parent, building1);
        int r2 = find(parent, building2);
        if (r1 == r2)
            return false;
        if (rank[r1] < rank[r2])
            parent[r1] = r2;
        else if (rank[r1] > rank[r2])
            parent[r2] = r1;
        else
        {
            parent[r2] = r1;
            rank[r1]++;
        }
        return true;
    }

    //!!ETHIEL CHECK builds the minimum spanning tree: goes through the edges from cheapest to most expensive and keeps each one that connects two different sets, until N-1 edges are chosen
    void kruskalMST()
    {
        sortEdgesByCost();

        int *parent = new int[N];
        int *rank = new int[N];
        for (int i = 0; i < N; i++)
        {
            parent[i] = i;
            rank[i] = 0;
        }

        delete[] mst;
        mst = new Edge[N > 0 ? N - 1 : 0];
        mstCount = 0;
        mstCost = 0;

        for (int i = 0; i < edgeCount && mstCount < N - 1; i++)
        {
            if (unite(parent, rank, edges[i].a, edges[i].b))
            {
                mst[mstCount++] = edges[i];
                mstCost += edges[i].cost;
            }
        }

        delete[] parent;
        delete[] rank;
    }

    //!!ETHIEL CHECK prints the edges chosen by Kruskal and the total installation cost
    void displayMST() const
    {
        cout << "Minimum Spanning Tree (Kruskal)" << endl;
        for (int i = 0; i < mstCount; i++)
            cout << nodes[mst[i].a].name << " - " << nodes[mst[i].b].name
                 << " : " << mst[i].cost << endl;
        cout << "Total cost: " << mstCost << endl;
        if (mstCount < N - 1)
            cout << "Warning: network is disconnected, this is a spanning forest" << endl;
        cout << endl;
    }
};

bool nextLine(ifstream &in, string &line)
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
