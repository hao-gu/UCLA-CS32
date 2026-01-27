//DELETE BEFORE TURNING IN

#include "Map.h"
#include <type_traits>

#define CHECKTYPE(c, f, r, a)  \
        {  \
         static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
             "You did not declare " #c "::" #f " as the Project 2 spec does");  \
         auto p = static_cast<r (c::*)a>(&c::f);  \
         (void) p;  \
        }
#define CHECKTYPENONMEMBER(f, t)  \
        {  \
         static_assert(std::is_same<decltype(f), t>::value, \
             "You did not declare " #f " as the Project 2 spec does");  \
         auto p = static_cast<std::add_pointer<t>::type>(f);  \
         (void) p;  \
        }

static_assert(std::is_default_constructible<Map>::value,
    "Map must be default-constructible.");
static_assert(std::is_copy_constructible<Map>::value,
    "Map must be copy-constructible.");
static_assert(std::is_copy_assignable<Map>::value,
    "Map must be assignable.");
static_assert(std::is_same<decltype(std::declval<Map>() = Map()), Map&>::value,
    "Map::operator= return type should be Map&");

void thisFunctionWillNeverBeCalled()
{
    CHECKTYPE(Map, empty, bool, () const);
    CHECKTYPE(Map, size, int, () const);
    CHECKTYPE(Map, insert, bool, (const KeyType&, const ValueType&));
    CHECKTYPE(Map, update, bool, (const KeyType&, const ValueType&));
    CHECKTYPE(Map, insertOrUpdate, bool, (const KeyType&, const ValueType&));
    CHECKTYPE(Map, erase, bool, (const KeyType&));
    CHECKTYPE(Map, contains, bool, (const KeyType&) const);
    CHECKTYPE(Map, swap, void, (Map&));

    CHECKTYPENONMEMBER(::merge, bool(const Map&, const Map&, Map&));
    CHECKTYPENONMEMBER(::reassign, void(const Map&, Map&));
    { auto p = static_cast<bool (Map::*)(const KeyType&, ValueType&) const>(&Map::get); (void)p; };
    { auto p = static_cast<bool (Map::*)(int, KeyType&, ValueType&) const>(&Map::get); (void)p; };
}

int main()
{
}