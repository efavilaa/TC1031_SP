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

// Added for points 4 and 5: a saved copy of an installed network
// so we can compare it with the network after a failure / restoration
struct Network
{
    Edge *edges;
    int count;
    int cost;

    Network() : edges(nullptr), count(0), cost(0) {}
    ~Network() { delete[] edges; }
    Network(const Network &) = delete;
    Network &operator=(const Network &) = delete;
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
                    cout << "*\t";
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

    int find(int *parent, int building)
    {
        if (parent[building] != building)
            parent[building] = find(parent, parent[building]);
        return parent[building];
    }

    // we make sure we don't create a cycle
    // first find the roots (above) because if we have the same root, connecting would create a cycle
    // if not, we connect
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

    // we sort all the edges from cheapes to most expensive, use the cheap one first, and if able we add them

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

    // We print
    void displayMST() const
    {
        cout << "2. Installation Costs" << endl;
        for (int i = 0; i < mstCount; i++)
            cout << nodes[mst[i].a].name << " - " << nodes[mst[i].b].name
                 << " : " << mst[i].cost << endl;
        cout << "Total cost: " << mstCost << endl;
        if (mstCount < N - 1)
            cout << "spanning forest error" << endl;
        cout << endl;
    }

    // POINT 3. Communication between buildings, aka BFS

    // We check from where to where
    void getBuildingInput(int &start, int &dest) const
    {
        string a, b;
        cout << "3. Communication Between Buildings" << endl;
        do
        {
            cout << "Starting building: ";
            cin >> a;
            start = indexOf(a);
            if (start == -1)
                cout << "Building " << a << " does not exist" << endl;
        } while (start == -1);
        do
        {
            cout << "Destination building: ";
            cin >> b;
            dest = indexOf(b);
            if (dest == -1)
                cout << "Building " << b << " does not exist" << endl;
        } while (dest == -1);
    }

    bool BFS(int start, int dest, int *parent) const
    {
        // create tree from mst
        bool **tree = new bool *[N];
        for (int i = 0; i < N; i++)
        {
            tree[i] = new bool[N];
            for (int j = 0; j < N; j++)
                tree[i][j] = false;
        }
        for (int i = 0; i < mstCount; i++)
        {
            tree[mst[i].a][mst[i].b] = true;
            tree[mst[i].b][mst[i].a] = true;
        }

        // Not visited yet
        for (int i = 0; i < N; i++)
            parent[i] = -1;
        parent[start] = start;

        // Since we don't repeat building max size is N, and we add the route to the queue (FIFO)
        int *queue = new int[N];
        int front = 0, back = 0;
        queue[back++] = start;

        while (front < back && parent[dest] == -1)
        {
            int cur = queue[front++];
            for (int next = 0; next < N; next++)
            {
                if (tree[cur][next] && parent[next] == -1)
                {
                    parent[next] = cur;
                    queue[back++] = next;
                }
            }
        }

        for (int i = 0; i < N; i++)
            delete[] tree[i];
        delete[] tree;
        delete[] queue;

        return parent[dest] != -1;
    }

    // we reconstruct the oath from destination to back and then again to get the correct order
    int reconstructPath(int *parent, int start, int dest, int *path) const
    {
        int length = 0;
        for (int cur = dest; cur != start; cur = parent[cur])
            path[length++] = cur;
        path[length++] = start;

        for (int i = 0; i < length / 2; i++)
        {
            int tmp = path[i];
            path[i] = path[length - 1 - i];
            path[length - 1 - i] = tmp;
        }
        return length;
    }

    // Print
    void displayPath(int *path, int length) const
    {
        int total = 0;
        cout << "Route: ";
        for (int i = 0; i < length; i++)
        {
            cout << nodes[path[i]].name;
            if (i < length - 1)
            {
                cout << " -> ";
                total += matrix[path[i]][path[i + 1]].weight;
            }
        }
        cout << endl
             << "Total cost: " << total << endl
             << endl;
    }

    // POINT 4. Connection failure

    void printEdge(const Edge &e) const
    {
        cout << "  " << nodes[e.a].name << " - " << nodes[e.b].name << " : " << e.cost << endl;
    }

    // we copy the installed network (mst) so we can compare it later
    void saveNetwork(Network &out) const
    {
        delete[] out.edges;
        out.edges = new Edge[mstCount];
        for (int i = 0; i < mstCount; i++)
            out.edges[i] = mst[i];
        out.count = mstCount;
        out.cost = mstCost;
    }

    // position of the cable a-b inside a list of cables, -1 if it is not there (order of a and b doesn't matter)
    static int findEdge(const Edge *list, int count, int a, int b)
    {
        for (int i = 0; i < count; i++)
            if ((list[i].a == a && list[i].b == b) || (list[i].a == b && list[i].b == a))
                return i;
        return -1;
    }

    // we show the installed cables and ask which one fails, only installed cables can fail
    bool getFailedCableInput(int &a, int &b) const
    {
        cout << "4. Connection Failure" << endl;
        if (mstCount == 0)
        {
            cout << "There are no installed cables" << endl
                 << endl;
            return false;
        }
        cout << "Installed cables:" << endl;
        for (int i = 0; i < mstCount; i++)
            printEdge(mst[i]);

        string x, y;
        while (true)
        {
            cout << "Failed cable (building1 building2): ";
            if (!(cin >> x >> y))
                return false;
            a = indexOf(x);
            b = indexOf(y);
            if (a == -1 || b == -1)
                cout << "Building does not exist" << endl;
            else if (findEdge(mst, mstCount, a, b) == -1)
                cout << "There is no installed cable between " << x << " and " << y << endl;
            else
                return true;
        }
    }

    // the cable breaks: we take it out of the installed network and we turn it "off" in the matrix
    // (we don't delete it) so it is never used again, not in the restoration and not in the expansion
    void failCable(int a, int b)
    {
        int k = findEdge(mst, mstCount, a, b);
        if (k == -1)
            return;
        mstCost -= mst[k].cost;
        for (int i = k; i < mstCount - 1; i++)
            mst[i] = mst[i + 1];
        mstCount--;
        removeEdge(nodes[a].name, nodes[b].name);
    }

    // which buildings can still communicate: we use union-find (same as kruskal) with the cables that still work
    // at the end group[i] = the root of building i, buildings with the same root can communicate
    // returns how many groups there are (1 = the whole campus is connected)
    int findGroups(int *group)
    {
        int *rank = new int[N];
        for (int i = 0; i < N; i++)
        {
            group[i] = i;
            rank[i] = 0;
        }
        for (int i = 0; i < mstCount; i++)
            unite(group, rank, mst[i].a, mst[i].b);

        int groups = 0;
        for (int i = 0; i < N; i++)
            if (find(group, i) == i)
                groups++;

        delete[] rank;
        return groups;
    }

    // Print each group of buildings that can talk to each other
    void displayGroups(const int *group) const
    {
        int count = 0;
        for (int root = 0; root < N; root++)
        {
            if (group[root] != root)
                continue;
            cout << "  Group " << ++count << ": ";
            bool first = true;
            for (int i = 0; i < N; i++)
            {
                if (group[i] == root)
                {
                    if (!first)
                        cout << ", ";
                    cout << nodes[i].name;
                    first = false;
                }
            }
            cout << endl;
        }
    }

    // Print
    void displayFailure(int a, int b)
    {
        int *group = new int[N];
        int groups = findGroups(group);

        cout << "Failed cable: " << nodes[a].name << " - " << nodes[b].name
             << " : " << matrix[a][b].weight << endl;
        cout << "Cables still working:" << endl;
        for (int i = 0; i < mstCount; i++)
            printEdge(mst[i]);
        cout << "Buildings that can still communicate:" << endl;
        displayGroups(group);
        if (groups == 1)
            cout << "The entire campus remains connected" << endl;
        else
            cout << "The campus is NOT connected anymore (" << groups << " separated groups)" << endl;
        cout << endl;

        delete[] group;
    }

    // POINT 5. Restoring the network

    // we run kruskal again with every connection that is still available
    // the failed cable is "off" so kruskal doesn't see it (sortEdgesByCost only takes active cells)
    void restoreNetwork()
    {
        kruskalMST();
    }

    // Print the new network and compare it with the original one
    void displayRestoration(const Network &original)
    {
        cout << "5. Restoring the Network" << endl;

        bool possible = (mstCount == N - 1);
        if (possible)
            cout << "New installed network:" << endl;
        else
            cout << "Restoration impossible: there is no available connection that joins these groups" << endl
                 << "Best network we can install:" << endl;
        for (int i = 0; i < mstCount; i++)
            printEdge(mst[i]);
        cout << "Total cost: " << mstCost << endl;

        if (!possible)
        {
            int *group = new int[N];
            findGroups(group);
            cout << "Buildings that stay separated:" << endl;
            displayGroups(group);
            delete[] group;
        }

        // comparison: cables that stay, cables that are gone and cables that are new
        cout << endl
             << "Comparison with the original network:" << endl;
        cout << "Cables kept:" << endl;
        for (int i = 0; i < original.count; i++)
            if (findEdge(mst, mstCount, original.edges[i].a, original.edges[i].b) != -1)
                printEdge(original.edges[i]);
        cout << "Cables removed:" << endl;
        for (int i = 0; i < original.count; i++)
            if (findEdge(mst, mstCount, original.edges[i].a, original.edges[i].b) == -1)
                printEdge(original.edges[i]);
        cout << "Cables added:" << endl;
        bool added = false;
        for (int i = 0; i < mstCount; i++)
            if (findEdge(original.edges, original.count, mst[i].a, mst[i].b) == -1)
            {
                printEdge(mst[i]);
                added = true;
            }
        if (!added)
            cout << "  (none)" << endl;

        int diff = mstCost - original.cost;
        cout << "Original cost: " << original.cost << endl
             << "New cost: " << mstCost << endl
             << "Difference: " << (diff >= 0 ? "+" : "") << diff << endl
             << endl;
    }
};
