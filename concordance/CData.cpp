/**
 * CData.cpp
 * Definitions for class CData.
 * 
 * Thurman Gillespy
 * 12/5/17
 */

#include <iostream>
#include <iomanip>
#include "CData.h"
#include "DataIn.h"
#include "BNode.h"

/**
 * initialize the static variable
 */
int CData::width = 0;

/**
 * constructor
 * Create a CData object using a DataIn data structure.
 * Update CData::width if length of before context string is larger.
 */
CData::CData(DataIn& din)
{
   this->keyWord = din.keyWord;
   this->context.addTail(din.before, din.after);
   if (CData::width < din.width)
      CData::width = din.width;
}

/**
 * copy constructor
 * Creae a deep copy.
 * @param src linked list nodes copied from src into new object
 */
CData::CData(const CData& src)
{
   this->keyWord = src.keyWord;
   this->context.setHeadPtr(nullptr);
   this->context.setTailPtr(nullptr);

   // deep copy of linked list nodes
   LNode* curr = src.context.getHeadPtr();
   while (curr != nullptr)
   {
      this->context.addTail(curr->before, curr->after);
      curr = curr->next;
   }
}

/**
 * update
 * Update the linked list with data for a new node that contains the before
 * and after context strings.
 * Update CData::width if length of before context string is larger.
 * Used when the keyWord already exists in the BST.
 * @param din struct containing the before and after context strings for the
 * new linked list node.
 */
void CData::update(const DataIn& din)
{
   context.addTail(din.before, din.after);
   if (CData::width < din.width)
      CData::width = din.width;
}

/**
 * print
 * Print the keyWord and context stings in this object.
 * Utility mathod for testing CData objects.
 * Loop through all of the linked list context nodes.
 * Use reset() and adavanceCounter() methods of linked list to control
 * a pointer to the linked list node to be printed.
 * Use CData::width for formatting the left context.
 */
void CData::print()
{
   this->context.reset();
   do
   {
      std::cout << std::setw(CData::width) << std::right;
      std::cout << this->context.getBefore() << this->context.getAfter();
      std::cout << std::endl;

   } while (this->context.advanceCounter());
   this->context.reset();

}

/**
 * operator <<
 * Overload the outstream operator for this class.
 * Loop through all the nodes in the linked list.
 * Use reset() and advanceCounter() methods of linked list to
 * advance a pointer to the linked list node to print.
 * Use CData::width to format the before context string.
 * @param os the outstream (ostream) object.
 * @param data CData object that contains the output data
 * @return reference to ostream
 */
std::ostream& operator<<(std::ostream &os, CData &data)
{
   bool multiple = false;
   int leftWidth = CData::width;
   
   data.context.reset();
   do
   {
      if (multiple)
      {
         os << std::endl;
         os << std::setw(leftWidth) << std::right;
      }
      os << data.context.getBefore() << data.context.getAfter();
      multiple = true;

   } while (data.context.advanceCounter());
   data.context.reset();

   return os;
}