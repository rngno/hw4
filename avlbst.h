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

                // initialDiff should be 1 since we added to the left. StopOnInsert=true.
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

                // initialDiff should be -1 since we added to the right. StopOnInsert=true.
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
    // error catch if key not in tree already
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(nodeToRemove == nullptr) {
        return; // key not found
    }

    // check to see if nodeToRemove has 2 kids, if so then we swap until there's only <=1 kid associated with it 
    while (nodeToRemove->getLeft() != nullptr && nodeToRemove->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeToRemove));
        nodeSwap(nodeToRemove, pred); 
    }

    // populate a child node pointer with either the leftmost child or, after we run out of left kids, the right child
    AVLNode<Key, Value>* child = (nodeToRemove->getLeft() != nullptr) ? nodeToRemove->getLeft() : nodeToRemove->getRight(); 

    AVLNode<Key, Value>* parent = nodeToRemove->getParent();

    // checking for which side was reduced for initialDiff in our rebalanceUp call later
    bool shrunkLeft = false;
    if(parent != nullptr){
        shrunkLeft = (parent->getLeft() == nodeToRemove);
    }

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
        // Removal uses stopOnInsertBehavior=false.
        // If left subtree shrank, balance change is -1. If right shrank, balance change is +1.
        rebalanceUp(parent, (shrunkLeft ? -1 : +1), false); 
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node){
    if (node == nullptr || node->getRight() == nullptr){
        return;
    }

    AVLNode<Key, Value>* node2 = node->getRight();
    AVLNode<Key, Value>* parent = node->getParent();

    // 1. Move node2's left child to node's right
    node->setRight(node2->getLeft());
    if(node2->getLeft() != nullptr){
        node2->getLeft()->setParent(node);
    }

    // 2. node becomes node2's left child
    node2->setLeft(node);
    node->setParent(node2);

    // 3. Update parent pointers
    node2->setParent(parent);
    if(parent == nullptr){
        this->root_ = node2;
    }
    else if(parent->getLeft() == node){
        parent->setLeft(node2);
    }
    else{
        parent->setRight(node2);
    }
}

// just a repeat of rotateLeft but reversed left/right
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node){
    if (node == nullptr || node->getLeft() == nullptr){
        return;
    }

    AVLNode<Key, Value>* node2 = node->getLeft(); 
    AVLNode<Key, Value>* parent = node->getParent();

    // 1. Move node2's right child to node's left
    node->setLeft(node2->getRight());
    if(node2->getRight() != nullptr){
        node2->getRight()->setParent(node);
    }

    // 2. node becomes node2's right child
    node2->setRight(node);
    node->setParent(node2);

    // 3. Update parent pointers
    node2->setParent(parent);
    if(parent == nullptr){
        this->root_ = node2;
    }
    else if(parent->getLeft() == node){
        parent->setLeft(node2);
    }
    else{
        parent->setRight(node2);
    }
}

// helper function for rebalancing (i got annoyed by repeating my code in insert and remove)
template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceUp(AVLNode<Key, Value>* parent, int8_t initialDiff, bool stopOnInsertBehavior)
{
    if (parent == nullptr) {
        return;
    }

    // `parent` is the node currently being checked.
    // `diff` is the balance change from the child subtree.
    
    AVLNode<Key, Value>* child = nullptr; // Tracks the new subtree root after rotation, or the node we left when traversing up.
    int8_t diff = initialDiff;

    while (parent != nullptr) {
        
        parent->updateBalance(diff);
        int8_t parentBalance = parent->getBalance(); 

        // 1. Check for Termination / Traversal (No Rotation needed yet)

        // Case A: Balance is 0
        if (parentBalance == 0) {
            if (stopOnInsertBehavior) { // INSERTION: Height is same, stop.
                break;
            }
            // REMOVAL: Height shrank, continue up.
        } 
        // Case B: Balance is +/-1
        else if (parentBalance == 1 || parentBalance == -1) {
            if (!stopOnInsertBehavior) { // REMOVAL: Height maintained, stop.
                break;
            }
            // INSERTION: Height increased, continue up.
        }
        // Case C: Balance is +/-2 (Rotation required) - Handled below

        // If we are continuing up (parentBalance was 0 for removal, or +/-1 for insertion)
        if (parentBalance != 2 && parentBalance != -2) {
            child = parent; // The node we just finished checking
            parent = parent->getParent();

            if (parent == nullptr){
                break;
            }

            // Determine new diff for the next level up.
            if (child == parent->getLeft()) {
                // Left subtree changed: +1 for insert, -1 for removal
                diff = stopOnInsertBehavior ? 1 : -1;
            } else {
                // Right subtree changed: -1 for insert, +1 for removal
                diff = stopOnInsertBehavior ? -1 : 1;
            }
            continue; // Go to the next iteration
        }

        // 2. ROTATION LOGIC (parentBalance is 2 or -2)

        // Left Heavy (+2)
        if (parentBalance == 2) {
            AVLNode<Key, Value>* L = parent->getLeft();
            
            if (L->getBalance() >= 0) { // Zig-Zig Left (LL or L0)
                // Zig-Zig Rotation (Right Rotation)
                rotateRight(parent);

                if (L->getBalance() == 0) { // L0 case (only possible on removal/shrink)
                    parent->setBalance(1);
                    L->setBalance(-1);
                    if (!stopOnInsertBehavior) break; // L0 case stops rebalancing for removal
                } else { // LL case (L->getBalance() == 1)
                    parent->setBalance(0);
                    L->setBalance(0);
                    if (stopOnInsertBehavior) break; // LL case stops rebalancing for insertion
                }
                
                child = L; // New subtree root

            } else { // Zig-Zag Left (LR) (L->getBalance() == -1)
                // Double Rotation (Left(L) then Right(P))
                AVLNode<Key, Value>* LR = L->getRight();
                rotateLeft(L);
                rotateRight(parent);

                // Balance updates based on LR's balance before rotation
                if (LR->getBalance() == 1) {
                    parent->setBalance(-1);
                    L->setBalance(0);
                } else if (LR->getBalance() == 0) {
                    parent->setBalance(0);
                    L->setBalance(0);
                } else { // LR->getBalance() == -1
                    parent->setBalance(0);
                    L->setBalance(1);
                }
                LR->setBalance(0);
                
                if (stopOnInsertBehavior) break; // Double rotation always stops rebalancing for insertion
                
                child = LR; // New subtree root
            }
        } 
        
        // Right Heavy (-2)
        else { // parentBalance == -2
            AVLNode<Key, Value>* R = parent->getRight();

            if (R->getBalance() <= 0) { // Zig-Zig Right (RR or R0)
                // Zig-Zig Rotation (Left Rotation)
                rotateLeft(parent);

                if (R->getBalance() == 0) { // R0 case (only possible on removal/shrink)
                    parent->setBalance(-1);
                    R->setBalance(1);
                    if (!stopOnInsertBehavior) break; // R0 case stops rebalancing for removal
                } else { // RR case (R->getBalance() == -1)
                    parent->setBalance(0);
                    R->setBalance(0);
                    if (stopOnInsertBehavior) break; // RR case stops rebalancing for insertion
                }
                
                child = R; // New subtree root

            } else { // Zig-Zag Right (RL) (R->getBalance() == 1)
                // Double Rotation (Right(R) then Left(P))
                AVLNode<Key, Value>* RL = R->getLeft();
                rotateRight(R);
                rotateLeft(parent);

                // Balance updates based on RL's balance before rotation
                if (RL->getBalance() == -1) {
                    parent->setBalance(1);
                    R->setBalance(0);
                } else if (RL->getBalance() == 0) {
                    parent->setBalance(0);
                    R->setBalance(0);
                } else { // RL->getBalance() == 1
                    parent->setBalance(0);
                    R->setBalance(-1);
                }
                RL->setBalance(0);
                
                if (stopOnInsertBehavior) break; // Double rotation always stops rebalancing for insertion
                
                child = RL; // New subtree root
            }
        }
        
        // If we reach here after a rotation during removal (and didn't break on L0/R0), 
        // the height shrank, and we need to continue up from the new root (`child`).
        
        AVLNode<Key, Value>* up = child->getParent();
        if (up == nullptr) break;
        
        parent = up;
        // Removal diff continues up.
        diff = (child == parent->getLeft()) ? -1 : 1;
        // Loop will continue to check the balance of the new parent.
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
