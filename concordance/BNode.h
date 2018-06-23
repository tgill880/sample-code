/**
 * BNode.h
 * Thurman Gillespy
 * 12/5/17
 * 
 * Header file for class BNode.
 */

#pragma once

#include <string>
#include "CData.h"

/**
 * class BNode
 * Class template for the individual nodes of a binary search tree of type N.
 */
template <class N>
class BNode
{
 private:
   N item; // data item of type N for node
   BNode *left = nullptr; // left child pointer
   BNode *right = nullptr; // right child pointer

 public:
   
   /**
    * Constructor.
    * Creates a new node with an item of type N.
    * @param item of type N
    */
   BNode(N newNode) : item(newNode) {}

   /**
    * getLeftNodePtr
    * Get the pointer to the left child node.
    * @returns pointer to BNode
    */
   BNode* getLeftNodePtr() const { return this->left;}

   /**
    * setLeftNodePtr
    * Assign a node to the left child node pointer.
    * @param leftPtr - pointer to new node
    * @pre left child node pointer is null
    * @post left child node pointer points to new BNode
    */
   void setLeftNodePtr(BNode* leftPtr) { this->left = leftPtr; }

   /**
    * getRightNodePtr
    * Get the pointer to the right child node.
    * @returns pointer to BNode
    */
   BNode* getRightNodePtr() const { return this->right; }

   /**
    * setRightNodePtr
    * Assign a node to the right child node pointer.
    * @param rightPtr - pointer to new node
    * @pre right child node pointer is null
    * @post right child node pointer points to new BNode
    */
   void setRightNodePtr(BNode *rightPtr) { this->right = rightPtr; }

   /**
    * getItem
    * Get a pointer to the item of type N for the node.
    * @return pointer to item of type N
    */
   N* getItem() { return &item; }

};

// all methods inline
//#include "BNode.cpp"