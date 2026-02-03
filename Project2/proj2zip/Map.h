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
    void dump() const;

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