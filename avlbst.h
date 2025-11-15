#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO testing if insert() from bst has issue

    // if tree is empty, we can just insert at root and skip the rest
    if(root_ == NULL) {
        root_ = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
        return;
    }

    Node<Key, Value>* curr = root_; 

    // in this loop, walk thru tree to find if key already exists or where to insert new node
    while(true) {
        // key already exists, so just update value and return
        if(keyValuePair.first == curr->getKey()) {
            curr->setValue(keyValuePair.second);
            return;
        }

        // key doesn't exist, so time to start walking

        // key is less than current, so go left
        else if(keyValuePair.first < curr->getKey()) {
            // no left child, so we can insert to left
            if(curr->getLeft() == NULL) {
                Node<Key, Value>* newNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, curr); // REMEMBER TO FREE
                curr->setLeft(newNode);
                return; // placed node, so we can end here
            }
            // left child exists, so take it from top of loop
            else {
                curr = curr->getLeft();
            }
        }
        // key is greater than current, so go right
        else {
            // no right child, so we can insert to right
            if(curr->getRight() == NULL) {
                Node<Key, Value>* newNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, curr); // REMEMBER TO FREE
                curr->setRight(newNode);
                return; // placed node, so we can end here
            }
            // right child exists, so take it from top of loop
            else {
                curr = curr->getRight();
            }
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO testing if remove from bst has issue

    // error catch if key not in tree already
    Node<Key, Value>* nodeToRemove = internalFind(key);
    if(nodeToRemove == nullptr) {
        return; // key not found
    }

    // check to see if nodeToRemove has 2 kids, if so then we swap until there's only <=1 kid associated with it 
    while (nodeToRemove->getLeft() != nullptr && nodeToRemove->getRight() != nullptr) {
        Node<Key, Value>* pred = predecessor(nodeToRemove);
        nodeSwap(nodeToRemove, pred); // swap so nodeToRemove will still point at the node we want to nuke but placed in pred spot
    }

    // populate a child node pointer with either the leftmost child or, after we run out of left kids, the right child
    Node<Key, Value>* child = (nodeToRemove->getLeft() != nullptr) ? nodeToRemove->getLeft() : nodeToRemove->getRight(); // this syntax is so much better looking than the if statements that would take its place tbh

    Node<Key, Value>* parent = nodeToRemove->getParent();

    // ohhh my god the nesting is so ugly here
    // if there's no parent node (i.e. we're at root) then nuke root
    if (parent == nullptr) {
        root_ = child;

        // child becomes new root node if the rest of the tree isn't empty
        if (child != nullptr){
            child->setParent(nullptr);
        }
    }

    // there is a parent node so we let a child take parent's place
    else {
        // can use given getters and setters declared in class 
        // node to be nuked is on the left of our parent node, so use child to take its place
        if (parent->getLeft() == nodeToRemove) {
            parent->setLeft(child);
        } 

        // node to be nuked is on the right of our parent node, so use child to take its place
        else {
            parent->setRight(child);
        }

        // preserve tree structure by having child's parent become parent node
        if (child != nullptr){
            child->setParent(parent);
        }
    }

    delete nodeToRemove;

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
