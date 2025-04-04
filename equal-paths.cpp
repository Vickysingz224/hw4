#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
// Note: Went to Office Hours
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

//helper function declarations 

int findHeight(Node * root, int height)
{
    // Base case to check if node is null and returns -1
    if(root == nullptr)
    {
        return -1;
    }
    
    // check if the node is a leaf node, meaning at end of the tree and return the height
    if( root->left == NULL && root->right == NULL)
    {
        return height;
    }

    // Two variables to store the heights of the left and right subtrees recursively
    int leftH = findHeight(root->left , height +1 );
    int rightH = findHeight(root->right, height +1);

    // Checks if there is no leaf in the left subtree, and return the height from the right subtree
    if(leftH == -1)
    {
        return rightH;
    }

    // Checks if there is no leaf node in right subtree and returns height from left subtree
    if(rightH == -1)
    {
        return leftH;
    }

    // return height from left subtree as final case
    return leftH;
}

// function for checking if the leaf nodes are at the same height
bool checkLeafPath( Node * root, int height, int goal)
{
     // Base case to check if root is null and returns true
     if(root == NULL)
     {
         return true;
     }

     // checks if this node is a leaf node and checks if the height matches the target
     if(root ->left== NULL && root->right == NULL)
     {
        return height == goal;
     }

     // Checking through recursion that all leaves in both subtrees are at the specfied goal height
     return checkLeafPath(root->left, height +1, goal) &&  checkLeafPath(root->right, height +1, goal);
}


bool equalPaths(Node * root)
{
    // Add your code below
      // Base case to check if an empty tree or single node tree contains equal paths
     if( root== NULL)
     {
        return true;
     }

     // Checks if a node has equal paths
     if(root->left == NULL && root->right == NULL)
     {
        return true;
     }

     // Declaration of a height variable to store the height of the first leaf node 
     int newHeight = findHeight(root, 0);

     // checks if this height = -1, and return true suggesting there are no leaves to compare
     if(newHeight == -1)
     {
        return true;
     }

     // this checks if all the leaf nodes are the same height
     return checkLeafPath(root,0, newHeight);
}

