#include "Searcher.h"
#include <fstream>
#include <iostream>
#include <utility>

// constructor
// Complejidad: O(1)
Searcher::Searcher()
{
    startKey = 0;
    endKey = 0;
    first = nullptr;
    last = nullptr;
    count = 0;
}

// Convierte "a.b.c.d" (o "a.b.c.d:puerto") en un numero comparable.

long long Searcher::ipToKey(string ip)
{
    long long key = 0;
    int octet = 0;
    int digits = 0;
    int octets = 0;
    for (size_t i = 0; i <= ip.size(); i++)
    {
        // el final del texto o ':' (inicio del puerto) cierran la ultima división
        bool endOfIp = (i == ip.size() || ip[i] == ':');
        if (endOfIp || ip[i] == '.')
        {
            if (digits == 0 || octets == 4)
            {
                return -1;
            }
            key = key * 1000 + octet;
            octets++;
            octet = 0;
            digits = 0;
            if (endOfIp)
            {
                break;
            }
        }
        else if (ip[i] >= '0' && ip[i] <= '9' && digits < 3)
        {
            octet = octet * 10 + (ip[i] - '0');
            digits++;
        }
        else
        {
            return -1;
        }
    }
    if (octets != 4)
    {
        return -1;
    }
    return key;
}

// Pide una IP hasta que el usuario escriba una valida
// Regresa false solo si ya no hay entrada o sea se acaba el archivo
bool Searcher::askIp(string message, string &ip)
{
    while (true)
    {
        cout << message;
        if (!(cin >> ip))
        {
            return false;
        }
        if (ipToKey(ip) != -1)
        {
            return true;
        }
        cout << "Invalid IP, use the format a.b.c.d (e.g. 10.15.176.92)" << endl;
    }
}

// Pide la IP inicial y final de la busqueda
// Si el usuario las da al reves, se intercambian en vez de regresar 0 resultados
bool Searcher::askRange()
{
    if (!askIp("Start IP: ", startIp) || !askIp("End IP: ", endIp))
    {
        cout << "No IP range was given" << endl;
        return false;
    }
    startKey = ipToKey(startIp);
    endKey = ipToKey(endIp);
    if (startKey > endKey)
    {
        swap(startKey, endKey);
        swap(startIp, endIp);
        cout << "Start IP was greater than end IP, the range was swapped" << endl;
    }
    return true;
}

// Busca los registros con IP entre startIp y endIp 

void Searcher::search(Keeper &keeper)
{
    first = nullptr;
    last = nullptr;
    count = 0;

    Node *node = keeper.head;
    while (node != nullptr && ipToKey(node->data.ip) < startKey)
    {
        node = node->next;
    }
    while (node != nullptr && ipToKey(node->data.ip) <= endKey)
    {
        if (first == nullptr)
        {
            first = node;
        }
        last = node;
        count++;
        node = node->next;
    }
}

// Muestra en consola los registros encontrados
void Searcher::display()
{
    Node *node = first;
    for (int i = 0; i < count; i++)
    {
        cout << node->data.toString() << endl;
        node = node->next;
    }
    cout << count << " records found between " << startIp << " and " << endIp << endl;
}

// Guarda los registros encontrados en el archivo de salida
bool Searcher::store(string name)
{
    ofstream file(name);
    if (!file.is_open())
    {
        cout << "Could not create " << name << endl;
        return false;
    }
    Node *node = first;
    for (int i = 0; i < count; i++)
    {
        file << node->data.toString() << endl;
        node = node->next;
    }
    file.close();
    cout << "Results saved to " << name << endl;
    return true;
}

// cuantos registros se encontraron
int Searcher::size()
{
    return count;
}
