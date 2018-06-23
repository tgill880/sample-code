/**
 * BST.cpp
 * Definitions for templated class BST.
 * 
 * Thurman Gillespy
 * 12/7/17
 */

#include <iomanip>
#include "BST.h"

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
template <class N>
N* BST<N>::find(BNode<N>* curr, const N& din) const
{
   if (curr == nullptr)
      return nullptr;

   N *currItem = curr->getItem();

   if (*currItem == din)
      return curr->getItem();

   if (*currItem > din)
   {
      return find(curr->getLeftNodePtr(), din);
   }
   else
   {
      return find(curr->getRightNodePtr(), din);
   }
}

/**
 * insert
 * Insert item of type N into the tree beginning at node curr.
 * Creates a new BNode of type N.
 * Item must not already exist in the tree (exception thrown).
 * @param curr node to begin search at.
 * @param item of type N to insert.
 * return true if item was inserted, otherwise false.
 */
template <class N>
bool BST<N>::insert(BNode<N>** curr, const N &din)
{

   if (*curr == nullptr)
   {
      *curr = new BNode<N>(din);
      return true;
   }

   N* currItem = (*curr)->getItem();

   if (*currItem > din)
   {
      if ((*curr)->getLeftNodePtr() == nullptr)
      {
         BNode<N>* tempNPtr = new BNode<N>(din);
         (*curr)->setLeftNodePtr(tempNPtr);
         return true;
      }
      else
      {
         BNode<N>* tempNPtr = (*curr)->getLeftNodePtr();
         return insert(&tempNPtr, din);
      }
   }

   if (*currItem < din)
   {
      if ((*curr)->getRightNodePtr() == nullptr)
      {
         BNode<N>* tempNPtr = new BNode<N>(din);
         (*curr)->setRightNodePtr(tempNPtr);
         return true;
      }
      else
      {
         BNode<N>* tempNPtr = (*curr)->getRightNodePtr();
         return insert(&tempNPtr, din);
      }
   }

   // if equal, throw exception since duplicates not permitted
   throw "Duplicate nodes not permitted in BST.";

   return false;
}

/**
 * inOrder
 * Do an inorder traversal, and print each node.
 * @param curr node to begin search at.
 * @param width used for output formatting
 */
template <class N>
void BST<N>::inOrder(BNode<N> *curr, int leftWidth) const
{
   if (curr != nullptr)
   {
      inOrder(curr->getLeftNodePtr(), leftWidth);

      std::cout << std::setw(leftWidth) << std::right;
      std::cout << *(curr->getItem()) << std::endl;

      inOrder(curr->getRightNodePtr(), leftWidth);
   }
}

/**
 * clear
 * Delete the nodes in the tree using a post-order traversal.
 * @params curr node to begin traverse
 */
template <class N>
void BST<N>::clear(const BNode<N> *curr)
{
   if (curr == nullptr)
      return;
   
   clear(curr->getLeftNodePtr());
   clear(curr->getRightNodePtr());
   delete curr;
   curr = nullptr;
} 