#include "Keeper.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

// We convert the month to numbre so it works
// Complexity: O(1)
int Keeper::monthToNumber(string month)
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

// Reading the file and storing it
bool Keeper::read(string fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        cout << "Crashed" << fileName << endl;
        return false;
    }

    string month, time, ip, reason;
    int day;
    while (file >> month >> day >> time >> ip)
    {
        getline(file, reason);

        int hour = stoi(time.substr(0, 2));
        int minute = stoi(time.substr(3, 2));
        int second = stoi(time.substr(6, 2));

        // we save date as a number so we can compare them easy
        int date = monthToNumber(month) * 100000000 + day * 1000000 + hour * 10000 + minute * 100 + second;

        lines.push_back(month + " " + to_string(day) + " " + time + " " + ip + reason);
        dates.push_back(date);
    }
    file.close();
    return true;
}

// Sort by date with merge sort
// Complexity: O(n log n)
void Keeper::sorting()
{
    mergeSort(0, dates.size() - 1);
}

// merge sort dividing part
// Complexity: O(n log n)
void Keeper::mergeSort(int left, int right)
{
    if (left >= right)
    {
        return;
    }
    int mid = (left + right) / 2;
    mergeSort(left, mid);
    mergeSort(mid + 1, right);
    merge(left, mid, right);
}

// calls merge sort and does the merging
// Complexity: O(n)
void Keeper::merge(int left, int mid, int right)
{
    vector<int> tempDates;
    vector<string> tempLines;
    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right)
    {
        if (dates[i] <= dates[j])
        {
            tempDates.push_back(dates[i]);
            tempLines.push_back(lines[i]);
            i++;
        }
        else
        {
            tempDates.push_back(dates[j]);
            tempLines.push_back(lines[j]);
            j++;
        }
    }
    while (i <= mid)
    {
        tempDates.push_back(dates[i]);
        tempLines.push_back(lines[i]);
        i++;
    }
    while (j <= right)
    {
        tempDates.push_back(dates[j]);
        tempLines.push_back(lines[j]);
        j++;
    }

    for (int k = 0; k < tempDates.size(); k++)
    {
        dates[left + k] = tempDates[k];
        lines[left + k] = tempLines[k];
    }
}

// Binary search
// Complexity: O(log n)
int Keeper::binarySearch(int key)
{
    int low = 0;
    int high = dates.size();
    while (low < high)
    {
        int mid = (low + high) / 2;
        if (dates[mid] < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid;
        }
    }
    return low;
}

// dates and prints
// Complexity: O(log n + k)
void Keeper::searchRecords()
{
    int start, end;

    cout << "Start date (MMDDyyyy): ";
    cin >> start;
    cout << "End date (MMDDyyyy): ";
    cin >> end;

    // we delete the year
    int startMonthDay = start / 10000;
    int endMonthDay = end / 10000;
    // swe use all seconds
    int startKey = startMonthDay * 1000000;
    int endKey = endMonthDay * 1000000 + 235959;

    if (start < 1010000 || end < 1010000 || startKey > endKey)
    {
        cout << "Range does not exist" << endl;
        return;
    }

    results.clear();
    int first = binarySearch(startKey);
    int last = binarySearch(endKey + 1);
    for (int i = first; i < last; i++)
    {
        results.push_back(lines[i]);
        cout << lines[i] << endl;
    }
    cout << results.size() << " records found." << endl;
}

// Output
// Complexity: O(n)
bool Keeper::store(string fileName)
{
    ofstream file(fileName);
    if (!file.is_open())
    {
        cout << "Could not create " << fileName << endl;
        return false;
    }
    for (int i = 0; i < results.size(); i++)
    {
        file << results[i] << endl;
    }
    file.close();
    cout << "Results saved to " << fileName << endl;
    return true;
}

// how many lines.
// Complexity: O(1)
int Keeper::size()
{
    return lines.size();
}
