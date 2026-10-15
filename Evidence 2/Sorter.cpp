#include "Sorter.h"
#include <stack>
#include <vector>
#include <utility>

// Item junta cada registro con su IP y hora ya convertidas a número,
// para comparar rápido sin reconvertir el texto en cada comparación.

struct Item
{
    long long ip;
    long long timestamp;
    Entry entry;
};

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

static bool lessThan(const Item &a, const Item &b)
{
    if (a.ip != b.ip)
    {
        return a.ip < b.ip;
    }
    return a.timestamp < b.timestamp;
}

// Quicksort iterativo 

// Decisiones de diseno:

// - Iterativo con stack: es el requisito y evita desbordar la pila de llamadas
//   con una recursion.

// - Particion de Hoare: maneja bien muchas llaves repetidas
//   porque reparte los iguales entre ambos lados en vez de degradarse como Lomuto.

// Complejidad: O(n log n) en promedio, O(n^2) en el peor caso porque el pivote esta en el medio

static void quickSort(vector<Item> &items)
{
    if (items.size() < 2)
    {
        return;
    }

    // cada elemento de la pila es un rango (inicio, fin) que falta ordenar
    stack<pair<int, int>> ranges;
    ranges.push(make_pair(0, (int)items.size() - 1));

    while (!ranges.empty())
    {
        pair<int, int> range = ranges.top();
        ranges.pop();
        int low = range.first;
        int high = range.second;
        if (low >= high)
        {
            continue;
        }

        Item pivot = items[low + (high - low) / 2];
        int i = low;
        int j = high;
        while (i <= j)
        {
            while (lessThan(items[i], pivot))
            {
                i++;
            }
            while (lessThan(pivot, items[j]))
            {
                j--;
            }
            if (i <= j)
            {
                swap(items[i], items[j]);
                i++;
                j--;
            }
        }

        
        if (low < j)
        {
            ranges.push(make_pair(low, j));
        }
        if (i < high)
        {
            ranges.push(make_pair(i, high));
        }
    }
}

// Copia la lista a un vector porque el quicksort necesita acceder rapido
// a cualquier posicion y la linked list no lo permite.

// Complejidad: O(n log n) en promedio (copiar y reescribir solo cuestan O(n)).

void Sorter::sort(Keeper &keeper)
{
    vector<Item> items;
    items.reserve(keeper.count);
    for (Node *node = keeper.head; node != nullptr; node = node->next)
    {
        Item item;
        item.ip = ipToNumber(node->data.ip);
        item.timestamp = node->data.timestamp;
        item.entry = node->data;
        items.push_back(item);
    }

    quickSort(items);

    size_t k = 0;
    for (Node *node = keeper.head; node != nullptr; node = node->next)
    {
        node->data = items[k].entry;
        k++;
    }
}
