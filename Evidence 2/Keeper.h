#ifndef KEEPER_H
#define KEEPER_H
#include <string>
#include "Entry.h"
using namespace std;

// node of the doubly linked list
struct Node
{
    Entry data;
    Node *next;
    Node *prev;

    Node(Entry data);
};

class Keeper
{
private:
    Node *head;
    Node *tail;
    int count;

    void pushBack(Entry data);

public:
    Keeper();
    ~Keeper();

    bool read(string name);
    int size();
};

#endif
