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
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void rebalanceUp(AVLNode<Key, Value>* start, int8_t initialDiff, bool stopOnInsertBehavior);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

    // if tree is empty, we can just insert at root and skip the rest
    if(this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_); 

    // find insertion spot or existing key
    while (true) {
        // key already exists so we can just update and return
        if (new_item.first == curr->getKey()) {
            curr->setValue(new_item.second);
            return;
        } 
        // key is less than current, so go left
        else if (new_item.first < curr->getKey()) {
            // no left child, so we can insert to left
            if (curr->getLeft() == nullptr) {
                AVLNode<Key, Value>* node = new AVLNode<Key, Value>(new_item.first, new_item.second, curr);
                curr->setLeft(node);

                // use helper fxn, initialDiff should be 1 since we added to the left, and our insertion detector should be set to true
                rebalanceUp(curr, 1, true);
                return;
            } 
            // left child exists
            else {
                curr = curr->getLeft();
            }
        }
        // going right since key is greater than current 
        else {
            if (curr->getRight() == nullptr) {
                AVLNode<Key, Value>* node = new AVLNode<Key, Value>(new_item.first, new_item.second, curr);
                curr->setRight(node);

                // use helper fxn, initialDiff should be -1 since we added to the right, and our insertion detector should be set to true
                rebalanceUp(curr, -1, true);
                return;
            } 
            // right child exists
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
    // TODO 

    // error catch if key not in tree already
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(nodeToRemove == nullptr) {
        return; // key not found
    }

    // check to see if nodeToRemove has 2 kids, if so then we swap until there's only <=1 kid associated with it 
    while (nodeToRemove->getLeft() != nullptr && nodeToRemove->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeToRemove));
        nodeSwap(nodeToRemove, pred); // swap so nodeToRemove will still point at the node we want to nuke but placed in pred spot
    }

    // populate a child node pointer with either the leftmost child or, after we run out of left kids, the right child
    AVLNode<Key, Value>* child = (nodeToRemove->getLeft() != nullptr) ? nodeToRemove->getLeft() : nodeToRemove->getRight(); // this syntax is so much better looking than the if statements that would take its place tbh

    AVLNode<Key, Value>* parent = nodeToRemove->getParent();

    // checking for which side was reduced for initialDiff in our rebalanceUp call later
    bool shrunkLeft = false;
    if(parent != nullptr){
        shrunkLeft = (parent->getLeft() == nodeToRemove);
    }

    // ohhh my god the nesting is so ugly here
    // if there's no parent node (i.e. we're at root) then nuke root
    if (parent == nullptr) {
        this->root_ = child;

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

    // now we can rebalance if needed
    if(parent != nullptr){
        rebalanceUp(parent, (shrunkLeft ? -1 : +1), false); // initialDiff will depend on which side had an element removed from it
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node){
    if (node == nullptr) return;
    AVLNode<Key, Value>* r = node->getRight();
    if (r == nullptr) return;

    AVLNode<Key, Value>* p = node->getParent();

    node->setRight(r->getLeft());
    if (r->getLeft() != nullptr) r->getLeft()->setParent(node);

    r->setLeft(node);
    node->setParent(r);

    r->setParent(p);
    if (p == nullptr) this->root_ = r;
    else if (static_cast<AVLNode<Key, Value>*>(p)->getLeft() == node) p->setLeft(r);
    else p->setRight(r);
}

// just a repeat of rotateLeft but reversed left/right
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){
    if (node == nullptr) return;
    AVLNode<Key, Value>* l = node->getLeft();
    if (l == nullptr) return;

    AVLNode<Key, Value>* p = node->getParent();

    node->setLeft(l->getRight());
    if (l->getRight() != nullptr) l->getRight()->setParent(node);

    l->setRight(node);
    node->setParent(l);

    l->setParent(p);
    if (p == nullptr) this->root_ = l;
    else if (static_cast<AVLNode<Key, Value>*>(p)->getLeft() == node) p->setLeft(l);
    else p->setRight(l);
}

// helper function for rebalancing (i got annoyed by repeating my code in insert and remove)
template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceUp(AVLNode<Key, Value>* start, int8_t initialDiff, bool stopOnInsertBehavior)
{
    if (start == nullptr) return;

    AVLNode<Key, Value>* parent = start;
    AVLNode<Key, Value>* child = (initialDiff == 1) ? start->getLeft() : start->getRight();
    int8_t diff = initialDiff;

    while (parent != nullptr) {
        parent->updateBalance(diff);
        int8_t b = parent->getBalance();

        if (stopOnInsertBehavior && b == 0) return; // height unchanged on insert
        if (!stopOnInsertBehavior && (b == 1 || b == -1)) return; // height unchanged on delete

        if (b == 2) { // left heavy
            AVLNode<Key, Value>* L = parent->getLeft();
            if (L->getBalance() >= 0) { // LL
                rotateRight(parent);
                if (L->getBalance() == 0) {
                    parent->setBalance(1);
                    L->setBalance(-1);
                    if (stopOnInsertBehavior) return;
                } else {
                    parent->setBalance(0);
                    L->setBalance(0);
                }
                child = L;
            } else { // LR
                AVLNode<Key, Value>* LR = L->getRight();
                rotateLeft(L);
                rotateRight(parent);
                int8_t cb = LR->getBalance();
                if (cb == 1) { parent->setBalance(-1); L->setBalance(0); }
                else if (cb == 0) { parent->setBalance(0); L->setBalance(0); }
                else if (cb == -1) { parent->setBalance(0); L->setBalance(1); }
                LR->setBalance(0);
                child = LR;
            }
        } else if (b == -2) { // right heavy
            AVLNode<Key, Value>* R = parent->getRight();
            if (R->getBalance() <= 0) { // RR
                rotateLeft(parent);
                if (R->getBalance() == 0) {
                    parent->setBalance(-1);
                    R->setBalance(1);
                    if (stopOnInsertBehavior) return;
                } else {
                    parent->setBalance(0);
                    R->setBalance(0);
                }
                child = R;
            } else { // RL
                AVLNode<Key, Value>* RL = R->getLeft();
                rotateRight(R);
                rotateLeft(parent);
                int8_t cb = RL->getBalance();
                if (cb == -1) { parent->setBalance(1); R->setBalance(0); }
                else if (cb == 0) { parent->setBalance(0); R->setBalance(0); }
                else if (cb == 1) { parent->setBalance(0); R->setBalance(-1); }
                RL->setBalance(0);
                child = RL;
            }
        }

        AVLNode<Key, Value>* up = (child != nullptr) ? child->getParent() : parent->getParent();
        if (up == nullptr) return;
        diff = (child != nullptr) ? ((child == up->getLeft()) ? 1 : -1)
                                  : ((parent == up->getLeft()) ? 1 : -1);
        parent = up;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t temparentBalance = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(temparentBalance);
}


#endif
