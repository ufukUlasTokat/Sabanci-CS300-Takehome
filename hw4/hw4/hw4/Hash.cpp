#include "Hash.h"
#include <string>



//a function to chech if the number is prime
template <class Key, class Value>
bool HashTable<Key, Value>::isPrime(int n)const //checks if n is a prime number
{
    if (n == 2 || n == 3)
        return true;
    
    if (n == 1 || n % 2 == 0)
        return false;
    
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

//a function to find next prime
template <class Key, class Value>
int HashTable<Key, Value>::nextPrime(int n)const //finds the next prime after n if n isnt a prime
{
    if (n % 2 == 0)
        n++;
    for (; ! isPrime(n); n += 2)
        ;
    
    return n;
}



/**
* Construct the hash table.
*/
template <class Key, class Value>
HashTable<Key, Value>::HashTable(const Value & notFound, int size): ITEM_NOT_FOUND(notFound), array(nextPrime(size))
{
    makeEmpty();
}


//hashing function
template <class Key, class Value>
long int HashTable<Key, Value>::hash(const string & x, int size) const
{
	long int hashed = 0;
	
	for(unsigned int i=0;i<x.length();i++){
		hashed = 12*hashed + x[i];
	}
	hashed = hashed % size;
	if(hashed < 0){
		hashed = hashed + size;
	}
	return hashed;
}

/**
* Method that performs quadratic probing resolution.
* Return the position where the search for x terminates.
*/
template <class Key, class Value>
int HashTable<Key, Value>::findPos(const Key & x) const
{
    int collisionNum = 0;
	int size = array.size();
    unsigned long int currentPos = hash(x,size);
	HashEntry<Key,Value> tmp;
	tmp = array[currentPos];

	while (tmp.info != EMPTY && tmp.element != x)
    {
        currentPos += pow(++collisionNum, 2) ;  //add the difference
        if (currentPos >= array.size())              // perform the mod
                currentPos = currentPos -  array.size();  // if necessary
		tmp = array[currentPos];
    }
    return currentPos;
}

/**
* Find item x in the hash table.
* Return the matching item, or ITEM_NOT_FOUND, if not found.
*/
template <class Key, class Value>
const Value & HashTable<Key, Value>::find(const Key & x) const
{
    int currentPos = findPos(x);
    if (isActive(currentPos)) {
		return array[currentPos].value; 
	}
    return ITEM_NOT_FOUND;
}





/**
* Return true if currentPos exists and is active.
*/
template <class Key, class Value>
bool HashTable<Key, Value>::isActive(int currentPos) const
{
	HashEntry<Key,Value> tmp;
	tmp = array[currentPos];

	return tmp.info == ACTIVE;
}

/**
* Remove item x from the hash table.
*  x has to be in the table
*/
template <class Key, class Value>
void HashTable<Key, Value>::remove(const Key & x)
{
    int currentPos = findPos(x);
    if (isActive(currentPos))
	{
		HashEntry<Key,Value> tmp;
		tmp = array[currentPos];
        tmp.info = DELETED;
		array[currentPos] = tmp;
	}
	currentSize = currentSize - 1;
}


/**
* Insert item x into the hash table. If the item is
* already present, then do nothing.
*/
template <class Key, class Value>
void HashTable<Key, Value>::insert(const Key & x, const Value & y)
{
    // Insert x as active
    int currentPos = findPos(x);
    
	if(isActive(currentPos)){
		return;
	}
	// enlarge the hash table if necessary 
	
    array[currentPos] = HashEntry<Key,Value>(x,y, ACTIVE);
	
	if (++currentSize >= array.size() / 1.8)
		rehash();

}


/**
* Expand the hash table.
*/
template <class Key, class Value>
void HashTable<Key,Value>::rehash()
{
    vector<HashEntry<Key,Value>> oldArray = array;

    // Create new double-sized, empty table
	int newsize = nextPrime(2*oldArray.size());
    array.resize(newsize);
	double load = static_cast<double>(currentSize)/array.size();
    for (unsigned int j = 0; j < array.size(); j++){
		HashEntry<Key,Value> tmp;
		tmp = array[j];
        tmp.info = EMPTY;
		array[j] = tmp;
	}
    // Copy table over
    currentSize = 0;
    for (unsigned int i = 0; i < oldArray.size(); i++){
		HashEntry<Key,Value> tmp2;
		tmp2 = oldArray[i];
        if (tmp2.info == ACTIVE)
                insert(tmp2.element, tmp2.value);
	}
}

//(private) makes the able empty
template <class Key, class Value>
void HashTable<Key, Value>::makeEmpty(const vector<HashEntry<Key,Value>> arr) {
    for (unsigned int i = 0; i < array.size(); ++i) {
		HashEntry<Key,Value> tmp;
		tmp = array[i];
        tmp.info = EMPTY;
		array[i] = tmp;
    }
    currentSize = 0;
}

//(public) makes the table empty
template <class Key, class Value>
void HashTable<Key, Value>::makeEmpty() {
	makeEmpty(array);
}

//returns distinct entry count of table
template <class Key, class Value>
int HashTable<Key, Value>::sizeofTable(){
	return currentSize;
}
//returns load ratio of table
template <class Key, class Value>
double HashTable<Key, Value>::loadofTable(){
	return static_cast<double>(currentSize)/array.size();
}