// The words for and while must not appear in DoomscrollerMap.h or DoomscrollerMap.cpp, except in the implementation of DoomscrollerMap::print if you wish
// The characters [ (open square bracket) and * must not appear in DoomscrollerMap.h or DoomscrollerMap.cpp, except in comments if you wish
// You do not have to change std::string to KeyType and double to ValueType in DoomscrollerMap.h and DoomscrollerMap.cpp if you don't want to (since unlike Map, which is designed for a wide variety of key and value types, DoomscrollerMap is specifically designed to work with strings and doubles).
// In the code you turn in, DoomscrollerMap's member functions must not call Map::dump.

#include "DoomscrollerMap.h"
#include <string>
#include <iostream>

DoomscrollerMap::DoomscrollerMap() {
}

bool DoomscrollerMap::addDoomscroller(std::string name) {
	return m_map.insert(name, 0.0); 
}
// If a person with the specified name is not currently in the map, 
// and there is room in the map, add an entry for that person
// recording that they have spent 0 hours doomscrolling, and return
// true.  Otherwise, make no change to the map and return false.

double DoomscrollerMap::hoursSpent(std::string name) const {
	if (!m_map.contains(name))
		return -1;
	double v;
	m_map.get(name, v);
	return v;
}
// If a person with the specified name is in the map, return how
// many hours they have spent doomscrolling; otherwise, return -1.

bool DoomscrollerMap::doomscroll(std::string name, double hours) {
	if (hours < 0 || !m_map.contains(name))
		return false;
	double v;
	m_map.get(name, v);
	m_map.update(name, v + hours);
	return true;
}
// If no person with the specified name is in the map or if hours
// is negative, make no change to the map and return false.
// Otherwise, increase by the hours parameter the number of hours
// the indicated person has spent doomscrolling and return true.

int DoomscrollerMap::numDoomscrollers() const {
	return m_map.size();
}
// Return the number of people in the DoomscrollerMap.

void DoomscrollerMap::print() const {
	for (int i = 0; i < m_map.size(); i++) {
		std::string name;
		double hours;
		m_map.get(i, name, hours);
		std::cout << name << " " << hours << std::endl;
	}
}
// Write to cout one line for every person in the map.  Each line
// consists of the person's name, followed by one space, followed by
// the number of hours that person has spent doomscrolling.  Write
// no other text.  The lines need not be in any particular order.
