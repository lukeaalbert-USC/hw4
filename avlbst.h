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
    void insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node);
    bool ZigZig(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent);
    bool ZigZag(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent);
    void rotateRight(AVLNode<Key,Value>* node);
    void rotateLeft(AVLNode<Key,Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    //check if head
    if (this -> root_ == nullptr)
    {

        Node<Key, Value>* newRoot = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr); //be careful of dynamic data here!!! you may be allocating data that could cause leaks. 
        this -> root_ = newRoot;
        return;
    }

    //vars to find spot in tree
    AVLNode<Key, Value>* finder = dynamic_cast<AVLNode<Key, Value>*>(this -> root_);

    //find spot in tree
    while (finder != nullptr)
    {
        if (new_item.first == finder->getKey()) //nodes are equal
        {
            finder -> setValue(new_item.second);
            return;
        }

        else if (new_item.first < finder->getKey()) //move in left direction
        {
            if (finder -> getLeft() == nullptr)
            {
                AVLNode<Key, Value>* n = new AVLNode<Key, Value>(new_item.first, new_item.second, finder); //be careful of dynamic data here!!! you may be allocating data that could cause leaks. 
                finder -> setLeft(n);
                if (n -> getParent() -> getBalance() == 1 || n -> getParent() -> getBalance() == -1)
                {
                    return;
                }
                else
                {
                   insertFix(n -> getParent(), n); 
                   return;
                }
            }
            finder = finder -> getLeft();
        }

        else if(new_item.first > finder->getKey()) //move in right direction
        {
            if (finder -> getRight() == nullptr)
            {
                AVLNode<Key, Value>* n = new AVLNode<Key, Value>(new_item.first, new_item.second, finder); //be careful of dynamic data here!!! you may be allocating data that could cause leaks. 
                finder -> setRight(n);
                if (n -> getParent() -> getBalance() == 1 || n -> getParent() -> getBalance() == -1)
                {
                    return;
                }
                else
                {
                   insertFix(n -> getParent(), n);
                   return;
                }
            }
            finder = finder -> getRight();
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
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


/*
HELPER
FUNCTIONS
*/

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child)
{
    if (parent == nullptr || parent -> getParent() == nullptr) //base case
    {
        return;
    }

    AVLNode<Key,Value>* grandParent = parent -> getParent(); //declare grandparent

    if (grandParent -> getLeft() == parent) //Left Rotation Case
    {
        grandParent -> updateBalance(-1);
        if (grandParent -> getBalance() == 0)
        {
            return;
        }
        else if (grandParent -> getBalance() == -1)
        {
            insertFix(grandParent, parent);
        }
        else if (grandParent -> getBalance() == -2)
        {
            if (ZigZig(child, grandParent))
            {
                rotateRight(grandParent);
                grandParent -> setBalance(0);
                parent -> setBalance(0);
            }
            else if (ZigZag(child, grandParent))
            {
                rotateLeft(parent);
                rotateRight(grandParent);
                if (child -> getBalance() == -1)
                {
                    parent -> updateBalance(0);
                    grandParent -> updateBalance(1);
                    child -> updateBalance(0);
                }
                else if (child -> getBalance() == 0)
                {
                    parent -> updateBalance(0);
                    grandParent -> updateBalance(0);
                    child -> updateBalance(0);
                }
                else if (child -> getBalance() == 1)
                {
                    parent -> updateBalance(-1);
                    grandParent -> updateBalance(0);
                    child -> updateBalance(0);
                }
            }
        }
    }

    else if (grandParent -> getRight() == parent)
    {
        grandParent -> updateBalance(1);
        if (grandParent -> getBalance() == 0)
        {
            return;
        }
        else if (grandParent -> getBalance() == 1)
        {
            insertFix(grandParent, parent);
        }
        else if (grandParent -> getBalance() == 2)
        {
            if (ZigZig(child, grandParent))
            {
                rotateLeft(grandParent);
                grandParent -> setBalance(0);
                parent -> setBalance(0);
            }
            else if (ZigZag(child, grandParent))
            {
                rotateRight(parent);
                rotateLeft(grandParent);
                if (child -> getBalance() == 1)
                {
                    parent -> updateBalance(0);
                    grandParent -> updateBalance(-1);
                    child -> updateBalance(0);
                }
                else if (child -> getBalance() == 0)
                {
                    parent -> updateBalance(0);
                    grandParent -> updateBalance(0);
                    child -> updateBalance(0);
                }
                else if (child -> getBalance() == -1)
                {
                    parent -> updateBalance(1);
                    grandParent -> updateBalance(0);
                    child -> updateBalance(0);
                }
            }
        }
    }
}

template<class Key, class Value>
bool AVLTree<Key, Value>::ZigZig(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent)
{
   return (grandParent -> getLeft() -> getLeft() == child || grandParent -> getRight() -> getRight() == child);
}

template<class Key, class Value>
bool AVLTree<Key, Value>::ZigZag(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent)
{
   return (grandParent -> getLeft() -> getRight() == child || grandParent -> getRight() -> getLeft() == child);
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node)
{
    return; //update me!!!
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node)
{
    return; //update me!!!
}
#endif
