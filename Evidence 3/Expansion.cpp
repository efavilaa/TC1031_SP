#include <iomanip>

// POINT 6. Campus expansion
// POINT 7. Network comparison

struct NewEdge
{
    string from, to;
    int cost;
};

struct NewBuilding
{
    string name;
    int x = 0, y = 0;
    NewEdge *edges = nullptr;
    int count = 0;

    NewBuilding() {}
    ~NewBuilding() { delete[] edges; }
    NewBuilding(const NewBuilding &) = delete;
    NewBuilding &operator=(const NewBuilding &) = delete;
};

// a saved network + how many buildings the campus had at that moment
struct Stage
{
    string title;
    Network net;
    int buildings = 0;
};

void saveStage(Graph &g, Stage &stage, const string &title)
{
    stage.title = title;
    g.saveNetwork(stage.net);
    stage.buildings = g.size();
}

// reads the "# NEW NODE" and "# NEW EDGES" sections of the file
bool readNewBuilding(const string &fileName, NewBuilding &building)
{
    ifstream in(fileName);
    string line, section;
    int read = 0;

    while (getline(in, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty())
            continue;
        if (line[0] == '#')
        {
            section = line;
            continue;
        }

        stringstream ss(line);
        if (section.find("NEW NODE") != string::npos)
            ss >> building.name >> building.x >> building.y;
        else if (section.find("NEW EDGES") != string::npos)
        {
            if (building.edges == nullptr)
            {
                ss >> building.count;
                building.edges = new NewEdge[building.count];
            }
            else if (read < building.count)
            {
                NewEdge &e = building.edges[read++];
                ss >> e.from >> e.to >> e.cost;
            }
        }
    }
    building.count = read;
    return !building.name.empty();
}

// adds the building and its cables, then runs kruskal again
// the damaged cable is still "off" in the matrix, so kruskal never uses it
bool expandCampus(Graph &g, const NewBuilding &building)
{
    cout << "6. Campus Expansion" << endl;
    if (building.name.empty())
    {
        cout << "There is no new building in the file" << endl
             << endl;
        return false;
    }
    if (g.indexOf(building.name) != -1)
    {
        cout << "Building " << building.name << " already exists" << endl
             << endl;
        return false;
    }

    g.addNode(building.name, building.x, building.y);
    int newIndex = g.size() - 1;
    cout << "New building: " << building.name << " (" << building.x << ", " << building.y << ")" << endl;
    cout << "Possible connections:" << endl;

    int added = 0;
    for (int i = 0; i < building.count; i++)
    {
        const NewEdge &e = building.edges[i];
        string other = (e.from == building.name) ? e.to : e.from;
        int j = g.indexOf(other);

        // only cables that touch the new building, so an old damaged cable can't come back
        bool touchesNew = (e.from == building.name || e.to == building.name);
        if (!touchesNew || j == -1 || j == newIndex)
        {
            cout << "  " << e.from << " - " << e.to << " : ignored (invalid connection)" << endl;
            continue;
        }
        g.addEdge(newIndex, j, e.cost);
        cout << "  " << building.name << " - " << other << " : " << e.cost << endl;
        added++;
    }
    if (added == 0)
        cout << building.name << " has no valid connections, it can't be connected" << endl;
    cout << endl;

    g.printMatrix();
    g.kruskalMST();
    return true;
}

// a network without cycles with n buildings and k cables has n - k separated groups
int countGroups(const Stage &stage)
{
    return stage.buildings - stage.net.count;
}

void printStage(Graph &g, const Stage &stage)
{
    cout << "--- " << stage.title << " ---" << endl;
    for (int i = 0; i < stage.net.count; i++)
        g.printEdge(stage.net.edges[i]);

    int groups = countGroups(stage);
    cout << "Buildings: " << stage.buildings
         << " | Cables: " << stage.net.count
         << " | Total cost: " << stage.net.cost << endl;
    if (groups == 1)
        cout << "The whole campus is connected" << endl;
    else
        cout << "The campus is NOT connected (" << groups << " separated groups)" << endl;
    cout << endl;
}

// cables that are in "a" but not in "b"
int printMissing(Graph &g, const Network &a, const Network &b)
{
    int missing = 0;
    for (int i = 0; i < a.count; i++)
        if (Graph::findEdge(b.edges, b.count, a.edges[i].a, a.edges[i].b) == -1)
        {
            g.printEdge(a.edges[i]);
            missing++;
        }
    if (missing == 0)
        cout << "  (none)" << endl;
    return missing;
}

void explainChanges(Graph &g, const Stage &before, const Stage &after)
{
    cout << before.title << "  ->  " << after.title << endl;
    cout << "Cables removed:" << endl;
    int removed = printMissing(g, before.net, after.net);
    cout << "Cables added:" << endl;
    int added = printMissing(g, after.net, before.net);

    int diff = after.net.cost - before.net.cost;
    cout << "Cost: " << before.net.cost << " -> " << after.net.cost
         << " (" << (diff >= 0 ? "+" : "") << diff << ")" << endl;

    int newBuildings = after.buildings - before.buildings;
    if (newBuildings > 0)
    {
        cout << "Why: " << newBuildings << " new building joined the campus";
        if (removed > 0)
            cout << ", its cables gave a cheaper route, so " << removed << " old cable(s) are not needed anymore";
        cout << endl;
    }
    else if (removed > 0 && added > 0)
        cout << "Why: the failed cable can't be used, so kruskal picked the cheapest available replacement" << endl;
    else if (removed > 0)
        cout << "Why: the failed cable can't be used and there is no available replacement" << endl;
    else
        cout << "Why: nothing changed in the installed network" << endl;

    int groupsBefore = countGroups(before), groupsAfter = countGroups(after);
    if (groupsBefore == 1 && groupsAfter > 1)
        cout << "The campus lost connectivity" << endl;
    else if (groupsBefore > 1 && groupsAfter == 1)
        cout << "The campus is connected again" << endl;
    cout << endl;
}

void displayExpansion(Graph &g, const Stage &before, const Stage &after)
{
    cout << "New installed network:" << endl;
    printStage(g, after);
    explainChanges(g, before, after);
}

void compareNetworks(Graph &g, const Stage &original, const Stage &restored, const Stage &expanded)
{
    cout << "7. Network Comparison" << endl
         << endl;
    const Stage *stages[3] = {&original, &restored, &expanded};
    int total = expanded.title.empty() ? 2 : 3;

    for (int i = 0; i < total; i++)
        printStage(g, *stages[i]);

    cout << left << setw(32) << "Network" << setw(11) << "Buildings"
         << setw(8) << "Cables" << setw(6) << "Cost" << "Connected" << endl;
    for (int i = 0; i < total; i++)
        cout << left << setw(32) << stages[i]->title << setw(11) << stages[i]->buildings
             << setw(8) << stages[i]->net.count << setw(6) << stages[i]->net.cost
             << (countGroups(*stages[i]) == 1 ? "yes" : "no") << endl;
    cout << endl;

    cout << "Explanation of the changes:" << endl;
    for (int i = 0; i + 1 < total; i++)
        explainChanges(g, *stages[i], *stages[i + 1]);
}