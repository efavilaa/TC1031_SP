#include <iostream>
#include "Keeper.h"
#include "Sorter.h"
#include "Searcher.h"

using namespace std;

int main()
{
    Keeper keeper;

    if (!keeper.read("bitacora.txt"))
    {
        return -1;
    }
    cout << keeper.size() << " records read" << endl;

    Sorter sorter;
    sorter.sort(keeper);

    Searcher searcher;
    if (!searcher.askRange())
    {
        return -1;
    }
    searcher.search(keeper);
    searcher.display();
    searcher.store("output.txt");

    return 0;
}
