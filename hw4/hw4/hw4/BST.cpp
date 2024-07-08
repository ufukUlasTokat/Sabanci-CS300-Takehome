#include "BST.h"
#include <iostream>
#include <string>

using namespace std;

template <class Key, class Value>
BSTree<Key, Value>::BSTree(const Value &notFound) : ITEM_NOT_FOUND(notFound), root(NULL) {}

/**
* Internal method to get element field in node t.
* Return the element field or ITEM_NOT_FOUND if t is NULL.
*/
template <class Key, class Value>
const Value &BSTree<Key, Value>::elementAt(BSTNode<Key, Value>* t) const {
    return t == NULL ? ITEM_NOT_FOUND : t->value;
}

/**
* Find item x in the tree.
* Return the matching item or ITEM_NOT_FOUND if not found.
*/
template <class Key, class Value>
const Value &BSTree<Key, Value>::find(const Key &x) const {
    return elementAt(find(x, root));
}

/**
* Internal method to find an item in a subtree.
* x is item to search for.
* t is the node that roots the tree.
* Return node containing the matched item.
*/
template <class Key, class Value>
BSTNode<Key, Value>* BSTree<Key, Value>::find(const Key &x, BSTNode<Key, Value>* t) const {
    if (t == NULL)
        return NULL;
    else if (x < t->key)
        return find(x, t->left);
    else if (t->key < x)
        return find(x, t->right);
    else
        return t;
}

/**
* Find the smallest item in the tree.
* Return smallest item or ITEM_NOT_FOUND if empty.
*/
template <class Key, class Value>
const Value &BSTree<Key, Value>::findMin() const {
    return elementAt(findMin(root));
}

/**
* Internal method to find the smallest item in a subtree t.
* Return node containing the smallest item.
*/
template <class Key, class Value>
BSTNode<Key, Value>* BSTree<Key, Value>::findMin(BSTNode<Key, Value>* t) const {
    if (t == NULL)
        return NULL;
    if (t->left == NULL)
        return t;
    return findMin(t->left);
}

/**
* Find the smallest item in the tree.
* Return smallest item or ITEM_NOT_FOUND if empty.
*/
template <class Key, class Value>
BSTNode<Key, Value>* BSTree<Key, Value>::findMax(BSTNode<Key, Value>* t) const {
    if (t != NULL)
        while (t->right != NULL)
            t = t->right;
    return t;
}

/**
* Internal method to find the largest item in a subtree t.
* Return node containing the largest item.
*/
template <class Key, class Value>
const Value &BSTree<Key, Value>::findMax() const {
    return elementAt(findMax(root));
}

/**
* Insert x into the tree; duplicates are ignored.
*/
template <class Key, class Value>
void BSTree<Key, Value>::insert(const Key &x, const Value &val) {
    insert(x, val, root);
}


/**
* Remove x from the tree. Nothing is done if x is not found.
*/
template <class Key, class Value>
void BSTree<Key, Value>::remove(const Key &x) {
    remove(x, root);
}

/**
* Internal method to remove from a subtree.
* x is the item to remove.
* t is the node that roots the tree.
* Set the new root.
*/
template <class Key, class Value>
void BSTree<Key, Value>::remove(const Key &x, BSTNode<Key, Value>* &t) const {
    if (t == NULL) {
        return;   // Item not found; do nothing
    }
    if (x < t->key) {
        remove(x, t->left);
    }
    else if (t->key < x) {
        remove(x, t->right);
    }
    else if (t->left != NULL && t->right != NULL) { // Two children
		BSTNode<Key, Value> *newNode = findMin(t->right);
        t->value = newNode->value;
		t->key = newNode->key;
        remove(t->key, t->right);
    }
    else { // One or no children
        BSTNode<Key, Value>* oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
    }
}

/**
* Make the tree logically empty.
*/
template <class Key, class Value>
void BSTree<Key, Value>::makeEmpty() {
    makeEmpty(root);
}

/**
* Internal method to make subtree empty.
*/
template <class Key, class Value>
void BSTree<Key, Value>::makeEmpty(BSTNode<Key, Value>* &t) const {
    if (t != NULL) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}


/**
* Destructor for the tree.
*/
template <class Key, class Value>
BSTree<Key, Value>::~BSTree() {
    makeEmpty();
}


/**
* Internal method to insert into a subtree.
* x and y are items to insert.
* t is the node that roots the tree.
* Set the new root.
*/
template <class Key, class Value>
void BSTree<Key, Value>::insert(const Key & x, const Value & val, BSTNode<Key, Value>* &t) const {
    if( t == NULL ) //  create a new node at the right place
                t = new BSTNode<Key, Value>(x, val);
            else if(x < t->key)
                insert(x, val, t->left);  // insert at the left or 
            else if(t->key < x)
                insert(x, val, t->right);  // right subtree
            else
                ;  // Duplicate; do nothing

}


/**
* Internal method to clone subtree.
*/
template <class Key, class Value>
BSTNode<Key, Value>* BSTree<Key, Value>::clone(BSTNode<Key, Value>* t) const {
    if (t == NULL)
        return NULL;
    else
        return new BSTNode<Key, Value>(t->key, t->value, clone(t->left), clone(t->right));
}
