#ifndef SORTER_H
#define SORTER_H
#include "Keeper.h"

// Ordena los registros del Keeper por IP (ascendente) y, para una misma IP,
// por fecha y hora (ascendente). 
// 
// Usa un quicksort iterativo directamente sobre la lista doblemente ligada,
// con una pila (Stack) en lugar de recursion, como en la presentacion de clase.

class Sorter
{
public:
    // Solo creo el metodo 
    void sort(Keeper &keeper);
};

#endif
