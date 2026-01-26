#include "Map.h"

Map::Map() : m_size(0) {}

bool Map::empty() const
{
	return m_size == 0;
}
int Map::size() const
{
	return m_size;
}
bool Map::insert(const KeyType& key, const ValueType& value) {
	if (m_size >= DEFAULT_MAX_ITEMS) return false;
	if (contains(key)) return false;

	m_map[m_size].key = key;
	m_map[m_size].value = value;
	m_size++;

	return true;
}

// If key is not equal to any key currently in the map and if the
// key/value Item can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).

bool Map::update(const KeyType& key, const ValueType& value) {
	for (int i = 0; i < m_size; i++) {
		if (m_map[i].key == key) {
			m_map[i].value = value;
			return true;
		}
	}
	return false;
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value that it currently maps to, but instead map to
// the value of the second parameter; in this case, return true.
// Otherwise, make no change to the map and return false.


bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
	if (update(key, value)) {
		return true;
	}
	else {
		return insert(key, value);
	}
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value that it currently maps to, but instead map to
// the value of the second parameter; in this case, return true.
// If key is not equal to any key that is currently in the map, and if the
// key/value Item can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).

bool Map::erase(const KeyType& key) {
	if (!contains(key)) return false;
	bool shift = false;
	for (int i = 0; i < m_size; i++) {
		if (m_map[i].key == key) {
			shift = true;
		}
		if (shift && i != m_size-1) {
			m_map[i] = m_map[i + 1];
		}
	}
	if (shift) m_size--;
	return shift;
}
// If key is equal to a key currently in the map, remove the key/value
// Item with that key from the map and return true.  Otherwise, make
// no change to the map and return false.

bool Map::contains(const KeyType& key) const {
	for (int i = 0; i < m_size; i++) {
		if (m_map[i].key == key) {
			return true;
		}
	}
	return false;
}
// Return true if key is equal to a key currently in the map, otherwise
// false.

bool Map::get(const KeyType& key, ValueType& value) const {
	if (!contains(key)) return false;
	for (int i = 0; i < m_size; i++) {
		if (m_map[i].key == key) {
			value = m_map[i].value;
		}
	}
	return true;
}
// If key is equal to a key currently in the map, set value to the
// value in the map that the key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.

bool Map::get(int i, KeyType& key, ValueType& value) const {
	int num = i;
	for (int i = 0; i < m_size; i++) {
		int cnt = 0;
		for (int j = 0; j < m_size; j++) {
			if (m_map[j].key < m_map[i].key) cnt++;
		}
		if (cnt == num) {
			key = m_map[i].key;
			value = m_map[i].value;
			return true;
		}
	}
	return false;
}
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of the key/value Item in the map whose key is strictly
// greater than exactly i keys in the map and return true.  Otherwise,
// leave the key and value parameters unchanged and return false.

void Map::swap(Map& other) {
	int min_size = m_size < other.m_size ? m_size : other.m_size;
	for (int i = 0; i < min_size; i++) {
		Item temp = m_map[i];
		m_map[i] = other.m_map[i];
		other.m_map[i] = temp;
	}
	if (m_size < other.m_size) {
		for (int i = min_size; i < other.m_size; i++) {
			m_map[i] = other.m_map[i];
		}
	}
	else if (m_size > other.m_size) {
		for (int i = 0; i < m_size; i++) {
			other.m_map[i] = m_map[i];
		}
	}
	int other_size = other.m_size;
	other.m_size = m_size;
	m_size = other_size;
}
// Exchange the contents of this map with the other one.

