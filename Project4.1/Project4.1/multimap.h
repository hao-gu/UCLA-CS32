#ifndef MULTIMAP_H
#define MULTIMAP_H

#include "provided.h"
#include <string>

class Multimap : public MultimapBase
{
public:
    // Nested Iterator class
    class Iterator : public MultimapBase::IteratorBase
    {
    public:
        Iterator();
        Iterator(const std::vector<std::string>* values);

        virtual bool next(std::string& value);

    private:
        // TODO: add private members
        const std::vector<std::string>* m_values;
        int m_pos;
    };

    Multimap();
    virtual ~Multimap();
    virtual void put(const std::string& key, const std::string& value);
    virtual MultimapBase::IteratorBase* get(const std::string& key) const;
    virtual bool empty() const;
    virtual int size() const;

private:
    struct Node {
        std::string key;
        std::vector<std::string> val;
        Node* left;
        Node* right;
        Node(const std::string& myKey, const std::vector<std::string>& myVal)
        {
            key = myKey;
            val = myVal;
            left = right = nullptr;
        }
    };

    Node* head;
    int m_size;
};

#endif // MULTIMAP_H