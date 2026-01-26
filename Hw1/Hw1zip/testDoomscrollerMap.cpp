#include "DoomscrollerMap.h"
#include <cassert>
#include <iostream>
using namespace std;

void populate(DoomscrollerMap& d)
{
    assert(d.addDoomscroller("Alice"));
    assert(d.addDoomscroller("Bob"));
    assert(d.doomscroll("Alice", 3));
    assert(d.doomscroll("Bob", 5));
}

int main()
{
    // -------- Copy constructor test --------
    DoomscrollerMap original;
    populate(original);

    DoomscrollerMap copy(original);   // copy constructor

    // Modify copy
    assert(copy.doomscroll("Alice", 2));

    // Original should NOT change
    assert(original.hoursSpent("Alice") == 3);
    assert(copy.hoursSpent("Alice") == 5);

    // -------- Assignment operator test --------
    DoomscrollerMap assigned;
    assigned = original;              // assignment operator

    // Modify assigned
    assert(assigned.doomscroll("Bob", 10));

    // Original should NOT change
    assert(original.hoursSpent("Bob") == 5);
    assert(assigned.hoursSpent("Bob") == 15);

    // -------- Self-assignment test --------
    assigned = assigned;
    assert(assigned.hoursSpent("Bob") == 15);

    // -------- Destructor test (scope exit) --------
    {
        DoomscrollerMap temp;
        populate(temp);
        assert(temp.numDoomscrollers() == 2);
    } // temp destroyed here — should not affect others

    // Ensure original still intact after temp destruction
    assert(original.numDoomscrollers() == 2);
    assert(original.hoursSpent("Alice") == 3);
    assert(original.hoursSpent("Bob") == 5);

    cerr << "All Rule-of-Three tests passed!" << endl;
    return 0;
}
