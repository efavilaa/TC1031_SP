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

class Graph
{
private:
    int **matrix;
    Node *nodes;
    int N;
    int E;
    bool directed;
    bool weighted;

public:
    Graph(const Graph &) = delete;
    Graph &operator=(const Graph &) = delete;

    Graph(int n, bool dir = false, bool wei = true)
    {
        if (n < 0)
            n = 0;
        N = n;
        E = 0;
        directed = dir;
        weighted = wei;
        nodes = new Node[N];
        matrix = new int *[N];
        for (int i = 0; i < N; i++)
        {
            matrix[i] = new int[N];
            for (int j = 0; j < N; j++)
                matrix[i][j] = -1;
        }
    }

    ~Graph()
    {
        for (int i = 0; i < N; i++)
            delete[] matrix[i];
        delete[] matrix;
        delete[] nodes;
    }

    int size() const { return N; }

    void setNode(int i, const string &name, int x, int y)
    {
        if (i < 0 || i >= N)
            return;
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

    void addEdge(int a, int b, int weight = 1)
    {
        // dont do file consistency, never, he doesn't ask
        if (!weighted)
            weight = 1;
        if (a < 0 || a >= N || b < 0 || b >= N)
        {
            cout << "Error: Invalid edge (" << a << ", " << b << ")" << endl;
            return;
        }
        if (a == b) // si se puede conectar a si mismo
        {
            cout << "Error: self-loops are not allowed" << endl;
            return;
        }
        if (matrix[a][b] != -1)
        {
            cout << "Error: edge already exists" << endl;
            return;
        }
        // quitar directed, its undericted a fuerzas
        matrix[a][b] = weight;
        if (!directed)
            matrix[b][a] = weight;
        E++;
    }

    void addEdge(const string &a, const string &b, int weight)
    {
        int i = indexOf(a), j = indexOf(b);
        if (i == -1 || j == -1)
        {
            cout << "Error: unknown building " << (i == -1 ? a : b) << endl;
            return;
        }
        addEdge(i, j, weight);
    }
    // not remove but turning off the connection between two nodes
    void removeEdge(const string &a, const string &b)
    {
        int i = indexOf(a), j = indexOf(b);
        if (i == -1 || j == -1 || matrix[i][j] == -1)
        {
            cout << "Error: connection " << a << "-" << b << " does not exist" << endl;
            return;
        }
        matrix[i][j] = -1;
        if (!directed)
            matrix[j][i] = -1;
        E--;
    }

    void addNode(const string &name, int x, int y)
    {
        int n2 = N + 1;
        int **m2 = new int *[n2];
        for (int i = 0; i < n2; i++)
        {
            m2[i] = new int[n2];
            for (int j = 0; j < n2; j++)
                m2[i][j] = (i < N && j < N) ? matrix[i][j] : -1;
        }
        Node *nodes2 = new Node[n2];
        for (int i = 0; i < N; i++)
            nodes2[i] = nodes[i];

        for (int i = 0; i < N; i++)
            delete[] matrix[i];
        delete[] matrix;
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
                if (matrix[i][j] == -1)
                    cout << "-\t";
                else
                    cout << matrix[i][j] << "\t";
            }
            cout << endl;
        }
        cout << "Edges: " << E << endl
             << endl;
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