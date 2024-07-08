#ifndef _HASH_h
#define _HASH_h

#include <string>
#include <iostream>
#include <vector>
using namespace std;

// Enum to define different states of an entry in the hash table
enum EntryType {ACTIVE, EMPTY, DELETED};

// Forward declarations for classes
template <class Key, class Value>
class HashTable;

template <class Key, class Value>
class HashEntry {
public:
    Key element; // Key that will be used to navigate through the hash table
    Value value; // Stores information associated with the key
    EntryType info; // Indicates if the entry is active, empty, or deleted

    // Parameterized constructor
    HashEntry(const Key &theKey, const Value &theValue, EntryType i = EMPTY)
        : element(theKey), value(theValue), info(i) {}

    // Default constructor
    HashEntry() : element(), value(), info(EMPTY) {}

    friend class HashTable<Key, Value>; // Making the node friend with HashTable
};

template <class Key, class Value>
class HashTable {
private:
    vector<HashEntry<Key,Value>> array; // Array to hold the hash table
    int currentSize; // Current number of elements in the hash table
    const Value ITEM_NOT_FOUND; // Value to indicate item not found during searches

    // Private member functions
    bool isActive(int currentPos) const; // Check if the current position in the table is active
    int findPos(const Key & x) const; // Find the position of an element in the table
    void rehash(); // Rehashes the table when load factor is too high
    void makeEmpty(const vector<HashEntry<Key,Value>> arr); // Clears the table
    bool isPrime(int n) const; // Checks if a number is prime
    int nextPrime(int n) const; // Finds the next prime number after n

public:
    // Constructor
    explicit HashTable(const Value & notFound, int size = 50);

    // Copy constructor
    HashTable(const HashTable & rhs) : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), array(rhs.array), currentSize(rhs.currentSize) {}

    void makeEmpty(); // Clears the hash table
    const Value & find(const Key & x) const; // Finds a value associated with a key
    void insert(const Key & x, const Value & y); // Inserts a key-value pair into the hash table
    void remove(const Key & x); // Removes a key-value pair from the hash table
    const HashTable & operator=(const HashTable & rhs); // Assignment operator
    long int hash(const string & x, int size) const; // Hash function to determine the index in the table for a given key
    int sizeofTable(); // Returns the size of the table
    double loadofTable(); // Calculates the load factor of the table
};

#endif
