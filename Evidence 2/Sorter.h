#ifndef SORTER_H
#define SORTER_H
#include "Keeper.h"

// Ordena los registros del Keeper por IP (ascendente) y, para una misma IP,
// por fecha y hora (ascendente). 
// 
// Usa un quicksort iterativo para evitar la recursion como en la presentacion de clase.

class Sorter
{
public:
    // Solo creo el metodo 
    void sort(Keeper &keeper);
};

#endif
