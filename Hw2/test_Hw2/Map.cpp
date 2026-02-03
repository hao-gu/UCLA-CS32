// Map.cpp

#include "Map.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Map::Map(int capacity)
    : m_size(0), m_capacity(capacity)
{
    if (capacity < 0)
    {
        cout << "A Map capacity must not be negative." << endl;
        exit(1);
    }
    m_data = new Pair[m_capacity];
}

Map::Map(const Map& other)
    : m_size(other.m_size), m_capacity(other.m_capacity)
{
    m_data = new Pair[m_capacity];
    for (int k = 0; k < m_size; k++)
        m_data[k] = other.m_data[k];
}

Map::~Map()
{
    delete[] m_data;
}

Map& Map::operator=(const Map& rhs)
{
    if (this != &rhs)
    {
        Map temp(rhs);
        swap(temp);
    }
    return *this;
}

bool Map::erase(const KeyType& key)
{
    int pos = findFirstAtLeast(key);
    if (pos == m_size || m_data[pos].m_key != key)  // not found
        return false;

    // Shift pairs left to replace the one erased

    for (; pos + 1 < m_size; pos++)
        m_data[pos] = m_data[pos + 1];
    m_size--;
    return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    int pos = findFirstAtLeast(key);
    if (pos == m_size || m_data[pos].m_key != key)  // not found
        return false;
    value = m_data[pos].m_value;
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0 || i >= m_size)
        return false;
    key = m_data[i].m_key;
    value = m_data[i].m_value;
    return true;
}

void Map::swap(Map& other)
{
    // Swap the m_data pointers to dynamic arrays.

    Pair* tempData = m_data;
    m_data = other.m_data;
    other.m_data = tempData;

    // Swap sizes

    int t = m_size;
    m_size = other.m_size;
    other.m_size = t;

    // Swap capacities.

    t = m_capacity;
    m_capacity = other.m_capacity;
    other.m_capacity = t;
}

int Map::findFirstAtLeast(const KeyType& key) const
{
    // Do a binary search through the array.

    int begin = 0;
    int end = m_size;
    while (begin < end)
    {
        int mid = (begin + end) / 2;
        if (key < m_data[mid].m_key)
            end = mid;
        else if (m_data[mid].m_key < key)
            begin = mid + 1;
        else
            return mid;
    }
    return begin;
}

bool Map::doInsertOrUpdate(const KeyType& key, const ValueType& value,
    bool mayInsert, bool mayUpdate)
{
    int pos = findFirstAtLeast(key);
    if (pos < m_size&& m_data[pos].m_key == key)  // found
    {
        if (mayUpdate)
            m_data[pos].m_value = value;
        return mayUpdate;
    }
    if (!mayInsert)  // not found, and not allowed to insert
        return false;
    if (m_size == m_capacity)  // no room to insert
        return false;

    // Shift pairs right to make room for the one to be inserted

    for (int i = m_size; i > pos; i--)
        m_data[i] = m_data[i - 1];
    m_data[pos].m_key = key;
    m_data[pos].m_value = value;
    m_size++;
    return true;
}