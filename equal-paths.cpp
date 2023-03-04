#ifndef RECCHECK
#include <iostream>
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int pathNum(Node* root)
{
    if (root == NULL) { // base case
        return 0;
    }

    int num = 0;
    int left = pathNum(root->left);
    int right = pathNum(root->right);

    if (left >= right) {
        num = left;
    }
    else if (left <= right) {
        num = right;
    }
    
    ++num;
    return num;
}

bool equalPaths(Node * root)
{
    // Add your code below
    if (root == NULL) { // base case
        return true;
    }
    else if (root->left != NULL && root->right == NULL) { // left child
        return equalPaths(root->left);
    }
    else if (root->left == NULL && root->right != NULL) { // right child
        return equalPaths(root->right);
    }

    else { // both child
        int left = pathNum(root->left);
        int right = pathNum(root->right);
        if (left != right) {
            return false;
        }
    }
    return true;
}

