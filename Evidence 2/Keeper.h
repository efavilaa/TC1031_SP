#ifndef KEEPER_H
#define KEEPER_H
#include <string>
#include "Entry.h"
using namespace std;

//double linked list
//w3e use a struct instead of two vectors because we want to keep them clean
//major change since evidence 1
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
