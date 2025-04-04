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

    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void fixInsert(AVLNode<Key, Value>* parentNode, AVLNode<Key,Value>* child);
    void fixRemove(AVLNode<Key, Value>* node, int8_t dif);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    // Checks if the root is null, and sets to a new node  in the AVL tree
    if (this->root_ == nullptr) 
    {
      this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
      return;
    }

   // Declaration of two variables to hold the instance of the cureent node and parent node pointer
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parentNode = nullptr;

   // Checks if the current pointer is not null
   while (current != nullptr) 
   {
     parentNode = current;
    
     // checks if the new item firt side is equal to  the ky of the current node, and sets the value to second
     if (new_item.first == current->getKey())
     {
         
        current->setValue(new_item.second);
        return;
     } 
     // Checks  if the first of new item is less than the key of current, and sets current to left, and right if opposite
     else if (new_item.first < current->getKey()) 
     {
        current = current->getLeft();
     } 
     else
     {
        current = current->getRight();
     }
   }

   
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parentNode);
   

    // Checks if the first of the new item is less than the key of the parent node, and sets to newnode 
    if (new_item.first < parentNode->getKey()) 
    {
      parentNode->setLeft(newNode);
    } 
    else 
    {
      parentNode->setRight(newNode);
    }

    // Checks if the parent node balance is equal to -1, andf sets the balance 0.
    if (parentNode->getBalance() == -1) 
    {
      parentNode->setBalance(0);
      return;
    }    
    else if (parentNode->getBalance() == 1) 
    {
      parentNode->setBalance(0);
      return;
    }
    else 
    {
      // Parent's node balance was 0
      if (parentNode->getLeft() == newNode)
      {
        parentNode->setBalance(-1);
      } 
      else 
      {
        parentNode->setBalance(1);
      }
    
    
      fixInsert(parentNode, newNode);
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
    // Set a variable holding the ndoe to be removed 
    AVLNode<Key, Value>* nodeRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    // Base Case if the node to remove is null, just return 
    if (nodeRemove == nullptr)
    {
        return; 
    }
    
    // Checks if the node to remove has left and right children, and does a predecessor swap
    if (nodeRemove->getLeft() != nullptr && nodeRemove->getRight() != nullptr)
     {
        AVLNode<Key, Value>* predNode = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeRemove));
        nodeSwap(nodeRemove, predNode);
    }
    
    
    // Declaration of the parent node and child node , as node to remove of the parent, and a null pointer
    AVLNode<Key, Value>* parentNode = nodeRemove->getParent();
    AVLNode<Key, Value>* child = nullptr;
    
    // Checks if the node to remove left exists,a nd sets the child to this
    if (nodeRemove->getLeft() != nullptr) 
    {
        child = nodeRemove->getLeft();
    } 
    // Same done for right side
    else if (nodeRemove->getRight() != nullptr)
    {
        child = nodeRemove->getRight();
    }

    int8_t diff = 0;
    
    // Checks if parent node not null and does difference setting for balance factors and setting the parent node left or right of child
    if (parentNode != nullptr)
    {
        if (nodeRemove == parentNode->getLeft()) 
        {
            diff = 1;  
            parentNode->setLeft(child);
        } 
        else 
        {
            diff = -1; 
            parentNode->setRight(child);
        }
        
        if (child != nullptr) 
        {
            child->setParent(parentNode);
        }
    } 
    else 
    {
      
         
        this->root_ = child;
        // Checks if the child exists and sets the parent to null
        if (child != nullptr)
         {
            child->setParent(nullptr);
        }
    }
    
    
    delete nodeRemove;
    
    // Checks if parent node exists and calls instance of fixRemove which is a helper function i made
    if (parentNode != nullptr) 
    {
        fixRemove(parentNode, diff);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
       // Sets a variable equal to the right child
        AVLNode<Key, Value>* rightC = node->getRight();
        
        // Sets the parent of the right child to node of the parent
        rightC->setParent(node->getParent());
        
        // Checks if the node's parent equal to null, and sets the root to the right child
        if (node->getParent() == nullptr) 
        {
            
            this->root_ = rightC;
        }
        else 
        {
            // Checks if the node is equal to the parent of the left
            if (node == node->getParent()->getLeft()) 
            {
                node->getParent()->setLeft(rightC);
            } 
            else 
            {
                node->getParent()->setRight(rightC);
            }
        }
        
        
        node->setRight(rightC->getLeft());
        // Checks if the right child , left child exists and set parent of the node 
        if (rightC->getLeft() != nullptr) 
        {
            rightC->getLeft()->setParent(node);
        }
        
        // Set the right child's left instance of the node 
        rightC->setLeft(node);
        // Sets the node of the parent of the right child
        node->setParent(rightC);
}
    

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
       // Set the left child 
        AVLNode<Key, Value>* leftC = node->getLeft();
        
        // Sets the left child's parent to the node of parent
        leftC->setParent(node->getParent());
        
        // Checks if the parent of the node is null, setting root to left child
        if (node->getParent() == nullptr) 
        {
            
            this->root_ = leftC;
        } 
        else 
        {
            // Checks if the node equals the node of the parent of the left tree
            if (node == node->getParent()->getLeft()) 
            {
                node->getParent()->setLeft(leftC);
            } 
            else
             {
                node->getParent()->setRight(leftC);
            }
        }
        

        // sets the node of setleft of the left child of right 
        node->setLeft(leftC->getRight());
        // Checks if the left child's right is not null, and sets the left child right to the parent of the node
        if (leftC->getRight() != nullptr)
        {
            leftC->getRight()->setParent(node);
        }
        
        
        leftC->setRight(node);
        node->setParent(leftC);
}

template<class Key, class Value>
void AVLTree<Key, Value>::fixInsert(AVLNode<Key, Value>* parentNode, AVLNode<Key, Value>* child)
{
        // Checks if the parent node is null and the balance is 0
        
        if (parentNode == nullptr || parentNode->getBalance() == 0)
        {
            return;
        }
        
       
         // Set a varaible equal to grand parent of parent node 
         AVLNode<Key, Value>* grand = parentNode->getParent();

        // Base case checks if grand equals null, and returns
        if(grand == NULL)
        {
          return;
        }
    
      // Checks if the left child of the grand equals parent node 
      if (grand->getLeft() == parentNode) 
      {
          
          // Update balance factor
          grand->updateBalance(-1);
        
          // Checks if the grand's balance factor equals 0
          if (grand->getBalance() == 0)
          {    
             return;
          }
          // Checks if the grand balance is equal to -1, and calls fixInsert on the garnd parent and teh parent node
          else if (grand->getBalance() == -1)
          {
            fixInsert(grand, parentNode);
          } 
          // Checks if the grand parent's balance factor equals -2, does operations to rotate and reset balance factors accordingly
          else if (grand->getBalance() == -2)
          {
            
            if (parentNode->getLeft() == child)
             {
               
                rotateRight(grand);
                parentNode->setBalance(0);
                grand->setBalance(0);
            } 
            else
             {
                // Sets a variable to hold the child balance and set it to the child
                AVLNode<Key, Value>* childBalance = child;
                // Do rotations to reorganize tree
                rotateLeft(parentNode);
                rotateRight(grand);
                
                 // Checks if the child balance is equal to -1
                if (childBalance->getBalance() == -1) 
                {
                    // Sets the balanvce of parent node and garnd parent node
                    parentNode->setBalance(0);
                    grand->setBalance(1);
                } 
                // Checks if the the balance of the child equals 0, and sets balanvce of parent node and grand parent node
                else if (childBalance->getBalance() == 0)
                {
                    parentNode->setBalance(0);
                    grand->setBalance(0);
                } 
                 // Next case doing the same
                else
                { 
                    parentNode->setBalance(-1);
                    grand->setBalance(0);
                }

                childBalance->setBalance(0);
              }
            }
       } 
       else
       {
        
        grand->updateBalance(1);
        
        // Checks if the grand parent balance is equal to 0, and returns
        if (grand->getBalance() == 0) 
        {
            return;
        } 
        else if (grand->getBalance() == 1)
         {
            
            fixInsert(grand, parentNode);
        } 
        else if (grand->getBalance() == 2)
        {
            //  Checks if the grandparent is unbalanced and  performs rotations accordingly
            if (parentNode->getRight() == child) 
            {
               // This is the right-right case 
                rotateLeft(grand);
                parentNode->setBalance(0);
                grand->setBalance(0);
            } 
            else 
            {
                // This is the right-left case
                AVLNode<Key, Value>* childBalance = child;
                rotateRight(parentNode);
                rotateLeft(grand);
                
                // Update balance factors based on child's balance
                if (childBalance->getBalance() == 1)
                 {
                    parentNode->setBalance(0);
                    grand->setBalance(-1);
                } 
                else if (childBalance->getBalance() == 0) 
                {
                    parentNode->setBalance(0);
                    grand->setBalance(0);
                } 
                else 
                { 
                    parentNode->setBalance(1);
                    grand->setBalance(0);
                }
                childBalance->setBalance(0);
            }
        }
      }
}

template<class Key, class Value>
void AVLTree<Key, Value>::fixRemove(AVLNode<Key, Value>* node, int8_t diff)
{

    // Checks if the node is equal to null, and returns     
    if (node == nullptr) 
    {
        return;
    }

    // Sets the new balance to balance of current node +1     
    int8_t newBalance = node->getBalance() + diff;
    
    // Sets the parent to the node's parent
    AVLNode<Key, Value>* parent = node->getParent();
    int8_t nextDiff = 0;
    // Checks if the parent exists and the left child is equal to the node 
    if (parent != nullptr) 
    {
         // sets difference to 1, else otehr difference to 1, for balance factors
        if (parent->getLeft() == node) 
        {
            nextDiff = 1;  
        } 
        else 
        {
            nextDiff = -1; 
        }
    }
    
     // New balance being -2, indicating rotations must occur
    if (newBalance == -2)
     {
         // Sets a left child variable equal to the left child of the node 
        AVLNode<Key, Value>* leftChild = node->getLeft();
        
        // Checks the balance of the left child and does rotation, and resets balances of the ndoe and left child
        if (leftChild->getBalance() <= 0) 
        {
            
            rotateRight(node);
            
            if (leftChild->getBalance() == 0) 
            {
                
                node->setBalance(-1);
                leftChild->setBalance(1);
                return; 
            } 
            else
            {
                
                node->setBalance(0);
                leftChild->setBalance(0);
                
                // Checks if the parent exists and call fixRemove recursively 
                if (parent)
                { 
                  fixRemove(parent, nextDiff);
                }
            }
        } 
        else 
        {
            // Sets variables equal to the right grandchild and sets a balance variable to store this variable's balance 
            AVLNode<Key, Value>* rightGrandchild = leftChild->getRight();
            int8_t rBalance = rightGrandchild->getBalance();
            
            rotateLeft(leftChild);
            rotateRight(node);
            
            // Checks if this balnce value  equals 1, and sets the node's balance to 0, and sets the left child balance to -1
            if (rBalance == 1) 
            {
                node->setBalance(0);
                leftChild->setBalance(-1);
            } 
            // Then checks if the balance is 0, and sets the node's balance to 0, 
            else if (rBalance == 0) 
            {
                node->setBalance(0);
                leftChild->setBalance(0);
            } 
            else 
            {
                node->setBalance(1);
                leftChild->setBalance(0);
            }
            rightGrandchild->setBalance(0);
            
            // Recursively calls fixRemove once the parent exists
            if (parent)
            {  
              fixRemove(parent, nextDiff);
            }
        }
      }
      // Balnce factor equaling 2, indicates need for rotations and reorganization
      else if (newBalance == 2)
      {
        AVLNode<Key, Value>* rightChild = node->getRight();
        
        // Checks then right child's balance and determines the rotation
        if (rightChild->getBalance() >= 0) 
        {
            // This is the right-right case
            rotateLeft(node);
             
            
            if (rightChild->getBalance() == 0) 
            {
                
                node->setBalance(1);
                rightChild->setBalance(-1);
                return; 
            } 
            else 
            {
                // Normal case
                node->setBalance(0);
                rightChild->setBalance(0);
                // Recursivley fixes the AVL tree moving upwards
                if (parent)
                {
                   fixRemove(parent, nextDiff);
                }
            }
          } 
          else 
          {
            // Right-Left case
            AVLNode<Key, Value>* leftGrandchild = rightChild->getLeft();
            int8_t lBalance = leftGrandchild->getBalance();
            
            rotateRight(rightChild);
            rotateLeft(node);
            
            // Update balance factors
            if (lBalance == -1)
            {
                node->setBalance(0);
                rightChild->setBalance(1);
            } 
            else if (lBalance == 0)
            {
                node->setBalance(0);
                rightChild->setBalance(0);
            } 
            else 
            { 
                node->setBalance(-1);
                rightChild->setBalance(0);
            }
            leftGrandchild->setBalance(0);
            
            // Checks if the parent exists and calls fixRemove recursively 
            if (parent)
            { 
              fixRemove(parent, nextDiff);
            }
          }
       }
    // Balanced cases
    else 
    {
        // Update the balance
        node->setBalance(newBalance);
        
        if (diff != 0) 
        {
            // If balance is now 0, the height decreased, move upward
            if (newBalance == 0 && parent)
             {
                fixRemove(parent, nextDiff);
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
