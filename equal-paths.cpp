#ifndef RECCHECK
#include <iostream>
#include <algorithm>
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int heightFunc(Node* node)
{
    if (node == NULL) { // base case
        return 0;
    }

    int num = 0;
    int left = heightFunc(node->left);
    int right = heightFunc(node->right);

    if (left >= right) {
        num = left;
    }
    else if (left <= right) {
        num = right;
    }
    
    ++num;
    return num;
  // if (node == NULL) {
  //   return 0;
  // }
  // else {
  //   return 1 + max(heightFunc(node->left), heightFunc(node->right));
  // }
}

bool equalPaths(Node * root)
{
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
        int left = heightFunc(root->left);
        int right = heightFunc(root->right);
        if (left != right) {
            return false;
        }
    }
    return true;
    // int left, right;

    // if (root == NULL) {
    //   return true;
    // }

    // left = heightFunc(root->left);
    // right = heightFunc(root->right);

    // if (left != right) {
    //   return false;
    // }
    // return true;
}

