#include "newMap.h"
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

    cout << "Passed all tests" << endl;
}

//#include "newMap.h"
//#include "DoomscrollerMap.h"
//#include <cassert>
//#include <string>
//#include <iostream>
//using namespace std;
//
//void testCapacityAndInsert()
//{
//    Map a(1000);
//    Map b(5);
//    Map c;   // DEFAULT_MAX_ITEMS
//
//    KeyType k[6] = { "A", "B", "C", "D", "E", "F" };
//    ValueType v = 3.14;
//
//    for (int i = 0; i < 5; i++)
//        assert(b.insert(k[i], v));
//
//    assert(!b.insert(k[5], v));  // capacity exceeded
//}
//
//void testSwapCapacity()
//{
//    Map a(1000);
//    Map b(5);
//
//    KeyType k[6] = { "A", "B", "C", "D", "E", "F" };
//    ValueType v = 2.71;
//
//    for (int i = 0; i < 5; i++)
//        assert(b.insert(k[i], v));
//
//    a.swap(b);
//
//    // capacities must be swapped
//    assert(!a.insert(k[5], v));
//    assert(b.insert(k[5], v));
//}
//
//void testCopyConstructor()
//{
//    Map original(5);
//    original.insert("x", 1.0);
//    original.insert("y", 2.0);
//
//    Map copy(original);   // copy constructor
//
//    ValueType v;
//    assert(copy.get("x", v) && v == 1.0);
//
//    copy.update("x", 100.0);
//
//    // original must not change
//    assert(original.get("x", v) && v == 1.0);
//}
//
//void testAssignmentOperator()
//{
//    Map a(5);
//    a.insert("a", 10);
//    a.insert("b", 20);
//
//    Map b(10);
//    b.insert("z", 99);
//
//    b = a;   // assignment
//
//    ValueType v;
//    assert(b.get("a", v) && v == 10);
//    assert(b.size() == 2);
//
//    b.update("a", 999);
//
//    // a must not change
//    assert(a.get("a", v) && v == 10);
//
//    // self-assignment safety
//    b = b;
//    assert(b.get("a", v) && v == 999);
//}
//
//void testDestructorViaScope()
//{
//    Map survivor(5);
//    survivor.insert("safe", 1);
//
//    {
//        Map temp(5);
//        temp.insert("temp", 42);
//    } // temp destroyed here
//
//    ValueType v;
//    assert(survivor.get("safe", v) && v == 1);
//}
//
//void testGetByIndex()
//{
//    Map m(5);
//    m.insert("a", 1);
//    m.insert("b", 2);
//    m.insert("c", 3);
//
//    KeyType k;
//    ValueType v;
//
//    assert(m.get(0, k, v));
//    assert(m.get(1, k, v));
//    assert(m.get(2, k, v));
//    assert(!m.get(3, k, v));
//}
//
//void testErase()
//{
//    Map m(5);
//    m.insert("a", 1);
//    m.insert("b", 2);
//
//    assert(m.erase("a"));
//    assert(!m.contains("a"));
//    assert(m.size() == 1);
//
//    assert(!m.erase("a"));  // already gone
//}
//
//void testDoomscrollerCompatibility()
//{
//    DoomscrollerMap d;
//    assert(d.addDoomscroller("Alice"));
//    assert(d.addDoomscroller("Bob"));
//
//    assert(d.doomscroll("Alice", 2.5));
//    assert(d.doomscroll("Bob", 5));
//
//    assert(d.hoursSpent("Alice") == 2.5);
//    assert(d.hoursSpent("Bob") == 5);
//
//    assert(d.numDoomscrollers() == 2);
//}
//
//int main()
//{
//    testCapacityAndInsert();
//    testSwapCapacity();
//    testCopyConstructor();
//    testAssignmentOperator();
//    testDestructorViaScope();
//    testGetByIndex();
//    testErase();
//    testDoomscrollerCompatibility();
//
//    cout << "All newMap tests passed successfully!" << endl;
//    return 0;
//}