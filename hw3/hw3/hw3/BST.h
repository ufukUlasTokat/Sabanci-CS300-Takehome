#ifndef _BST_h
#define _BST_h

#include <string>
#include <iostream>
#include <vector>
using namespace std;

template <class Key, class Value>
class BSTree;

//BSTNode with Key and Value templates
template <class Key, class Value>
class BSTNode {
public:
    Key key; //key that will used to navigate true tree
    Value value; //store informations about key
    BSTNode<Key, Value>* left; 
    BSTNode<Key, Value>* right;
    int height;

    BSTNode(const Key &theKey, const Value &theValue, BSTNode<Key, Value>* lt, BSTNode<Key, Value>* rt, int h = 0) //parameterized constructer
        : key(theKey), value(theValue), left(lt), right(rt), height(h) {}

	BSTNode(const Key &theKey, const Value &theValue){//default constructer
        key = theKey;
		value = theValue;
		left = NULL;
		right = NULL;
		height = NULL;
	}


    friend class BSTree<Key, Value>;//making the node friend with BStree
};

//BSTree with Key and Value templates
template <class Key, class Value>
class BSTree {
private:
    BSTNode<Key, Value>* root; //root of the tree
    const Value ITEM_NOT_FOUND; //value that will returned when item beign searched is not found

    const Value &elementAt(BSTNode<Key, Value>* t) const; //returns value of the given node
    void insert(const Key &x, const Value &val, BSTNode<Key, Value>* &t) const; //inserts new element
    void remove(const Key &x, BSTNode<Key, Value>* &t) const; //removes an element
    BSTNode<Key, Value>* findMin(BSTNode<Key, Value>* t) const; //finds minimum 
    BSTNode<Key, Value>* findMax(BSTNode<Key, Value>* t) const; //finds maximum
    BSTNode<Key, Value>* find(const Key &x, BSTNode<Key, Value>* t) const; //finds node of the given key
    void makeEmpty(BSTNode<Key, Value>* &t) const; //clearing the tree
    BSTNode<Key, Value>* clone(BSTNode<Key, Value>* t) const;  //makes clone node
   

	
public:
    explicit BSTree(const Value &notFound); //first constructer which determines notfound
    BSTree(const BSTree &rhs); //constructer
    ~BSTree(); //destructer

    const Value &findMin() const; //public function to reach private findmin
    const Value &findMax() const; //public function to reach private findmax
    const Value &find(const Key &x) const; ///public function to reach private find
    bool isEmpty() const; //checking if the tree is empty
   
    void makeEmpty(); ////public function to reach private makeempty
    void insert(const Key &x, const Value &val); ////public function to reach private insert
    void remove(const Key &x); //public function to reach private remove

    const BSTree &operator=(const BSTree &rhs); //an operator to make two tree equal
	
	
};


#endif