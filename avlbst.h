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
    void RR(AVLNode <Key, Value>* currNode);
    void RL(AVLNode <Key, Value>* currNode);
    void insertHelperFunc(AVLNode <Key, Value>* currNode, AVLNode <Key, Value>* currNode2);
    void removeHelperFunc(AVLNode <Key, Value>* currNode, int8_t val);


};



template<class Key, class Value>
void AVLTree<Key, Value>::RR(AVLNode <Key, Value>* currNode)
{
    AVLNode<Key, Value>* left = currNode->getLeft();
    AVLNode<Key, Value>* parent = currNode->getParent();

    if (parent != NULL) {
        if (parent->getRight() == currNode) {
            currNode->setRight(left);
        }
        else {
            parent->setLeft(left);
        }
    }
    else {
        this->root_ = left;
    }

    left->setParent(parent);
    currNode->setParent(left);

    AVLNode<Key, Value>* leftright = left->getRight();
    if (leftright != NULL) {
        leftright->setParent(currNode);
    }

    currNode->setLeft(leftright);
    left->setRight(currNode);
}



template<class Key, class Value>
void AVLTree<Key, Value>::RL(AVLNode <Key, Value>* currNode)
{
    AVLNode<Key, Value>* parent = currNode->getParent();
	AVLNode<Key, Value>* right = currNode->getRight();

    if (parent != NULL) {
        if (parent->getRight() == currNode) {
            parent->setRight(right);
        }
        else {
            parent->setLeft(right);
        }
    }

    else {
        this->root_ = right;
    }

    right->setParent(parent);
    currNode->setParent(right);
    AVLNode<Key, Value>* rightleft = right->getLeft();

    if (rightleft != NULL) {
        rightleft->setParent(currNode);
    }
    currNode->setRight(rightleft);
    right->setLeft(currNode);
}



template<class Key, class Value>
void AVLTree<Key, Value>::insertHelperFunc(AVLNode <Key, Value>* currNode, AVLNode <Key, Value>* currNode2)
{
    if (currNode == NULL || currNode->getParent() == NULL) return;
    AVLNode<Key, Value>* currNodeParent = currNode->getParent();

    if (currNodeParent->getRight() == currNode) {
        currNodeParent->updateBalance(1);
        if (currNodeParent->getBalance() == 0) {
            return;
        }
        else if (currNodeParent->getBalance() == 1) {
            insertHelperFunc(currNodeParent, currNode);
        }
        else {
            if (currNode->getRight() == currNode2) {
                RL(currNodeParent);
                currNodeParent->setBalance(0);
                currNode->setBalance(0);
                return;
            }
            else {
                RR(currNode);
                RL(currNodeParent);
                int8_t currNode2Balance = currNode2->getBalance();
                if (currNode2Balance == -1) {
                    currNode->setBalance(1);
                    currNodeParent->setBalance(0);
                    currNode2->setBalance(0);
                } 
                else if (currNode2Balance == 0) {
                    currNode->setBalance(0);
                    currNodeParent->setBalance(0);
                    currNode2->setBalance(0);
                }
                else if (currNode2Balance == 1) {
                    currNode->setBalance(0);
                    currNodeParent->setBalance(-1);
                    currNode2->setBalance(0);
                }
                return;
            }
        }
    }
    else if (currNodeParent->getLeft() == currNode2) {
        currNodeParent->updateBalance(-1);
        if (currNodeParent->getBalance() == 0) return;

        else if (currNodeParent->getBalance() == -1) {
            insertHelperFunc(currNodeParent, currNode2);
        }
        else {
            if (currNode->getLeft() == currNode2) {
                RR(currNodeParent);
                currNode->setBalance(0);
                currNodeParent->setBalance(0);
                return;
            }
            else {
                RL(currNode);
                RR(currNodeParent);
                int8_t currNode2Balance = currNode2->getBalance();

                if (currNode2Balance == -1) {
                    currNode->setBalance(0);
                    currNodeParent->setBalance(1);
                    currNode2->setBalance(0);
                } 
                else if (currNode2Balance == 0) {
                    currNode->setBalance(0);
                    currNodeParent->setBalance(0);
                    currNode2->setBalance(0);
                }
                else if (currNode2Balance == 1) {
                    currNode->setBalance(-1);
                    currNodeParent->setBalance(0);
                    currNode2->setBalance(0);
                }
                return;
            }
        }
    }
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

    Key currKey = new_item.first;
    Value currVal = new_item.second;

    if (!(this->empty())) {
        AVLNode<Key, Value>* currRoot = static_cast<AVLNode<Key, Value>*>(this->root_);
        while (true) {
            Key currRootKey = currRoot->getKey();
            if (currKey == currRootKey) {
                currRoot->setValue(currVal);
                break;
            }
            else if (currKey < currRootKey) {
                if (currRoot->getLeft() == NULL) {
                    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(currKey, currVal, currRoot);
                    currRoot->setLeft(newNode);
                    if (currRoot->getBalance() == 1) {
                        currRoot->setBalance(0);
                    }
                    else if (currRoot->getBalance() == 0) {
                        currRoot->setBalance(-1);
                        insertHelperFunc(currRoot, newNode);
                    }
                    break;
                }
                currRoot = currRoot->getLeft();
            }
            else if (currKey > currRootKey) {
                if (currRoot->getRight() == NULL) {
                    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(currKey, currVal, currRoot);
                    currRoot->setRight(newNode);
                    if (currRoot->getBalance() == -1) {
                        currRoot->setBalance(0);
                    }
                    else if (currRoot->getBalance() == 0) {
                        currRoot->setBalance(1);
                        insertHelperFunc(currRoot, newNode);
                    }
                    break;
                }
                currRoot = currRoot->getRight();
            }
        }
    }
    else {
        AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(currKey, currVal, NULL); 
        this->root_ = newNode;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeHelperFunc(AVLNode<Key, Value>* currNode, int8_t val)
{
    if (currNode == NULL) return;

    AVLNode<Key, Value>* parent = currNode->getParent();
    int8_t nextVal;
    if (parent != NULL) {
      if (parent->getLeft() == currNode) {
        nextVal = 1;
      }
      else {
        nextVal = -1;
      }
    }

    int8_t currNodeBalance = currNode->getBalance();
    int8_t currNodeBalDiff = currNodeBalance + val;

    if (abs(currNodeBalDiff) == 2) {
      if (currNodeBalDiff == 2) {
        AVLNode<Key, Value>* currNodeRight = currNode->getRight();
        int8_t currNodeRightBalance = currNodeRight->getBalance();
        if (currNodeRightBalance == 1) {
          RL(currNode);
          currNode->setBalance(0);
          currNodeRight->setBalance(0);
          removeHelperFunc(parent, nextVal);
        }
        else if (currNodeRightBalance == 0) {
          RL(currNode);
          currNode->setBalance(1);
          currNodeRight->setBalance(-1);
        }
        else if (currNodeRightBalance == -1) {
          AVLNode<Key, Value>* currNodeRightLeft = currNodeRight->getLeft();
          RR(currNodeRight);
          RL(currNode);
          int8_t currNodeRightLeftBalance = currNodeRightLeft->getBalance();
          if (currNodeRightLeftBalance == 1) {
            currNode->setBalance(-1);
            currNodeRight->setBalance(0);
            currNodeRightLeft->setBalance(0);
          }
          else if (currNodeRightLeftBalance == 0) {
            currNode->setBalance(0);
            currNodeRight->setBalance(0);
            currNodeRightLeft->setBalance(0);
          }
          else if (currNodeRightLeftBalance == -1) {
            currNode->setBalance(0);
            currNodeRight->setBalance(1);
            currNodeRightLeft->setBalance(0);
          }
          removeHelperFunc(parent, nextVal);
        }
      }

      if (currNodeBalDiff == -2) {
        AVLNode<Key, Value>* currNodeLeft = currNode->getLeft();
        int8_t currNodeLeftBalance = currNodeLeft->getBalance();
        if (currNodeLeftBalance == 1) {
          AVLNode<Key, Value>* currNodeLeftRight = currNodeLeft->getRight();
          RL(currNodeLeft);
          RR(currNode);
          int8_t currNodeLeftRightBal = currNodeLeftRight->getBalance();
          if (currNodeLeftRightBal == 1) {
            currNode->setBalance(0);
            currNodeLeft->setBalance(-1);
            currNodeLeftRight->setBalance(0);
          }
          else if (currNodeLeftRightBal == 0) {
            currNode->setBalance(0);
            currNodeLeft->setBalance(0);
            currNodeLeftRight->setBalance(0);
          }
          else if (currNodeLeftRightBal == -1) {
            currNode->setBalance(1);
            currNodeLeft->setBalance(0);
            currNodeLeftRight->setBalance(0);
          }
          removeHelperFunc(parent, nextVal);
        }
      }
    }
    else if (abs(currNodeBalDiff) == 1) {
      if (currNodeBalDiff == -1) {
        currNode->setBalance(-1);
        return;
      }
      else {
        currNode->setBalance(1);
        return;
      }
    }
    else if (currNodeBalDiff == 0) {
      removeHelperFunc(parent, nextVal);
      currNode->updateBalance(val);
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

  if (this->root_ == NULL) {
    return;
  }
  
  else {
    AVLNode<Key, Value>* currRoot = static_cast<AVLNode<Key, Value>*>(this->root_);
    while (true) {
      Key currRootKey = currRoot->getKey();
      if (currRootKey == key) {
        if (currRoot->getLeft() == NULL && currRoot->getRight() == NULL) {
          if (currRoot == this->root_) {
            currRoot->setBalance(0);
            this->root_ = NULL;
            delete this->root_;
            break;
          }
          AVLNode<Key, Value> *parent = currRoot->getParent();
          if (parent->getRight() == currRoot) {
            parent->setRight(NULL);
            removeHelperFunc(parent, -1);
          }
          else if (parent->getLeft() == currRoot) {
            parent->setLeft(NULL);
            removeHelperFunc(parent, 1);
          }
          currRoot->setParent(NULL);
          delete currRoot;
          break;
        }
        else if (currRoot->getLeft() == NULL || currRoot->getRight() == NULL) {
          if (currRoot->getRight() != NULL) {
            AVLNode<Key, Value>* right = currRoot->getRight();
            AVLNode<Key, Value>* parent = currRoot->getParent();
            if (parent == NULL) {
              currRoot->setBalance(0);
              this->root_ = right;
              right->setParent(NULL);
              currRoot->setParent(NULL);
              currRoot->setRight(NULL);
              break;
            }
            if (currRoot == parent->getLeft()) {
              parent->setLeft(right);
              right->setParent(parent);
              removeHelperFunc(parent, 1);
            }
            else if (currRoot == parent->getRight()) {
              parent->setRight(right);
              right->setParent(parent);
              removeHelperFunc(parent, -1);
            }
            currRoot->setParent(NULL);
            currRoot->setRight(NULL); 
          }
          else if (currRoot->getLeft() != NULL) {
            AVLNode<Key, Value>* left = currRoot->getLeft();
            AVLNode<Key, Value>* parent = currRoot->getParent();
            if (parent == NULL) {
              currRoot->setBalance(0);
              this->root_ = left;
              left->setParent(NULL);
              currRoot->setParent(NULL);
              currRoot->setLeft(NULL);
              break;
            }
            if (currRoot == parent->getLeft()) {
              parent->setLeft(left);
              left->setParent(parent);
              removeHelperFunc(parent, 1);
            }
            else if (currRoot == parent->getRight()) {
              parent->setRight(left);
              left->setParent(parent);
              removeHelperFunc(parent, -1);
            }
            currRoot->setParent(NULL);
            currRoot->setLeft(NULL);
          }
          delete currRoot;
          break;
        }
        else {
          AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(currRoot));
          nodeSwap(currRoot, pred);
          Key removeCurrRootKey = currRoot->getKey();
          remove(removeCurrRootKey);
        }
      }
      else if (currRootKey < key) {
        if (currRoot->getRight() == NULL) {
          break;
        }
        else {
          currRoot = currRoot->getRight();
        }
      }
      else if (currRootKey > key) {
        if (currRoot->getLeft() == NULL) {
          break;
        }
        else {
          currRoot = currRoot->getLeft();
        }
      }
    }
  }
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
