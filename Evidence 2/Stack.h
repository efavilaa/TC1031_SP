#ifndef STACK_H
#define STACK_H
#include "Keeper.h"

// Pila hecha con una lista simplemente ligada.
// El inicio de la lista es el tope: push inserta al inicio y pop quita el primero.
// Cada elemento guarda las direcciones del nodo inicial y final de una
// particion que falta ordenar (en vez de usar llamadas recursivas).

struct StackNode
{
    Node *low;
    Node *high;
    StackNode *next;

    StackNode(Node *low, Node *high, StackNode *next);
};

class Stack
{
private:
    StackNode *top;

public:
    Stack();
    ~Stack();

    void push(Node *low, Node *high);
    bool pop(Node *&low, Node *&high);
    bool isEmpty();
};

#endif
