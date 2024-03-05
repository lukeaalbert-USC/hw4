#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"


#include <iostream> //DELETE MEEEE


using namespace std;


// You may add any prototypes of helper functions here

int countSteps(Node * root) //countSteps takes a root and returns the num of steps 
//that can be taken down the tree, to the furthest leaf node
{
    if (root == nullptr)
    {
        return 0;
    }
    int leftSteps = countSteps(root -> left);
    int rightSteps = countSteps(root -> right);
    return max(leftSteps, rightSteps) + 1; //"max" is taken in case either leftsteps or rightsteps reaches the base case AND to use the variable that is storing the prev countSteps return val
}


bool equalPaths(Node * root) //main function
{
    if (root == nullptr)
    {
        return true;
    }

    if (countSteps(root->left) != countSteps(root->right)) //if the num of steps to leaf nodes at any given subtree in the tree aren't equal, the entire funt returns false
    {
        return false;
    }

    return equalPaths(root->left) && equalPaths(root->right); //finally, we check the rest of all of the subtrees. if this hits base case, we get true.
}

