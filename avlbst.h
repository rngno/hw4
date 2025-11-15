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

                // initialDiff should be 1 since we added to the left, make sure to set insertion detector!!!
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

                // initialDiff should be -1 since we added to the right, make sure to set insertion detector!!!
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
        return;
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
        // check which side got shrank, insertion detector CANNOT be set here
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

    // jesus figuring this part out gave me a headache
    // node2's left child now node's right child
    node->setRight(node2->getLeft());
    if(node2->getLeft() != nullptr){
        node2->getLeft()->setParent(node);
    }

    // node now node2's left child
    node2->setLeft(node);
    node->setParent(node2);

    // clean up parents to maintain AVL tree order/connectivity
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

    // mirrored copy of rotateLeft
    // node2's right child now node's left child
    node->setLeft(node2->getRight());
    if(node2->getRight() != nullptr){
        node2->getRight()->setParent(node);
    }

    // node now node2's right child
    node2->setRight(node);
    node->setParent(node2);

    // clean up parents to maintain AVL tree order/connectivity
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
    
    AVLNode<Key, Value>* child = nullptr; // need this to maintain tree order later
    int8_t diff = initialDiff;

    while (parent != nullptr) {
        
        parent->updateBalance(diff);
        int8_t parentBalance = parent->getBalance(); 

        // no rebalance needed quite yet, just checking for insertion logic
        if (parentBalance == 0) {
            // insertion!! we don't need to go any further for insert() here :3
            if (stopOnInsertBehavior) {
                break;
            }
        } 
        else if (parentBalance == 1 || parentBalance == -1) {
            // removal!! we don't need to go any further for removal() here :3
            if (!stopOnInsertBehavior) {
                break;
            }
        }

        if (parentBalance != 2 && parentBalance != -2) {
            child = parent; // time to walk up the tree
            parent = parent->getParent();

            if (parent == nullptr){
                break;
            }

            // left subtree got changed by insert() or remove()
            if (child == parent->getLeft()) {
                diff = stopOnInsertBehavior ? 1 : -1;
            } 
            // right subtree got changed by insert() or remove()
            else {
                diff = stopOnInsertBehavior ? -1 : 1;
            }
            continue;
        }

        // actually handling rebalance
        // left has too many nodes
        if (parentBalance == 2) {
            AVLNode<Key, Value>* left = parent->getLeft();
            
            // right rotation (zig-zig tests check here!!!)
            if (left->getBalance() >= 0) {
                rotateRight(parent);

                // handling a removal
                if (left->getBalance() == 0) {
                    parent->setBalance(1);
                    left->setBalance(-1);
                    if (!stopOnInsertBehavior){
                        break;
                    }
                } 
                // handling an insertion
                else {
                    parent->setBalance(0);
                    left->setBalance(0);
                    if (stopOnInsertBehavior) {
                        break;
                    }
                }
                
                child = left; 

            } 
            // zig-zag tests here!! double rotation stuff
            else {
                AVLNode<Key, Value>* LR = left->getRight();
                rotateLeft(left);
                rotateRight(parent);

                // balance handling post-rotation
                if (LR->getBalance() == 1) {
                    parent->setBalance(-1);
                    left->setBalance(0);
                } 
                else if (LR->getBalance() == 0) {
                    parent->setBalance(0);
                    left->setBalance(0);
                } 
                else { 
                    parent->setBalance(0);
                    left->setBalance(1);
                }
                LR->setBalance(0);
                
                // insertion can stop here
                if (stopOnInsertBehavior){
                    break;
                }
                
                child = LR; 
            }
        } 
        
        // right subtree has too many nodes
        else {
            AVLNode<Key, Value>* right = parent->getRight();

            // zig-zig but left this time lol
            if (right->getBalance() <= 0) { 
                rotateLeft(parent);


                // removal handling
                if (right->getBalance() == 0) {
                    parent->setBalance(-1);
                    right->setBalance(1);

                    // removal can stop here
                    if (!stopOnInsertBehavior) {
                        break;
                    }
                } 
                // insertion handling
                else {
                    parent->setBalance(0);
                    right->setBalance(0);

                    // insertion can stop here
                    if (stopOnInsertBehavior) {
                        break;
                    }
                }
                
                child = right; 

            } 
            // zig-zag tests but RL lol
            else { 
                AVLNode<Key, Value>* RL = R->getLeft();
                rotateRight(R);
                rotateLeft(parent);

                // balance handling post-rotation
                if (RL->getBalance() == -1) {
                    parent->setBalance(1);
                    right->setBalance(0);
                } else if (RL->getBalance() == 0) {
                    parent->setBalance(0);
                    right->setBalance(0);
                } else { // RL->getBalance() == 1
                    parent->setBalance(0);
                    right->setBalance(-1);
                }
                RL->setBalance(0);
                
                if (stopOnInsertBehavior) {
                    break;
                }
                
                child = RL; 
            }
        }
        
        // height definitely shrank if we made it here, so time to start from the root again
        AVLNode<Key, Value>* up = child->getParent();
        if (up == nullptr) {
            break;
        }

        // time to walk up the tree again
        parent = up;
        diff = (child == parent->getLeft()) ? -1 : 1;
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
