#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

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
        //successor helper funt
        iterator& successor();
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
    // Note:  static means these functions don't have a "this" pointer
    //        and instead just use the input argument.

    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2) ;

    // Add helper functions here
    void postorderDestroyer(Node<Key, Value>* nodePtr);
    int countSteps(Node<Key, Value>* nodePtr) const;
    bool balHelper(Node<Key, Value>* root) const;
    void f2(Node<Key, Value>* nodePtr);
    void f3(Node<Key, Value>* nodePtr);
    void f4(Node<Key, Value>* nodePtr);

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
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value> *ptr): current_(ptr) {}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(): current_(nullptr) {}

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
BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return (this -> current_ == rhs.current_);
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return (this -> current_ != rhs.current_);
}


/**
* finds successor of this ptr
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::successor()
{
    //case where current is root_ and right subtree is empty
    if (this -> current_ -> getParent() == nullptr && this -> current_ -> getRight() == nullptr) 
    {
        this -> current_ = nullptr;
        return *this;
    }

    //case where you can take a step down first
    if (this -> current_ -> getRight() != nullptr) 
    {
        this -> current_ = this -> current_ -> getRight();
        while (this -> current_ -> getLeft() != nullptr)
        {
            this -> current_ = this -> current_ -> getLeft();
        }
        return *this;
    }

    //otherwise, this is the case where you can't take a step down so you have to move up
    while (this -> current_ != nullptr && this -> current_ -> getParent() != nullptr && this -> current_ -> getParent() -> getLeft() != this -> current_) 
    {
        this -> current_ = this -> current_ -> getParent();
    }

    /*
    if (this -> current_ -> getParent() == nullptr)
    {
        this -> current_ = nullptr;
        return *this;
    }
    */

    if (this -> current_ == nullptr) //checking if current was actually smallest node in the tree. returning nullptr if so.
    {
        this -> current_ = nullptr;
        return *this;
    }
    this -> current_ = this -> current_ -> getParent();
    return *this;
}

/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    if (this -> current_ -> getParent() == nullptr && this -> current_ -> getLeft() == nullptr && this -> current_ -> getRight() == nullptr)
    {
      this -> current_ = nullptr;
      return *this; //operator++ called on single node tree
    }
    return this -> successor();
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

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree(): root_(nullptr) {} // DOUBLE CHECK HERE

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clear();
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
    if (root_ == nullptr)
    {
        Node<Key, Value>* n = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, nullptr);
        root_ = n;
        return;
    }
    Node<Key, Value>* finder = root_;

    while (finder != nullptr)
    {
        if (keyValuePair.first == finder->getKey()) //nodes are equal
        {
            finder -> setValue(keyValuePair.second);
            return;
        }
        else if (keyValuePair.first < finder->getKey())
        {
            if (finder -> getLeft() == nullptr)
            {
                Node<Key, Value>* n = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, finder);
                finder -> setLeft(n);
                return;
            }
            finder = finder -> getLeft();
        }
        else if(keyValuePair.first > finder->getKey())
        {
            if (finder -> getRight() == nullptr)
            {
                Node<Key, Value>* n = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, finder);
                finder -> setRight(n);
                return;
            }
            finder = finder -> getRight();
        }
    }
}


/**
* A remove method to remove a specific key from a Binary Search Tree.
* Recall: The writeup specifies that if a node has 2 children you
* should swap with the predecessor and then remove.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::f2(Node<Key, Value>* nodePtr)
{
        std::cout << "f2" << std::endl;
        while (nodePtr->getLeft() != nullptr && nodePtr->getRight() != nullptr)
        {
          Node<Key, Value>* pred = predecessor(nodePtr);
          std::cout << "about to swap nodePtr, which is " << nodePtr -> getKey() << ", with it's pred, which is " << pred -> getKey() << std::endl;
          nodeSwap(nodePtr, predecessor(nodePtr));
        }
        if (nodePtr->getLeft() != nullptr || nodePtr->getRight() != nullptr)
        {
          f4(nodePtr);
          return;
        }
        else
        {
          f3(nodePtr);
          return;
        }
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::f3(Node<Key, Value>* nodePtr)
{
        std::cout << "f3" << std::endl;
        if (nodePtr -> getParent() -> getLeft() == nodePtr)
        {
            nodePtr -> getParent() -> setLeft(nullptr);
        }
        else if (nodePtr -> getParent() -> getRight() == nodePtr)
        {
            nodePtr -> getParent() -> setRight(nullptr);
        }
        delete nodePtr;
        return;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::f4(Node<Key, Value>* nodePtr)
{
    std::cout << "f4" << std::endl;
    Node<Key, Value>* child;
      if (nodePtr -> getLeft() == nullptr)
      {
        child = nodePtr -> getRight();
      }
      else
      {
        child = nodePtr -> getLeft();
      }
      if (nodePtr -> getParent() -> getRight() == nodePtr) // is a right child
      {
        nodePtr -> getParent() -> setRight(nodePtr -> getRight()); //set parent's child
      }
      else // is a left child
      {
        nodePtr -> getParent() -> setLeft(child); //set parent's child
      }

      child -> setParent(nodePtr -> getParent()); //set childs's parent
      delete nodePtr; //DELETE!

      std::cout << "deleting " << nodePtr -> getKey() << std::endl;
      std::cout << "new head is " << root_ -> getKey() << " who has (right) child " << root_ -> getRight() -> getKey() << std::endl;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key) 
{
    Node<Key, Value>* nodePtr = internalFind(key);

    /*
    Case 1: Node DNE in given tree.
    */
    if (nodePtr == nullptr)
    {
      std::cout << "f1" << std::endl;
      return;
    }

    /*
    Case 1.5: Deleting root_ with either NO children or ONE child
    */
    if (nodePtr -> getParent() == nullptr)
    {
      std::cout << "f1.5" << std::endl;
        if (nodePtr->getLeft() == nullptr && nodePtr->getRight() != nullptr) //only a right node
        { 
          root_ = nodePtr -> getRight();
          root_ -> setParent(nullptr);
          delete nodePtr;
          return;
        }
      else if (nodePtr->getLeft() != nullptr && nodePtr->getRight() == nullptr) //only a left node
        { 
          root_ = nodePtr -> getLeft();
          root_ -> setParent(nullptr);
          delete nodePtr;
          return;
        }
        else if (nodePtr -> getLeft() == nullptr && nodePtr -> getRight() == nullptr) //neither left or right node
        {
          delete nodePtr;
          root_ = nullptr;
          return;
        }
    }
    /*
    Case 2: Node has two children.
    */
    if (nodePtr->getLeft() != nullptr && nodePtr->getRight() != nullptr)
    {
      f2(nodePtr);
      return;
    }
    /*
    Case 3: Node has no children (non root_ case)
    */
    if (nodePtr->getLeft() == nullptr && nodePtr->getRight() == nullptr)
    {
      f3(nodePtr);
      return;
    }
    /*
    Case 4: Node has one child (non root_ case)
    */
   if (nodePtr->getLeft() == nullptr || nodePtr->getRight() == nullptr)
   {
    f4(nodePtr);
    return;
   }
}



template<class Key, class Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    //case where current is root_ and left subtree is empty
    if (current -> getParent() == nullptr && current -> getLeft() == nullptr) 
    {
        return nullptr;
    }

    //case where you can take a step down first
    if (current -> getLeft() != nullptr) 
    {
        current = current -> getLeft();
        while (current -> getRight() != nullptr)
        {
          current = current -> getRight();
        }
        std::cout << "returning " << current -> getKey() << "from pred function" << std::endl;
        return current;
    }

    //otherwise, this is the case where you can't take a step down so you have to move up
    while (current -> getParent() -> getRight() != current && current != nullptr) 
    {
        current = current -> getParent();
    }

    if (current == nullptr) //checking if current was actually smallest node in the tree. returning nullptr if so.
    {
        return nullptr;
    }
    current = current -> getParent();
    return current;
}


/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::postorderDestroyer(Node<Key, Value>* nodePtr)
{
    if (nodePtr != nullptr) {
      postorderDestroyer(nodePtr->getLeft());
      postorderDestroyer(nodePtr->getRight());
      delete nodePtr;
   }
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    postorderDestroyer(root_);
    root_ = nullptr;
}


/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>*
BinarySearchTree<Key, Value>::getSmallestNode() const
{
    Node<Key, Value>* finder = root_;
    while (finder -> getLeft() != nullptr)
    {
        finder = finder -> getLeft();
    }
    return finder;
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    Node<Key, Value>* finder = root_;
    while (finder != nullptr && finder -> getKey() != key)
    {
        if (key < finder->getItem().first)
        {
          finder = finder -> getLeft();
        }
        else if(key > finder->getItem().first)
        {
          finder = finder -> getRight();
        }
    }
    return finder;
}

template<class Key, class Value>
int BinarySearchTree<Key, Value>::countSteps(Node<Key, Value>* nodePtr) const //countSteps takes a root and returns the num of steps 
//that can be taken down the tree, to the furthest leaf node
{
    if (nodePtr == nullptr)
    {
        return 0;
    }
    int leftSteps = countSteps(nodePtr -> getLeft());
    int rightSteps = countSteps(nodePtr -> getRight());
    return std::max(leftSteps, rightSteps) + 1; //"max" is taken in case either leftsteps or rightsteps reaches the base case AND to use the variable that is storing the prev countSteps return val
}

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::balHelper(Node<Key, Value>* root) const
{
    if (root == nullptr) //iff we've reached all root nodes without violating balancing property, return true
    {
        return true;
    }

    if (!balHelper(root -> getLeft())) return false;
    if (!balHelper(root -> getRight())) return false;

    int leftCount = countSteps(root->getLeft()); //depth of subtree rooted at root_->getLeft()
    int rightCount = countSteps(root->getRight()); //depth of subtree rooted at root_->getRight()
    int diff = std::abs(rightCount - leftCount); //difference between trees

    return diff <= 1;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
  Node<Key, Value>* rootPointer = root_;
  bool isBal = balHelper(rootPointer);
  return isBal;
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
