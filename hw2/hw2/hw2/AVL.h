#ifndef _AVL_h
#define _AVL_h

#include <string>
#include <iostream>
#include <vector>
using namespace std;

template <class Key, class Value>
class AvlTree;

//AvlNode with Key and Value templates
template <class Key, class Value>
class AvlNode {
public:
    Key key; //key that will used to navigate true tree
    Value value; //store informations about key
    AvlNode<Key, Value>* left; 
    AvlNode<Key, Value>* right;
    int height;

    AvlNode(const Key &theKey, const Value &theValue, AvlNode<Key, Value>* lt, AvlNode<Key, Value>* rt, int h = 0) //parameterized constructer
        : key(theKey), value(theValue), left(lt), right(rt), height(h) {}

	AvlNode(const Key &theKey, const Value &theValue){//default constructer
        key = theKey;
		value = theValue;
		left = NULL;
		right = NULL;
		height = NULL;
	}


    friend class AvlTree<Key, Value>;//making the node friend with avltree
};

//AvlTree with Key and Value templates
template <class Key, class Value>
class AvlTree {
private:
    AvlNode<Key, Value>* root; //root of the tree
    const Value ITEM_NOT_FOUND; //value that will returned when item beign searched is not found

    const Value &elementAt(AvlNode<Key, Value>* t) const; //returns value of the given node
    void insert(const Key &x, const Value &val, AvlNode<Key, Value>* &t) const; //inserts new element
    void remove(const Key &x, AvlNode<Key, Value>* &t) const; //removes an element
    AvlNode<Key, Value>* findMin(AvlNode<Key, Value>* t) const; //finds minimum 
    AvlNode<Key, Value>* findMax(AvlNode<Key, Value>* t) const; //finds maximum
    AvlNode<Key, Value>* find(const Key &x, AvlNode<Key, Value>* t) const; //finds node of the given key
    void makeEmpty(AvlNode<Key, Value>* &t) const; //clearing the tree
    AvlNode<Key, Value>* clone(AvlNode<Key, Value>* t) const;  //makes clone node
    int height(AvlNode<Key, Value>* t) const; //gives height
    int max(int lhs, int rhs) const; //gives maximum
    void rotateWithLeftChild(AvlNode<Key, Value>* &k2) const; 
    void rotateWithRightChild(AvlNode<Key, Value>* &k1) const;
    void doubleWithLeftChild(AvlNode<Key, Value>* &k3) const;
    void doubleWithRightChild(AvlNode<Key, Value>* &k1) const;
	
public:
    explicit AvlTree(const Value &notFound); //first constructer which determines notfound
    AvlTree(const AvlTree &rhs); //constructer
    ~AvlTree(); //destructer

    const Value &findMin() const; //public function to reach private findmin
    const Value &findMax() const; //public function to reach private findmax
    const Value &find(const Key &x) const; ///public function to reach private find
    bool isEmpty() const; //checking if the tree is empty
   
    void makeEmpty(); ////public function to reach private makeempty
    void insert(const Key &x, const Value &val); ////public function to reach private insert
    void remove(const Key &x); //public function to reach private remove

    const AvlTree &operator=(const AvlTree &rhs); //an operator to make two tree equal
	
	
};


#endif