#include "multimap.h"
#include <string>
#include <queue>

// TODO: implement at least one constructor
//Multimap::Iterator::Iterator( /* your constructor arguments */ )
//{
//    // TODO: implement
//}


Multimap::Iterator::Iterator() : m_values(nullptr), m_pos(0) {}

Multimap::Iterator::~Iterator() {}

Multimap::Iterator::Iterator(const std::vector<std::string>* values) : m_values(values), m_pos(0) {}

bool Multimap::Iterator::next(std::string& value) {
	if (m_values == nullptr || m_pos >= m_values->size()) {
		return false;
	}
	value = (*m_values)[m_pos];
	m_pos++;

	return true;
}

Multimap::Multimap() : head(nullptr), m_size(0) {}

Multimap::~Multimap()
{
	//delete whole tree
	std::queue<Node*> q;
	q.push(head);
	Node* temp = nullptr;
	while (!q.empty()) {
		temp = q.front();
		q.pop();
		if (temp != nullptr) {
			q.push(temp->left);
			q.push(temp->right);
		}
	}
	delete temp;
	head = nullptr;
}

void Multimap::put(const std::string& key, const std::string& value)
{
	Node* curNode = head;
	Node* parentNode = nullptr;

	if (curNode == nullptr) {
		head = new Node(key, { value });
		m_size++;
		return;
	}

	while (curNode != nullptr) {
		parentNode = curNode;
		if (key < curNode->key) {
			curNode = curNode->left;
		}
		else if (key > curNode->key) {
			curNode = curNode->right;
		}
		else if (key == curNode->key) {
			for (std::string s : curNode->val) {
				if (s == value) return;
			}
			curNode->val.push_back(value);
			m_size++;
			return;
		}
	}

	if (key < parentNode->key) {
		parentNode->left = new Node(key, { value });
	}
	else if (key > parentNode->key) {
		parentNode->right = new Node(key, { value });
	}
	m_size++;
}

MultimapBase::IteratorBase* Multimap::get(const std::string& key) const
{
	Node* curNode = head;
	while (curNode != nullptr) {
		if (key < curNode->key) {
			curNode = curNode->left;
		}
		else if (key > curNode->key) {
			curNode = curNode->right;
		}
		else if (key == curNode->key) {
			return new Iterator(&(curNode->val));
		}
	}
	return new Iterator();
}

bool Multimap::empty() const
{
	return head == nullptr;
}

int Multimap::size() const
{
	return m_size;
}
