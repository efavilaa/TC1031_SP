#ifndef SEARCHER_H
#define SEARCHER_H
#include <string>
#include "Keeper.h"
using namespace std;

// Pide al usuario un rango de IPs, busca los registros que caen dentro
// y los muestra / guarda con el formato:
// Month Day Hour:Minute:Second IP:port Access_attempt_description
// Trabaja sobre la lista que ya ordeno el Sorter (por IP y luego por fecha),
// por eso los registros del rango quedan juntos y salen en orden cronologico.

class Searcher
{
private:
    string startIp;
    string endIp;
    long long startKey;
    long long endKey;

    // el resultado es un tramo continuo de la lista: de first a last
    // (igual que L y R en el quicksort)
    Node *first;
    Node *last;
    int count;

    long long ipToKey(string ip);
    bool askIp(string message, string &ip);

public:
    Searcher();

    bool askRange();
    void search(Keeper &keeper);
    void display();
    bool store(string name);
    int size();
};

#endif
