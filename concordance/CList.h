/**
 * CList.h
 * Declarations for CList class that contains the context strings
 * for concordance data.
 * 
 * Thurman Gillespy
 * 12/7/17
 */

#pragma once

#include <string>
#include <cassert>
#include <iostream>

/**
 * Node for linked list that contains the before and after 
 * context strings.
 */
struct LNode
{
   std::string before;
   std::string after;
   LNode *next;
   LNode(std::string bef, std::string aft) : 
                           before(bef), after(aft), next(nullptr) {}
};

/**
 * class CList
 * Linked list for concordance data.
 * Each node contains the before and after context strings.
 */
class CList
{
 public:
   /**
    * destructor
    * Delete all the nodes in the linked list.
    * Calls: clear()
    * @pre list contains 1..n nodes
    * @post list contains 0 nodes
    */
   ~CList() { clear(); }

   /**
    * getHeadPtr
    * @return head, pointer to first node
    */
   LNode* getHeadPtr() const { return this->head; }

   /**
    * setHeadPtr
    * Set the head node pointer.
    * @param ptr pointer to the new node
    */
   void setHeadPtr( LNode* ptr) { this->head = ptr; }

   /**
    * setTailPtr
    * Set the tail node pointer.
    * @param ptr pointer to the new node
    */
   void setTailPtr( LNode* ptr) { this->tail = ptr; }

   /**
    * reset
    * Set the curListPtr to head.
    */
   void reset() { currListPtr = head; };

   /**
    * advanceCounter
    * Advance the curListPtr to the next node.
    * Return a boolean indicating if there is another node in the list.
    * @return true if curListPtr is != nullptr, otherwise false
    */
   bool advanceCounter();

   /**
    * getBefore
    * Get the before context string referenced by currListPtr.
    * @return before context string
    */
   const std::string getBefore() const { return currListPtr->before; };

   /**
    * getAfter
    * Get the after context string referenced by currListPtr.
    * @return after context string
    */
   const std::string getAfter() const { return currListPtr->after; }

   /**
    * addTail
    * Add a new node to the end of the linked list.
    * @param before before context sting
    * @param after after context string
    */
   void addTail(std::string before, std::string after);

   /**
    * clear
    * Delete all the nodes in the linked list.
    */
   void clear();

   /**
    * print
    * Print all the nodes in the linked list.
    * Utility method for testing CList class
    */
   void print() const;

 private:
   LNode *head = nullptr; // pointer to first node
   LNode *tail = nullptr; // pointer to the last node
   LNode *currListPtr = head; // pointer to the current node to be printed
};
