#include "Map.h"
#include <cassert>
#include <iostream>
using namespace std;

void testBasicOperations()
{
    Map m;
    assert(m.empty());
    assert(m.size() == 0);

    // insert
    assert(m.insert("ten", 10.0));
    assert(m.insert("twenty", 20.0));
    assert(!m.insert("ten", 100.0));  // duplicate key
    assert(m.size() == 2);
    assert(!m.empty());

    // contains
    assert(m.contains("ten"));
    assert(!m.contains("thirty"));

    // get by key
    ValueType v = -1.0;
    assert(m.get("ten", v) && v == 10.0);
    assert(!m.get("thirty", v) && v == 10.0); // v unchanged on failure
}

void testGetByIndex()
{
    Map m;
    m.insert("thirty", 30.0);
    m.insert("ten", 10.0);
    m.insert("twenty", 20.0);

    KeyType k;
    ValueType v;

    // Keys should be ordered lexicographically: ten, thirty, twenty
    assert(m.get(0, k, v) && k == "ten" && v == 10.0);
    assert(m.get(1, k, v) && k == "thirty" && v == 30.0);
    assert(m.get(2, k, v) && k == "twenty" && v == 20.0);
    assert(!m.get(3, k, v));
}

void testUpdateErase()
{
    Map m;
    m.insert("one", 1.0);
    m.insert("two", 2.0);

    assert(m.update("one", 100.0));
    ValueType v;
    assert(m.get("one", v) && v == 100.0);

    assert(m.erase("one"));
    assert(!m.contains("one"));
    assert(m.size() == 1);
    assert(!m.erase("one"));  // already erased
}

void testCopyAndAssignment()
{
    Map m1;
    m1.insert("one", 1.0);
    m1.insert("two", 2.0);

    // Copy constructor
    Map m2(m1);
    assert(m2.size() == 2);
    ValueType v;
    assert(m2.get("one", v) && v == 1.0);

    // Assignment operator
    Map m3;
    m3 = m1;
    assert(m3.size() == 2);
    assert(m3.get("two", v) && v == 2.0);

    // Ensure deep copy behavior
    m1.erase("one");
    assert(m1.size() == 1);
    assert(m2.size() == 2);
    assert(m3.size() == 2);
}

void testConstCorrectness(const Map& m)
{
    assert(m.size() == 2);
    assert(m.contains("one"));
}

int main()
{
    testBasicOperations();
    testGetByIndex();
    testUpdateErase();
    testCopyAndAssignment();

    Map m;
    m.insert("one", 1.0);
    m.insert("two", 2.0);
    testConstCorrectness(m);

    cerr << "Passed all tests" << endl;
}