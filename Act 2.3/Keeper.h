#ifndef KEEPER_H
#define KEEPER_H
#include <string>
#include <vector>
using namespace std;

class Keeper
{
private:
    // we read each line
    vector<string> lines;
    vector<int> dates;
    vector<string> results;

    // functions
    int monthToNumber(string month);
    // using merge sort
    void mergeSort(int left, int right);
    // calls mergeSort
    void merge(int left, int mid, int right);
    // using binary search
    int binarySearch(int key);

public:
    // methods that we can call from anywehre
    bool read(string fileName);
    void sorting();
    void searchRecords();
    bool store(string fileName);
    int size();
};

#endif
