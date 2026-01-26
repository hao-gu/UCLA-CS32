#include "newMap.h"
#include "DoomscrollerMap.h"
#include <cassert>
#include <string>
#include <iostream>
using namespace std;

void testCapacityAndInsert()
{
    Map a(1000);
    Map b(5);
    Map c;   // DEFAULT_MAX_ITEMS

    KeyType k[6] = { "A", "B", "C", "D", "E", "F" };
    ValueType v = 3.14;

    for (int i = 0; i < 5; i++)
        assert(b.insert(k[i], v));

    assert(!b.insert(k[5], v));  // capacity exceeded
}

void testSwapCapacity()
{
    Map a(1000);
    Map b(5);

    KeyType k[6] = { "A", "B", "C", "D", "E", "F" };
    ValueType v = 2.71;

    for (int i = 0; i < 5; i++)
        assert(b.insert(k[i], v));

    a.swap(b);

    // capacities must be swapped
    assert(!a.insert(k[5], v));
    assert(b.insert(k[5], v));
}

void testCopyConstructor()
{
    Map original(5);
    original.insert("x", 1.0);
    original.insert("y", 2.0);

    Map copy(original);   // copy constructor

    ValueType v;
    assert(copy.get("x", v) && v == 1.0);

    copy.update("x", 100.0);

    // original must not change
    assert(original.get("x", v) && v == 1.0);
}

void testAssignmentOperator()
{
    Map a(5);
    a.insert("a", 10);
    a.insert("b", 20);

    Map b(10);
    b.insert("z", 99);

    b = a;   // assignment

    ValueType v;
    assert(b.get("a", v) && v == 10);
    assert(b.size() == 2);

    b.update("a", 999);

    // a must not change
    assert(a.get("a", v) && v == 10);

    // self-assignment safety
    b = b;
    assert(b.get("a", v) && v == 999);
}

void testDestructorViaScope()
{
    Map survivor(5);
    survivor.insert("safe", 1);

    {
        Map temp(5);
        temp.insert("temp", 42);
    } // temp destroyed here

    ValueType v;
    assert(survivor.get("safe", v) && v == 1);
}

void testGetByIndex()
{
    Map m(5);
    m.insert("a", 1);
    m.insert("b", 2);
    m.insert("c", 3);

    KeyType k;
    ValueType v;

    assert(m.get(0, k, v));
    assert(m.get(1, k, v));
    assert(m.get(2, k, v));
    assert(!m.get(3, k, v));
}

void testErase()
{
    Map m(5);
    m.insert("a", 1);
    m.insert("b", 2);

    assert(m.erase("a"));
    assert(!m.contains("a"));
    assert(m.size() == 1);

    assert(!m.erase("a"));  // already gone
}

void testDoomscrollerCompatibility()
{
    DoomscrollerMap d;
    assert(d.addDoomscroller("Alice"));
    assert(d.addDoomscroller("Bob"));

    assert(d.doomscroll("Alice", 2.5));
    assert(d.doomscroll("Bob", 5));

    assert(d.hoursSpent("Alice") == 2.5);
    assert(d.hoursSpent("Bob") == 5);

    assert(d.numDoomscrollers() == 2);
}

int main()
{
    testCapacityAndInsert();
    testSwapCapacity();
    testCopyConstructor();
    testAssignmentOperator();
    testDestructorViaScope();
    testGetByIndex();
    testErase();
    testDoomscrollerCompatibility();

    cerr << "All newMap tests passed successfully!" << endl;
    return 0;
}