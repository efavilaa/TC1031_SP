#include <iostream>
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

    return 0;
}
