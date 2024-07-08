#include "AVL.h"
#include <iostream>
#include <string>

using namespace std;

template <class Key, class Value>
AvlTree<Key, Value>::AvlTree(const Value &notFound) : ITEM_NOT_FOUND(notFound), root(NULL) {}

/**
* Internal method to get element field in node t.
* Return the element field or ITEM_NOT_FOUND if t is NULL.
*/
template <class Key, class Value>
const Value &AvlTree<Key, Value>::elementAt(AvlNode<Key, Value>* t) const {
    return t == NULL ? ITEM_NOT_FOUND : t->value;
}

/**
* Find item x in the tree.
* Return the matching item or ITEM_NOT_FOUND if not found.
*/
template <class Key, class Value>
const Value &AvlTree<Key, Value>::find(const Key &x) const {
    return elementAt(find(x, root));
}

/**
* Internal method to find an item in a subtree.
* x is item to search for.
* t is the node that roots the tree.
* Return node containing the matched item.
*/
template <class Key, class Value>
AvlNode<Key, Value>* AvlTree<Key, Value>::find(const Key &x, AvlNode<Key, Value>* t) const {
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
const Value &AvlTree<Key, Value>::findMin() const {
    return elementAt(findMin(root));
}

/**
* Internal method to find the smallest item in a subtree t.
* Return node containing the smallest item.
*/
template <class Key, class Value>
AvlNode<Key, Value>* AvlTree<Key, Value>::findMin(AvlNode<Key, Value>* t) const {
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
AvlNode<Key, Value>* AvlTree<Key, Value>::findMax(AvlNode<Key, Value>* t) const {
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
const Value &AvlTree<Key, Value>::findMax() const {
    return elementAt(findMax(root));
}

/**
* Insert x into the tree; duplicates are ignored.
*/
template <class Key, class Value>
void AvlTree<Key, Value>::insert(const Key &x, const Value &val) {
    insert(x, val, root);
}

/**
* Remove x from the tree. Nothing is done if x is not found.
*/
template <class Key, class Value>
void AvlTree<Key, Value>::remove(const Key &x) {
    remove(x, root);
}

/**
* Internal method to remove from a subtree.
* x is the item to remove.
* t is the node that roots the tree.
* Set the new root.
*/
template <class Key, class Value>
void AvlTree<Key, Value>::remove(const Key &x, AvlNode<Key, Value>* &t) const {
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
		AvlNode<Key, Value> *newNode = findMin(t->right);
        t->value = newNode->value;
		t->key = newNode->key;
        remove(t->key, t->right);
    }
    else { // One or no children
        AvlNode<Key, Value>* oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
    }
}

/**
* Make the tree logically empty.
*/
template <class Key, class Value>
void AvlTree<Key, Value>::makeEmpty() {
    makeEmpty(root);
}

/**
* Internal method to make subtree empty.
*/
template <class Key, class Value>
void AvlTree<Key, Value>::makeEmpty(AvlNode<Key, Value>* &t) const {
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
AvlTree<Key, Value>::~AvlTree() {
    makeEmpty();
}


/**
* Return the height of node t, or -1, if NULL.
*/
template <class Key, class Value>
int AvlTree<Key, Value>::height(AvlNode<Key, Value>* t) const {
    if (t == NULL)
        return -1;
    return t->height;
}

/**
* Return maximum of lhs and rhs.
*/
template <class Key, class Value>
int AvlTree<Key, Value>::max(int lhs, int rhs) const {
    if (lhs > rhs)
        return lhs;
    return rhs;
}

/**
* Rotate binary tree node with left child.
* For AVL trees, this is a single rotation for case 1.
* Update heights, then set new root.
*/
template <class Key, class Value>
void AvlTree<Key, Value>::rotateWithLeftChild(AvlNode<Key, Value>* &k2) const {
    AvlNode<Key, Value>* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

/**
* Rotate binary tree node with right child.
* For AVL trees, this is a single rotation for case 4.
* Update heights, then set new root.
*/
template <class Key, class Value>
void AvlTree<Key, Value>::rotateWithRightChild(AvlNode<Key, Value>* &k1) const {
    AvlNode<Key, Value>* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2;
}

/**
* Double rotate binary tree node: first left child.
* with its right child; then node k3 with new left child.
* For AVL trees, this is a double rotation for case 2.
* Update heights, then set new root.
*/
template <class Key, class Value>
void AvlTree<Key, Value>::doubleWithLeftChild(AvlNode<Key, Value>* &k3) const {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

/**
* Double rotate binary tree node: first right child.
* with its left child; then node k1 with new right child.
* For AVL trees, this is a double rotation for case 3.
* Update heights, then set new root.
*/
template <class Key, class Value>
void AvlTree<Key, Value>::doubleWithRightChild(AvlNode<Key, Value>* &k1) const {
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}


/**
* Internal method to insert into a subtree.
* x and y are items to insert.
* t is the node that roots the tree.
* Set the new root.
*/
template <class Key, class Value>
void AvlTree<Key, Value>::insert(const Key &x, const Value &y, AvlNode<Key, Value>* &t) const {
    if (t == NULL) {
        t = new AvlNode<Key, Value>(x, y); // Assuming default Value constructor exists
    }
    else if (x < t->key) {
        insert(x, y, t->left);
        if (height(t->left) - height(t->right) == 2) {
            if (x < t->left->key) {
                rotateWithLeftChild(t);
            }
            else {
                doubleWithLeftChild(t);
            }
        }
    }
    else if (t->key < x) {
        insert(x, y, t->right);
        if (height(t->right) - height(t->left) == 2) {
            if (t->right->key < x) {
                rotateWithRightChild(t);
            }
            else {
                doubleWithRightChild(t);
            }
        }
    }
    else {
        ;  // Duplicate; do nothing
    }
    t->height = max(height(t->left), height(t->right)) + 1;
}


/**
* Internal method to clone subtree.
*/
template <class Key, class Value>
AvlNode<Key, Value>* AvlTree<Key, Value>::clone(AvlNode<Key, Value>* t) const {
    if (t == NULL)
        return NULL;
    else
        return new AvlNode<Key, Value>(t->key, t->value, clone(t->left), clone(t->right));
}
