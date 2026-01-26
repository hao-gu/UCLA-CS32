#ifndef DOOMSCROLLERMAP_H
#define DOOMSCROLLERMAP_H


#include "newMap.h"
#include <string>

class DoomscrollerMap
{
public:
    DoomscrollerMap();       // Create an empty doomscroller map.

    bool addDoomscroller(std::string name);
    // If a person with the specified name is not currently in the map, 
    // and there is room in the map, add an entry for that person
    // recording that they have spent 0 hours doomscrolling, and return
    // true.  Otherwise, make no change to the map and return false.

    double hoursSpent(std::string name) const;
    // If a person with the specified name is in the map, return how
    // many hours they have spent doomscrolling; otherwise, return -1.

    bool doomscroll(std::string name, double hours);
    // If no person with the specified name is in the map or if hours
    // is negative, make no change to the map and return false.
    // Otherwise, increase by the hours parameter the number of hours
    // the indicated person has spent doomscrolling and return true.

    int numDoomscrollers() const;
    // Return the number of people in the DoomscrollerMap.

    void print() const;
    // Write to cout one line for every person in the map.  Each line
    // consists of the person's name, followed by one space, followed by
    // the number of hours that person has spent doomscrolling.  Write
    // no other text.  The lines need not be in any particular order.

private:
    Map m_map;
};

#endif