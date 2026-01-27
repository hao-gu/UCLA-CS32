#include "Map.h"
#include <cassert>
#include <iostream>
using namespace std;

void testEmptyAndSize()
{
    Map m;
    assert(m.empty());
    assert(m.size() == 0);
}

void testInsert()
{
    Map m;
    assert(m.insert("a", 1.0));
    assert(m.insert("b", 2.0));
    assert(!m.insert("a", 3.0));   // duplicate key
    assert(m.size() == 2);
}

void testContainsAndGetByKey()
{
    Map m;
    m.insert("x", 10.5);

    double v = 0;
    assert(m.contains("x"));
    assert(!m.contains("y"));
    assert(m.get("x", v) && v == 10.5);
    assert(!m.get("y", v));
}

void testUpdate()
{
    Map m;
    m.insert("a", 1.0);
    assert(m.update("a", 2.0));
    assert(!m.update("b", 3.0));

    double v;
    m.get("a", v);
    assert(v == 2.0);
}

void testInsertOrUpdate()
{
    Map m;
    assert(m.insertOrUpdate("a", 1.0));  // insert
    assert(m.insertOrUpdate("a", 2.0));  // update

    double v;
    m.get("a", v);
    assert(v == 2.0);
}

void testErase()
{
    Map m;
    m.insert("a", 1.0);
    m.insert("b", 2.0);

    assert(m.erase("a"));
    assert(!m.erase("a"));   // already gone
    assert(m.size() == 1);
    assert(!m.contains("a"));
    assert(m.contains("b"));
}

void testGetByIndex()
{
    Map m;
    m.insert("a", 1.0);
    m.insert("b", 2.0);
    m.insert("c", 3.0);

    string k;
    double v;

    for (int i = 0; i < m.size(); i++) {
        assert(m.get(i, k, v));
        assert(m.contains(k));
    }

    assert(!m.get(-1, k, v));
    assert(!m.get(m.size(), k, v));
}

void testSwap()
{
    Map m1;
    Map m2;

    m1.insert("a", 1.0);
    m1.insert("b", 2.0);
    m2.insert("x", 9.0);

    m1.swap(m2);

    assert(m1.size() == 1);
    assert(m2.size() == 2);

    assert(m1.contains("x"));
    assert(!m1.contains("a"));

    assert(m2.contains("a"));
    assert(m2.contains("b"));
}

void testCopyConstructor()
{
    Map m1;
    m1.insert("a", 1.0);
    m1.insert("b", 2.0);

    Map m2(m1);   // copy

    assert(m2.size() == 2);
    assert(m2.contains("a"));
    assert(m2.contains("b"));

    m1.erase("a");
    assert(m2.contains("a"));   // deep copy check
}

void testAssignmentOperator()
{
    Map m1;
    m1.insert("a", 1.0);

    Map m2;
    m2.insert("x", 9.0);

    m2 = m1;

    assert(m2.size() == 1);
    assert(m2.contains("a"));
    assert(!m2.contains("x"));
}

int main()
{
    testEmptyAndSize();
    testInsert();
    testContainsAndGetByKey();
    testUpdate();
    testInsertOrUpdate();
    testErase();
    testGetByIndex();
    testSwap();
    testCopyConstructor();
    testAssignmentOperator();

    cout << "All Map tests passed!" << endl;
}
