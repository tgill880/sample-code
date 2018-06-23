/**
 * CList.cpp
 * Definitions for class CList.
 * 
 * Thurman Gillespy
 * 12/7/17
 */

#include <iostream>
#include "CList.h"

/**
 * addTail
 * Add a new node to the end of the linked list.
 * @param before before context sting
 * @param after after context string
 */
void CList::addTail(std::string before, std::string after)
{
   LNode *newNode = new LNode(before, after);
   if (this->head == nullptr)
   {
      this->head = newNode;
      this->tail = newNode;
      newNode->next = nullptr;
   }
   else
   {
      assert(this->tail->next == nullptr);
      this->tail->next = newNode;
      this->tail = newNode;
      newNode->next = nullptr;
   }
}

/**
 * advanceCounter
 * Advance the curListPtr to the next node.
 * Return a boolean indicating if there is another node in the list.
 * @return true if curListPtr is != nullptr, otherwise false
 */
bool CList::advanceCounter()
{
   assert(currListPtr != nullptr);
   currListPtr = currListPtr->next;
   return currListPtr != nullptr;
}

/**
 * clear
 * Delete all the nodes in the linked list.
 */
void CList::clear()
{
   LNode *curr = this->head;
   LNode *next = nullptr;
   while (curr != nullptr)
   {
      next = curr->next;
      assert(curr != nullptr);
      delete curr;
      curr = nullptr;
      curr = next;
   }
}

/**
 * print
 * Print all the nodes in the linked list.
 * Utility method for testing CList class.
 */
void CList::print() const
{
   LNode *curr = this->head;
   while (curr != nullptr)
   {
      std::cout << curr->before << " " << curr->after << std::endl;
      curr = curr->next;
   }
}
