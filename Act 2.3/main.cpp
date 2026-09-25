#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Keeper.h"

using namespace std;

int main()
{
    Keeper keeper;

    if (!keeper.read("bitacora.txt"))
    {
        return -1;
    }
    cout << keeper.size() << " records read" << endl;

    keeper.sorting();
    keeper.searchRecords();
    keeper.store("resultados.txt");

    return 0;
}
