#include "Keeper.h"
#include <fstream>
#include <iostream>

// cconstructor
// Complexity: O(1)
Node::Node(Entry data)
{
    this->data = data;
    next = nullptr;
    prev = nullptr;
}

// constructor
//  Complexity: O(1)
Keeper::Keeper()
{
    head = nullptr;
    tail = nullptr;
    count = 0;
}

// del
// Complexity: O(n)
Keeper::~Keeper()
{
    Node *current = head;
    while (current != nullptr)
    {
        Node *next = current->next;
        delete current;
        current = next;
    }
}

// add at the end
// Complexity: O(1)
void Keeper::pushBack(Entry data)
{
    Node *node = new Node(data);
    if (head == nullptr)
    {
        head = node;
        tail = node;
    }
    else
    {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    count++;
}

// parsear
// open the fiñle and read
// Complexity: O(n)
bool Keeper::read(string name)
{
    ifstream file(name);
    if (!file.is_open())
    {
        cout << "Could not open " << name << endl;
        return false;
    }

    string month, time, ip, message;
    int day;
    while (file >> month >> day >> time >> ip)
    {
        getline(file, message);
        pushBack(Entry(month, day, time, ip, message));
    }
    file.close();
    return true;
}

// size
// Complexity: O(1)
int Keeper::size()
{
    return count;
}
