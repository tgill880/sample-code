/**
 * BST.h
 * Declarations for templated class BST<>
 * 
 * Thurman Gillespy
 * 12/7/17
 */

#pragma once

#include "BNode.h"

/**
 * class BST
 * Class template for implmenting an unbalanced binary search tree for
 * templated nodes of type N.
 * Duplicates not allowed (throws exception).
 * Use find() first to ensure item in node not already in the tree.
 */
template <class N>
class BST
{
public:
   /**
    * destructor
    * Delete all the nodes in the tree.
    * Calls: clear()
    * @pre 0.. n nodes in BST
    * @post 0 nodes in tree, memory for nodes deallocated.
    */
   ~BST(){ clear(this->root); }

   /**
    * find
    * Find the node containing item of type N in the tree.
    * Public interface.
    * @param item item of type N to find
    * @return pointer to the item of type N in the node.
    *   If not found, returns nullptr.
    */
   N* find(const N& item) const { return find(this->root, item); }

   /**
    * insert
    * Insert item of type N into the tree.
    * Creates a new BNode of type N.
    * Item must not already exist in the tree (exception thrown).
    * @param item of type N to insert.
    * return true if item was inserted, otherwise false.
    */
   bool insert(const N& item) { return insert(&this->root, item); }

   /**
    * print()
    * Print the nodes of the tree.
    * Calls: inOrder
    */
   void print() const { inOrder(this->root, 0); }

   /** print(int)
    * Print the nodes of the tree.
    * Calls: inOrder
    * @param width used to align right text margin. Used in
    *   cout << setw(width) << right << 
    * output sequence.
    */
   void print(int width) const { inOrder(this->root, width); }

private:
   BNode<N>* root = nullptr; // root node of tree

   /**
    * find(BNode<N>* curr, N& item)
    * Find the node containing item of type N in the tree, beginning 
    * at node curr.
    * Private interface.
    * Recursive method.
    * @param curr node to start search
    * @param item data element of type N in the node to search for.
    * @return pointer to the item of type N in the node.
    *   If not found, returns nullptr.
    */
   N* find(BNode<N>* curr, const N& item) const;

   /**
    * insert
    * Insert item of type N into the tree beginning at node curr.
    * Creates a new BNode of type N.
    * Item must not already exist in the tree (exception thrown).
    * @param curr node to begin search at.
    * @param item of type N to insert.
    * return true if item was inserted, otherwise false.
    */
   bool insert(BNode<N>** curr, const N& item);

   /**
    * inOrder
    * Do an inorder traverse, and print each node.
    * @param curr node to begin search at.
    * @param width used for output formatting
    */
   void inOrder(BNode<N>* curr, int width) const;

   /**
    * clear
    * Delete the nodes in the tree using a post-order traversal.
    * @params curr node to begin traversal
    */
   void clear(const BNode<N>* curr);

};

#include "BST.cpp"