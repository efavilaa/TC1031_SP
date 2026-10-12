#ifndef ENTRY_H
#define ENTRY_H
#include <string>
using namespace std;

// Parsear
struct Entry
{
    string month;
    int day;
    string time;
    string ip;
    string message;
    // mont day hour minute second
    long long timestamp;

    Entry();
    Entry(string month, int day, string time, string ip, string message);

    string toString() const;
    bool operator<(const Entry &other) const;
};

#endif
