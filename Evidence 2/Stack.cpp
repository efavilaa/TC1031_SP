#include "Stack.h"

// constructor del nodo
// Complejidad: O(1)
StackNode::StackNode(Node *low, Node *high, StackNode *next)
{
    this->low = low;
    this->high = high;
    this->next = next;
}

// constructor
// Complejidad: O(1)
Stack::Stack()
{
    top = nullptr;
}

// libera los nodos que hayan quedado
// Complejidad: O(n)
Stack::~Stack()
{
    while (top != nullptr)
    {
        StackNode *next = top->next;
        delete top;
        top = next;
    }
}

// inserta al inicio de la lista (nuevo tope)
// Complejidad: O(1)
void Stack::push(Node *low, Node *high)
{
    top = new StackNode(low, high, top);
}

// quita el primer elemento y regresa su rango en low y high
// Regresa false si la pila esta vacia
// Complejidad: O(1)
bool Stack::pop(Node *&low, Node *&high)
{
    if (top == nullptr)
    {
        return false;
    }
    StackNode *old = top;
    low = old->low;
    high = old->high;
    top = old->next;
    delete old;
    return true;
}

// Complejidad: O(1)
bool Stack::isEmpty()
{
    return top == nullptr;
}
