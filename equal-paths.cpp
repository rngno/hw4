#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <cstddef>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
static int checkDepth(Node *root); // returns >=0 depth if subtree's leaf-to-root paths are equal, -1 if mismatch

bool equalPaths(Node * root)
{
    // Add your code below
    if (root == nullptr){
        return true; // there aren't any paths to walk down, so they are all equal lol 
    }
    
    return checkDepth(root) != -1; // check to see if there are any unequal paths (-1 = unequal)
}

static int checkDepth(Node *root) {
    // base case, no children/empty subtree
    if (root == nullptr){
        return -2; 
    }

    // base case leaf node
    if (root->left == nullptr && root->right == nullptr){
        return 0; 
    }

    // check left and right subtrees for depth
    int leftTree = checkDepth(root->left);
    if (leftTree == -1){
        return -1; // depth doesn't match in left subtree
    }
    int rightTree = checkDepth(root->right);
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

