#include "Map.h"

Map::Map() {
	dummy_ = new Node();
	dummy_->prev = dummy_;
	dummy_->next = dummy_;
	m_size = 0;
}

Map::Map(const Map& other) {
	dummy_ = new Node();
	dummy_->prev = dummy_;
	dummy_->next = dummy_;
	m_size = 0;
	for (Node* p = other.dummy_->next; p != other.dummy_; p = p->next)
	{
		add_rear(p->key, p->val); // adding nodes to back of this LL
	}
	m_size = other.m_size; //not neccesary
}

Map& Map::operator=(const Map& other) {
	if (this != &other) {
		clear();
		delete dummy_;
		dummy_ = new Node();
		dummy_->prev = dummy_;
		dummy_->next = dummy_;
		m_size = 0;
		for (Node* p = other.dummy_->next; p != other.dummy_; p = p->next)
		{
			add_rear(p->key, p->val); // adding nodes to back of this LL
		}
		m_size = other.m_size; //also not neccesary
	}
	return *this;
}

Map::~Map() {
	clear();
	delete dummy_;
}

bool Map::empty() const
{
	return m_size == 0;
}
int Map::size() const
{
	return m_size;
}
bool Map::insert(const KeyType& key, const ValueType& value) {
	if (contains(key)) return false;
	add_alpha(key, value);

	return true;
}

// If key is not equal to any key currently in the map and if the
// key/value Item can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).

bool Map::update(const KeyType& key, const ValueType& value) {
	Node* p = findNode(key);
	if (p == nullptr)
		return false;
	p->val = value;
	return true;
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
	Node* p = findNode(key);
	if (p == nullptr)
		return false;

	remove(p);
	return true;
}
// If key is equal to a key currently in the map, remove the key/value
// Item with that key from the map and return true.  Otherwise, make
// no change to the map and return false.

bool Map::contains(const KeyType& key) const {
	return findNode(key) != nullptr;
}
// Return true if key is equal to a key currently in the map, otherwise
// false.

bool Map::get(const KeyType& key, ValueType& value) const {
	Node* p = findNode(key);
	if (p == nullptr)
		return false;
	value = p->val;
	return true;
}
// If key is equal to a key currently in the map, set value to the
// value in the map that the key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.

bool Map::get(int i, KeyType& key, ValueType& value) const {
	int num = i;
	Node* p = dummy_->next;
	if (num < 0 || num >= m_size) return false;
	for (int i = 0; i < num; i++) {
		p = p->next;
	}
	key = p->key;
	value = p->val;
	return true;
}
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of the key/value Item in the map whose key is strictly
// greater than exactly i keys in the map and return true.  Otherwise,
// leave the key and value parameters unchanged and return false.

void Map::swap(Map& other) {
	if (this == &other) return;

	Node* temp = dummy_;
	dummy_ = other.dummy_;
	other.dummy_ = temp;

	int other_size = other.m_size;
	other.m_size = m_size;
	m_size = other_size;
}
// Exchange the contents of this map with the other one.


Map::Node* Map::findNode(const KeyType& key) const {
	Node* p = dummy_->next;
	while (p != dummy_) {
		if (p->key == key)
			return p;
		p = p->next;
	}
	return nullptr;

}
//returns pointer to node with a specific key, else returns nullptr;


//private helper functions
void Map::add_front(const KeyType& key, const ValueType& value) {
	Node* nn = new Node;
	nn->key = key;
	nn->val = value;

	nn->prev = dummy_;
	nn->next = dummy_->next;
	dummy_->next->prev = nn;
	dummy_->next = nn;
	m_size++;
}
//adds new node to front of map

void Map::add_rear(const KeyType& key, const ValueType& value) {
	Node* nn = new Node();
	nn->key = key;
	nn->val = value;

	nn->prev = dummy_->prev;
	nn->next = dummy_;
	dummy_->prev->next = nn;
	dummy_->prev = nn;
	m_size++;
}
//adds new node to rear of map

void Map::add_alpha(const KeyType& key, const ValueType& value) {
	Node* p = dummy_->next;

	while (p != dummy_) {
		if (p->key > key) {
			Node* nn = new Node();
			nn->key = key;
			nn->val = value;

			nn->prev = p->prev;
			nn->next = p;
			p->prev->next = nn;
			p->prev = nn;
			m_size++;
			return;
		}
		p = p->next;
	}
	add_rear(key, value);
	return;
}
//add new node in alphanumberic order in LL

void Map::remove(Node* p) {
	if (p == dummy_ || p == nullptr) return;
	p->prev->next = p->next;
	p->next->prev = p->prev;
	delete p;
	m_size--;
}
//remove LL node pointed to at p.

void Map::clear() {
	Node* p = dummy_->next;

	while (p != dummy_) {
		Node* temp = p;
		p = p->next;
		delete temp;
	}
}
//deallocate memory of entire LL


//non-class functions

bool merge(const Map& m1, const Map& m2, Map& result) {
	return false;
}
void reassign(const Map& m, Map& result) {
	return;
}