#include "Sorter.h"
#include "Stack.h"
#include <utility>

// Convierte "a.b.c.d:puerto" en un numero comparable octeto por octeto (el puerto se ignora).

// Porque hice esto?: porque si comparo la IP como string daria un orden incorrecto ("10.x" < "9.x"),
// y toca tener en cuenta los valores mayores a 255 por eso uso base 1000 porque si no estarian haciendo una
// especie de "overlapping".

// Complejidad: O(1)

static long long ipToNumber(const string &ip)
{
    long long result = 0;
    long long octet = 0;
    for (size_t i = 0; i < ip.size(); i++)
    {
        char c = ip[i];
        if (c >= '0' && c <= '9')
        {
            octet = octet * 10 + (c - '0');
        }
        else if (c == '.')
        {
            result = result * 1000 + octet;
            octet = 0;
        }
        else
        {
            break; // ':' marca el inicio del puerto, lo saltamos
        }
    }
    return result * 1000 + octet;
}

// a va antes que b? (primero por IP, y si son iguales, cronologicamente)

// Porque hice esto?: se separa el criterio en una funcion para que el quicksort
// no dependa de como se ordena.

// Complejidad: O(1)

static bool lessThan(const Entry &a, const Entry &b)
{
    long long ipA = ipToNumber(a.ip);
    long long ipB = ipToNumber(b.ip);
    if (ipA != ipB)
    {
        return ipA < ipB;
    }
    return a.timestamp < b.timestamp;
}

// Particion entre dos nodos de la lista doblemente ligada (low y high).

// Se usa el ultimo nodo (high) como pivote. i marca el ultimo nodo que ya
// es menor que el pivote; j recorre de low hasta antes de high. Cuando j es
// menor que el pivote, i avanza y se intercambian los datos de i y j.
// Al final el pivote se pone despues de i, que es su lugar definitivo.

// Solo se intercambian los datos (data) de los nodos, los punteros next y prev
// no cambian, asi la lista sigue igual de conectada.

// Complejidad: O(n) donde n es el tamano de la particion

static Node *partition(Node *low, Node *high)
{
    Node *i = low->prev;
    for (Node *j = low; j != high; j = j->next)
    {
        if (lessThan(j->data, high->data))
        {
            i = (i == nullptr) ? low : i->next;
            swap(i->data, j->data);
        }
    }
    i = (i == nullptr) ? low : i->next;
    swap(i->data, high->data);
    return i;
}

// Quicksort iterativo sobre la lista doblemente ligada

// Decisiones de diseno:

/* - Iterativo con nuestra pila (Stack): en vez de llamarse a si mismo, guarda
 en la pila las direcciones del nodo inicial y final de cada particion pendiente.
    Asi tambien se evita desbordar la pila de llamadas con la recursion.*/

// - Solo se meten a la pila particiones con 2 o mas nodos.

// Complejidad: O(n log n) en promedio, O(n^2) en el peor caso
// (cuando el pivote siempre queda en un extremo, ej. si la lista ya viene ordenada).

void Sorter::sort(Keeper &keeper)
{
    if (keeper.head == nullptr || keeper.head == keeper.tail)
    {
        return;
    }

    Stack pending;
    pending.push(keeper.head, keeper.tail);

    Node *low;
    Node *high;
    while (pending.pop(low, high))
    {
        if (low == high)
        {
            continue;
        }

        Node *pivot = partition(low, high);

        // lado izquierdo: de low hasta antes del pivote
        if (pivot != low)
        {
            pending.push(low, pivot->prev);
        }
        // lado derecho: despues del pivote hasta high
        if (pivot != high)
        {
            pending.push(pivot->next, high);
        }
    }
}
