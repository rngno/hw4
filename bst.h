#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
//#include "equal-paths.h"

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;

    // declaration for checkDepth function from equal-paths.cpp ported over here
    static int checkDepth(Node<Key,Value>* root);
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed by the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); //TODO
    virtual ~BinarySearchTree(); //TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    virtual void remove(const Key& key); //TODO
    void clear(); //TODO
    bool isBalanced() const; //TODO
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const & operator[](const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; // TODO
    Node<Key, Value> *getSmallestNode() const;  // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    static Node<Key, Value>* successor(Node<Key, Value>* current); // added successor declaration as recommended in guide
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here


protected:
    Node<Key, Value>* root_;
    // You should not need other data members
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr)
{
    current_ = ptr; // is it this easy??? 
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() 
{
    current_ = nullptr; // this should be already associated with a BST, so we can just set to NULL
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return (current_ == rhs.current_); // check if the node pointers are the same, if yes then they should have same internals
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return !(current_ == rhs.current_); // same deal as above but negated
}


/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    current_ = BinarySearchTree<Key, Value>::successor(current_);
    return *this; // TODO want this to return the iterator, but not sure if *this is correct over current_
}


/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

template<typename Key, typename Value>
Node<Key,Value>* BinarySearchTree<Key,Value>::successor(Node<Key,Value>* current) {
    // check if right child exists
    if(current->getRight() != nullptr) {
        Node<Key, Value>* kid = current->getRight();

        // go as far down left as possible
        while(kid->getLeft() != nullptr) {
            kid = kid->getLeft();
        }

        return kid;
    }

    // couldn't find right child, so now we move on to parent that can actually be a successor
    else {
        Node<Key, Value>* ancestor = current->getParent();

        // trying to find a parent that has current as its left child
        while(ancestor != nullptr && current == ancestor->getRight()) {
            current = ancestor; // keep track of where we're walking as we go up
            ancestor = ancestor->getParent(); // keep walking up as necessary
        }

        return ancestor;
    }
}

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{
    // instantiate an empty tree
    BinarySearchTree<Key, Value>::root_ = NULL;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    BinarySearchTree<Key, Value>::clear(); // use built in clear function
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * @precondition The key exists in the map
 * Returns the value associated with the key
 */
template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}
template<class Key, class Value>
Value const & BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
* Recall: If key is already in the tree, you should 
* overwrite the current value with the updated value.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    // TODO DEF COME BACK

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


/**
* A remove method to remove a specific key from a Binary Search Tree.
* Recall: The writeup specifies that if a node has 2 children you
* should swap with the predecessor and then remove.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    // TODO DEF COME BACK

    // error catch if key not in tree already
    Node<Key, Value>* nodeToRemove = internalFind(key);
    if(nodeToRemove == nullptr) {
        return; // key not found
    }

    // node has no kids
    if(nodeToRemove->getLeft() == nullptr && nodeToRemove->getRight() == nullptr) {
        Node<Key, Value>* parent = nodeToRemove->getParent();  // use parent so we can keep the structure of the tree after the node gets nuked

        // if we're at root node
        if(parent == nullptr) {
            root_ = nullptr; // set root to null (basically deleted tree)
        }

        // move on to left child (checking to see if left child even exists)
        else if(parent->getLeft() == nodeToRemove) {
            parent->setLeft(NULL); // swap out predecessor with left child
        }

        // right child
        else {
            parent->setRight(NULL); // swap out predecessor with right child
        }
        delete nodeToRemove;
    }

    // node only has one child
    else if(nodeToRemove->getLeft() == nullptr || nodeToRemove->getRight() == nullptr) {
        Node<Key, Value>* child = (nodeToRemove->getLeft() != nullptr) ? nodeToRemove->getLeft() : nodeToRemove->getRight();
        Node<Key, Value>* parent = nodeToRemove->getParent();

        // if we're at root node
        if(parent == nullptr) {
            root_ = child; // swap out root with child
            child->setParent(NULL); // set new root's parent to null, since nothing should be above it
        }

        // left child
        else if(parent->getLeft() == nodeToRemove) {
            parent->setLeft(child); // swap out predecessor with left child
            child->setParent(parent); // set child's parent to nodeToRemove's parent (parent's parent)
        }

        // right child
        else {
            parent->setRight(child); // swap out predecessor with right child
            child->setParent(parent); // set child's parent to nodeToRemove's parent (parent's parent)
        }
        delete nodeToRemove;
    }

    // node has 2 children
    else {
        Node<Key, Value>* pred = predecessor(nodeToRemove); 
        nodeSwap(nodeToRemove, pred);
        // go back and remove nodeToRemove (which is now in pred's original position)
        remove(key); // recursive call to remove now that we swapped
    }
}



template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    // TODO

    // error catch if current is entered as null mistakenly
    if(current == nullptr) {
        return nullptr;
    }

    // if left child exists, go down left once
    if(current->getLeft() != NULL) {
        Node<Key, Value>* pred = current->getLeft();

        // now go to right as far as we can
        while(pred->getRight() != NULL) {
            pred = pred->getRight();
        }

        return pred;
    }
    // no left child, so go up to parent until we find a node that is a right child of its parent
    else {
        Node<Key, Value>* ancestor = current->getParent();

        // loop until we find a node that is a right child of its parent
        while(ancestor != nullptr && current == ancestor->getLeft()) {
            current = ancestor; // keep track of where we're walking as we go up
            ancestor = ancestor->getParent(); // keep walking up as necessary
        }

        return ancestor;
    }
}


/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    // TODO
    while(root_ != nullptr) {
        remove(root_->getKey()); // keep removing root until tree is empty
    }    
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    // TODO

    // error catch if tree is empty
    if(root_ == nullptr) {
        return nullptr;
    }

    // walk down left until we can't anymore (left child always smaller than right/parent)
    Node<Key, Value>* curr = root_;
    while(curr->getLeft() != NULL) {
        curr = curr->getLeft();
    }

    return curr;
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    // TODO
    Node<Key, Value>* curr = root_;

    // walk thru tree to find key
    while(curr != nullptr) {
        // found key, just return pointer
        if(key == curr->getKey()) {
            return curr;
        }
        // key is less than current, so go left
        else if(key < curr->getKey()) {
            curr = curr->getLeft();
        }
        // key is greater than current, so go right
        else {
            curr = curr->getRight();
        }
    }
    return nullptr; // key not in tree
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    // TODO
   //return checkDepth(root_) != -1; // i think we can just use the helper fxn from equalPaths? this is asking for the same thing basically 

    return checkBalanced(root_) != -1; // testing new helper fxn
}

// helper function for isBalanced, returns -1 if unbalanced, else returns depth of subtree
template<typename Key, typename Value>
static int checkDepth(Node<Key,Value> *root) {
    // base case, no children/empty subtree
    if (root == nullptr){
        return -2; 
    }

    // base case leaf node
    if (root->left_ == nullptr && root->right_ == nullptr){
        return 0; 
    }

    // check left and right subtrees for depth
    int leftTree = checkDepth(root->left_);
    if (leftTree == -1){
        return -1; // depth doesn't match in left subtree
    }
    int rightTree = checkDepth(root->right_);
    if (rightTree == -1){
        return -1; // depth doesn't match in right subtree
    }

    // check for missing kids (that sounds weird in retrospect)
    if (leftTree == -2){
        return rightTree + 1; // no left child, use right depth
    }
    if (rightTree == -2){
        return leftTree + 1; // no right child, use left depth
    }

    // now we know both kids are there, so compare depths
    return (leftTree == rightTree) ? (leftTree + 1) : -1;
}

// helper function for isBalanced, returns -1 if unbalanced, else returns height of subtree
// testing this out since checkDepth is a little broken lol
template<typename Key, typename Value>
static int checkBalanced(Node<Key,Value>* node) {
    // base case
    if (node == nullptr) {
        return 0;
    }

    // recurse down left subtree 
    int leftTree = checkBalanced(node->getLeft());
    if (leftTree == -1) {
        return -1;
    }

    // recurse down right subtree
    int rightTree = checkBalanced(node->getRight());
    // right subtree unbalanced
    if (rightTree == -1){
        return -1;
    }

    // now check balance btwn subtrees
    int diff = (leftTree > rightTree) ? (leftTree - rightTree) : (rightTree - leftTree); // could make this an if statement but it looks cuter this way (i hate nesting) 
    if (diff > 1){
        return -1;
    }

    // balanced, might as well return the height lol
    return 1 + ((leftTree > rightTree) ? leftTree : rightTree); // could make this an if statement but it looks cuter this way (i hate nesting)
}


template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->root_ == n1) {
        this->root_ = n2;
    }
    else if(this->root_ == n2) {
        this->root_ = n1;
    }

}

/**
 * Lastly, we are providing you with a print function,
   BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->root_) // or any other node pointer

   It will print up to 5 levels of the tree rooted at the passed node,
   in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#endif
