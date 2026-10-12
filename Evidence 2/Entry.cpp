#include "Entry.h"

// month to number (funcion aparte)
//  Complexity: O(1)
static int monthToNumber(string month)
{
    string names[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++)
    {
        if (names[i] == month)
        {
            return i + 1;
        }
    }
    return 0;
}

// cosntructor
//  Complexity: O(1)
Entry::Entry()
{
    day = 0;
    timestamp = 0;
}

// remove :
// Complexity: O(1)
Entry::Entry(string month, int day, string time, string ip, string message)
{
    this->month = month;
    this->day = day;
    this->time = time;
    this->ip = ip;
    this->message = message;

    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));
    int second = stoi(time.substr(6, 2));

    timestamp = monthToNumber(month) * 100000000LL + day * 1000000LL + hour * 10000 + minute * 100 + second;
}

// record organized
//  Complexity: O(1)
string Entry::toString() const
{
    return month + " " + to_string(day) + " " + time + " " + ip + message;
}

// compare by date (later for sorter)
//  Complexity: O(1)
bool Entry::operator<(const Entry &other) const
{
    return timestamp < other.timestamp;
}
