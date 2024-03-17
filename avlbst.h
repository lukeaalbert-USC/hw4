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
    bool ZigZigLeft(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent);
    bool ZigZigRight(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent);
    bool ZigZagLeft(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent);
    bool ZigZagRight(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent);
    void rotateRight(AVLNode<Key,Value>* node);
    void rotateLeft(AVLNode<Key,Value>* node);
    void removeFix(AVLNode<Key,Value>* parent, int diff);
    AVLNode<Key, Value>* getTaller(AVLNode<Key, Value>* left, AVLNode<Key, Value>* right);
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
                if (finder -> getBalance() == 1)
                {
                    finder -> setBalance(0);
                    return;
                }
                else if (finder -> getBalance() == 0)
                {
                   finder -> updateBalance(-1);
                   insertFix(finder, n); 
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
                if (finder -> getBalance() == -1)
                {
                    finder -> setBalance(0);
                    return;
                }
                else if (finder -> getBalance() == 0)
                {
                  finder -> updateBalance(1);
                  insertFix(finder, n);
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
    AVLNode<Key, Value>* toRemove = dynamic_cast<AVLNode<Key, Value>*>(this -> internalFind(key)); //get node to remove

    if (toRemove == nullptr) //if not found, return
    {
        return;
    }

    if (toRemove -> getLeft() != nullptr && toRemove -> getRight() != nullptr) //2 child case
    {
        AVLNode<Key, Value>* predecessor = dynamic_cast<AVLNode<Key, Value>*>(this -> predecessor(dynamic_cast<Node<Key, Value>*>(toRemove)));
        nodeSwap(toRemove, predecessor);
    }

    //at this point, it's assumed that toRemove has either NO or only ONE child
    AVLNode<Key, Value>* parent = toRemove -> getParent();
    int diff;

    if (parent != nullptr)
    {
        if (parent -> getLeft() == toRemove)
        {
            diff = 1;
            parent -> updateBalance(1);
        }
        else if (parent -> getRight() == toRemove)
        {
            diff = -1;
            parent -> updateBalance(-1);
        }
    }

    if (parent == nullptr) //root case with either 1 or 0 children. recall, root can only have bal of 1, 0, or -1.
    {
        if (toRemove -> getBalance() == 0) //single root_ node
        {
            delete toRemove;
            this -> root_ = nullptr;
        }
        else if (toRemove -> getBalance() == 1) //root_ node with single right child
        {
            nodeSwap(toRemove, toRemove -> getRight());
            delete toRemove;
        }
        else if (toRemove -> getBalance() == -1)//root_ node with single left child
        {
            nodeSwap(toRemove, toRemove -> getLeft());
            delete toRemove;
        }
        return;
    }

    if (toRemove -> getLeft() == nullptr && toRemove -> getRight() == nullptr) //no children case
    {
        if (parent -> getLeft() == toRemove)
        {
            parent -> setLeft(nullptr);
        }
        else
        {
            parent -> setRight(nullptr);
        }
        delete toRemove;
    }

    else if (toRemove -> getLeft() == nullptr || toRemove -> getRight() == nullptr) //single child case
    {
        if (parent -> getLeft() == toRemove && toRemove -> getLeft() != nullptr)
        {
            parent -> setLeft(toRemove -> getLeft());
            toRemove -> getLeft() -> setParent(parent);
        }
        else if (parent -> getLeft() == toRemove && toRemove -> getRight() != nullptr)
        {
            parent -> setLeft(toRemove -> getRight());
            toRemove -> getRight() -> setParent(parent);
        }
        else if (parent -> getRight() == toRemove && toRemove -> getLeft() != nullptr)
        {
            parent -> setRight(toRemove -> getLeft());
            toRemove -> getLeft() -> setParent(parent);
        }
        else if (parent -> getRight() == toRemove && toRemove -> getRight() != nullptr)
        {
            parent -> setRight(toRemove -> getRight());
            toRemove -> getRight() -> setParent(parent);
        }
        delete toRemove;
    }

    removeFix(parent, diff);
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
          return;
        }
        else if (grandParent -> getBalance() == -2)
        {
            if (ZigZigLeft(child, grandParent))
            {
                rotateRight(grandParent);
                grandParent -> setBalance(0);
                parent -> setBalance(0);
                return;
            }
            else if (ZigZagLeft(child, grandParent))
            {
                rotateLeft(parent);
                rotateRight(grandParent);
                if (child -> getBalance() == -1)
                {
                    parent -> setBalance(0);
                    grandParent -> setBalance(1);
                    child -> setBalance(0);
                }
                else if (child -> getBalance() == 0)
                {
                    parent -> setBalance(0);
                    grandParent -> setBalance(0);
                    child -> setBalance(0);
                }
                else if (child -> getBalance() == 1)
                {
                    parent -> setBalance(-1);
                    grandParent -> setBalance(0);
                    child -> setBalance(0);
                }
                return;
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
            return;
        }
        else if (grandParent -> getBalance() == 2)
        {
            if (ZigZigRight(child, grandParent))
            {
                rotateLeft(grandParent);
                grandParent -> setBalance(0);
                parent -> setBalance(0);
                return;
            }
            else if (ZigZagRight(child, grandParent))
            {
                rotateRight(parent);
                rotateLeft(grandParent);
                if (child -> getBalance() == 1)
                {
                    parent -> setBalance(0);
                    grandParent -> setBalance(-1);
                    child -> setBalance(0);
                }
                else if (child -> getBalance() == 0)
                {
                    parent -> setBalance(0);
                    grandParent -> setBalance(0);
                    child -> setBalance(0);
                }
                else if (child -> getBalance() == -1)
                {
                    parent -> setBalance(1);
                    grandParent -> setBalance(0);
                    child -> setBalance(0);
                }
                return;
            }
        }
    }
}

template<class Key, class Value>
bool AVLTree<Key, Value>::ZigZigLeft(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent)
{
  return (grandParent -> getLeft() -> getLeft() == child);
}

template<class Key, class Value>
bool AVLTree<Key, Value>::ZigZigRight(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent)
{
  return (grandParent -> getRight() -> getRight() == child);
}
template<class Key, class Value>
bool AVLTree<Key, Value>::ZigZagLeft(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent)
{
   return (grandParent -> getLeft() -> getRight() == child);
}

template<class Key, class Value>
bool AVLTree<Key, Value>::ZigZagRight(AVLNode<Key,Value>* child, AVLNode<Key,Value>* grandParent)
{
   return (grandParent -> getRight() -> getLeft() == child);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node)
{

    AVLNode<Key,Value>* newParent = node -> getLeft();

    if (newParent == nullptr) //checking if invalid rotateRight call (for clarity)
    {
        return;
    }

    if (node == this -> root_) //root case
    {
        this -> root_ = newParent; //root_ is now left child
        newParent -> setParent(nullptr); // root parent has to be null
        node -> setLeft(newParent->getRight()); //node's left val is it's left child's right val(s)
        if (node -> getLeft() != nullptr)
        {
          node -> getLeft() -> setParent(node);
        }
        node -> setParent(newParent); // node parent changes to node's left child
        newParent -> setRight(node); //root right changes to node
        return;
    }
    else //non-root case
    {
        newParent -> setParent(node -> getParent()); // new parent's parent is node's old parent
        if (node -> getParent() -> getLeft() == node) //setting the parent of nodes child
        {
            node -> getParent() -> setLeft(newParent);
        }
        else
        {
            node -> getParent() -> setRight(newParent);
        }
        node -> setParent(newParent); // node parent changes to node's left child
        node -> setLeft(newParent->getRight()); //node's left val is it's left child's right val(s)
        if (node -> getLeft() != nullptr)
        {
          node -> getLeft() -> setParent(node);
        }
        newParent -> setRight(node); //root right changes to node
        return;
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node)
{
  AVLNode<Key,Value>* newParent = node -> getRight();

    if (newParent == nullptr) //checking if invalid rotateRight call (for clarity)
    {
        return;
    }

    if (node == this -> root_) //root case
    {
        this -> root_ = newParent; //root_ is now left child
        newParent -> setParent(nullptr); // root parent has to be null
        node -> setRight(newParent->getLeft()); //node's left val is it's left child's right val(s)
        if (node -> getRight() != nullptr)
        {
          node -> getRight() -> setParent(node);
        }
        node -> setParent(newParent); // node parent changes to node's left child
        newParent -> setLeft(node); //root right changes to node
        return;
    }

    else //non-root case
    {
        newParent -> setParent(node -> getParent()); // new parent's parent is node's old parent
        if (node -> getParent() -> getLeft() == node) //setting the parent of nodes child
        {
          node -> getParent() -> setLeft(newParent);
        }
        else
        {
          node -> getParent() -> setRight(newParent);
        }
        node -> setParent(newParent); // node parent changes to node's left child
        node -> setRight(newParent->getLeft()); //node's left val is it's left child's right val(s)
        if (node -> getRight() != nullptr)
        {
          node -> getRight() -> setParent(node);
        }
        newParent -> setLeft(node); //root right changes to node
        return;
    }
}

template <class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::getTaller(AVLNode<Key,Value>* left, AVLNode<Key,Value>* right)
{
    if (left == nullptr)
    {
        return right;
    }
    if (right == nullptr)
    {
        return left;
    }
    if (this -> countSteps(dynamic_cast<Node<Key, Value>*>(left)) > this -> countSteps(dynamic_cast<Node<Key, Value>*>(right)))
    {
        return left;
    }
    return right;
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int diff)
{
    if (n == nullptr) //base case
    {
        return;
    }

    //info for next recursive call
    AVLNode<Key,Value>* nextParent = n -> getParent();
    int nextDiff;
    if (nextParent != nullptr)
    {
        if (nextParent -> getLeft() == n)
        {
            nextDiff = 1;
        }
        else if (nextParent -> getRight() == n)
        {
            nextDiff = -1;
        }
    }

    if (diff == -1) //case where node child was a right child
    {
        if (n -> getBalance() + diff == -2) //new balance would be -2 (cause an inbalance)
        {
            //[Perform the check for the mirror case where b(n) + diff == +2, flipping left/right and -1/+1]
            AVLNode<Key,Value>* c = getTaller(nextParent -> getLeft(), nextParent -> getRight());

            if (c -> getBalance() == -1 ) //zig zig case
            {
                rotateRight(n);
                n -> setBalance(0);
                c -> setBalance(0);
                removeFix(nextParent, nextDiff);
            }
            else if (c -> getBalance() == 0) //zig zig case
            {
                rotateRight(n);
                n -> setBalance(-1);
                c -> setBalance(1);
            }
            else if (c -> getBalance() == 1) // zig zag case
            {
                AVLNode<Key,Value>* g = c -> getRight();
                rotateLeft(c);
                rotateRight(n);
                if (g -> getBalance() == 1)
                {
                    n -> setBalance(0);
                    c -> setBalance(-1); 
                    g -> setBalance(0);
                }
                else if (g -> getBalance() == 0)
                {
                    n -> setBalance(0);
                    c -> setBalance(0); 
                    g -> setBalance(0);
                }
                else if (g -> getBalance() == -1)
                {
                    n -> setBalance(1);
                    c -> setBalance(0); 
                    g -> setBalance(0);
                }
                removeFix(nextParent, nextDiff);
            }
            return;
        }
        else if (n -> getBalance() + diff == -1)
        {
            n -> setBalance(-1);
            return;
        }
        else if (n -> getBalance() + diff == 0)
        {
            n -> setBalance(0);
            removeFix(nextParent, nextDiff);
            return;
        }
    }


    if (diff == 1) //case where node child was a right child
    {
        if (n -> getBalance() + diff == 2) //new balance would be 2 (cause an inbalance)
        {
            //[Perform the check for the mirror case where b(n) + diff == -2, flipping left/right and -1/+1]
            AVLNode<Key,Value>* c = getTaller(nextParent -> getLeft(), nextParent -> getRight());

            if (c -> getBalance() == 1 ) //zig zig case
            {
                rotateLeft(n);
                n -> setBalance(0);
                c -> setBalance(0);
                removeFix(nextParent, nextDiff);
            }
            else if (c -> getBalance() == 0) //zig zig case
            {
                rotateLeft(n);
                n -> setBalance(1);
                c -> setBalance(-1);
            }
            else if (c -> getBalance() == -1) // zig zag case
            {
                AVLNode<Key,Value>* g = c -> getLeft();
                rotateRight(c);
                rotateLeft(n);
                if (g -> getBalance() == -1)
                {
                    n -> setBalance(0);
                    c -> setBalance(1); 
                    g -> setBalance(0);
                }
                else if (g -> getBalance() == 0)
                {
                    n -> setBalance(0);
                    c -> setBalance(0); 
                    g -> setBalance(0);
                }
                else if (g -> getBalance() == 1)
                {
                    n -> setBalance(-1);
                    c -> setBalance(0); 
                    g -> setBalance(0);
                }
                removeFix(nextParent, nextDiff);
            }
            return;
        }
        else if (n -> getBalance() + diff == 1)
        {
            n -> setBalance(1);
            return;
        }
        else if (n -> getBalance() + diff == 0)
        {
            n -> setBalance(0);
            removeFix(nextParent, nextDiff);
            return;
        }
    }
}
#endif
