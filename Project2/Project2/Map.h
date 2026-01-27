#ifndef MAP_H
#define MAP_H

#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
public:
    Map();         // Create an empty map (i.e., one whose size() is 0).

    Map(const Map& other);

    Map& operator=(const Map& other);

    ~Map();

    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    /*
    bool empty() const;  // Return true if the map is empty, otherwise false.

    int size() const;    // Return the number of key/value Items in the map.

    bool insert(const KeyType& key, const ValueType& value);
    // If key is not equal to any key currently in the map and if the
    // key/value Item can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map, or the map has a fixed
    // capacity and is full).

    bool update(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value that it currently maps to, but instead map to
    // the value of the second parameter; in this case, return true.
    // Otherwise, make no change to the map and return false.

    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value that it currently maps to, but instead map to
    // the value of the second parameter; in this case, return true.
    // If key is not equal to any key that is currently in the map, and if the
    // key/value Item can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that the key is not already in the map and the map has a fixed
    // capacity and is full).

    bool erase(const KeyType& key);
    // If key is equal to a key currently in the map, remove the key/value
    // Item with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.

    bool contains(const KeyType& key) const;
    // Return true if key is equal to a key currently in the map, otherwise
    // false.

    bool get(const KeyType& key, ValueType& value) const;
    // If key is equal to a key currently in the map, set value to the
    // value in the map that the key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.

    bool get(int i, KeyType& key, ValueType& value) const;
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of the key/value Item in the map whose key is strictly
    // greater than exactly i keys in the map and return true.  Otherwise,
    // leave the key and value parameters unchanged and return false.

    void swap(Map& other);
    // Exchange the contents of this map with the other one.
    */

private:
    struct Node
    {
        KeyType key;
        ValueType val;
        Node* prev;
        Node* next;
    };
    
    Node* findNode(const KeyType& key) const;
    void add_front(const KeyType& key, const ValueType& value);
    void add_rear(const KeyType& key, const ValueType& value);
    void add_alpha(const KeyType& key, const ValueType& value);
    void remove(Node* p);
    void clear();


    Node* dummy_;
    int m_size;
    //int m_capacity;
};

bool merge(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);


#endif