#include <iostream>
#include <cassert>
using namespace std;

bool somePredicate(double x)
{
    return x > 0;
}
// Return true if the somePredicate function returns false for at
   // least one of the array elements; return false otherwise.
bool anyFalse(const double a[], int n)
{
    if (n < 1) return false;
    return !somePredicate(a[0]) || anyFalse(a + 1, n - 1); 
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
    if (n < 1) return 0;
    if (!somePredicate(a[0])) return 1 + countFalse(a + 1, n - 1);
    return countFalse(a + 1, n - 1);  // This is incorrect.
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{   
    if (n < 1) return -1;
    if (!somePredicate(a[0])) return 0;

    int temp = firstFalse(a + 1, n - 1);
    if (temp == -1) return -1;
    else return 1 + temp;
}

// Return the subscript of the first double in the array that is <=
// all doubles in the array (i.e., return the smallest subscript m such
// that a[m] <= a[k] for all k from 0 to n-1).  If the function is
// told that no doubles are to be considered to be in the array,
// return -1.
int indexOfMinimum(const double a[], int n)
{
    if (n < 1) return -1;
    if (n == 1) return 0;
    int min_index = indexOfMinimum(a + 1, n - 1);
    if (min_index == -1) return 0;
    if (a[0] <= a[min_index + 1]) return 0;
    return min_index + 1;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0) return true;
    if (n1 <= 0) return false;

    if (a1[0] == a2[0]) return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    return includes(a1 + 1, n1 - 1, a2, n2);
}

/*int main() {
    double arr1[7] = { 10, 50, 40, 20, 50, 40, 30 };
    double arr2[3] = { 50, 40, -40 };
    double arr3[3] = { 50, 40, 40 };
    double arr4[3] = { 50, 40, 20 };
    assert(anyFalse(arr1, 7) == false);
    assert(anyFalse(arr2, 3) == true);
    assert(countFalse(arr1, 7) == 0);
    assert(countFalse(arr2, 3) == 1);
    assert(firstFalse(arr1, 7) == -1);
    assert(firstFalse(arr2, 3) == 2);
    cout << indexOfMinimum(arr1, 7);
    assert(indexOfMinimum(arr1, 7) == 0);
    assert(indexOfMinimum(arr2, 3) == 2);
    assert(includes(arr1, 7, arr2, 3) == false);
    assert(includes(arr1, 7, arr3, 3) == true);
    assert(includes(arr1, 7, arr4, 3) == true);
    cout << "Passed all test cases";
}*/