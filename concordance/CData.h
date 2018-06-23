/**
 * CData.h
 * Declarations for class CData.
 * 
 * Thurman Gillespy
 * 12/7/17
 */

#pragma once

#include <ostream>
#include <string>
#include "CList.h"
#include "DataIn.h"

/**
 * class CData
 * Class that holds the data and methods for a concordance BST.
 */
class CData
{
 public:
   // length of (eventually) longest before context string
   // updated each time CData is created or updated
   static int width;
   
   /**
    * constructor
    * Create a CData object using a DataIn data structure.
    * Update CData::width if length of before context string is larger.
    */
   CData(DataIn& data);
   
   /**
    * copy constructor
    * Creae a deep copy.
    * @param src linked list nodes copied from src into new object
    */
   CData(const CData& src);
   
   /**
    * getKeyWord
    * Get the keyWord data element.
    * @return string that contains the keyWord
    */
   const std::string getKeyWord() const { return this->keyWord; }

   /**
    * update
    * Update the linked list with data for a new node that contains the before
    * and after context strings.
    * Update CData::width if length of before context string is larger.
    * Used when the keyWord already exists in the BST.
    * @param din struct containing the before and after context strings for the
    * new linked list node.
    */
   void update(const DataIn &din);
   
   /**
    * print
    * Print the keyWord and context stings in this object.
    * Utility method for testing CData objects.
    * Loop through all of the linked list context nodes.
    * Use reset() and adavanceCounter() methods of linked list to control
    * a pointer to the node to be printed.
    * Use CData::width for formatting the left context.
    */
   void print();

   /**
    * operator >
    * Implement the greater than comparison operator.
    * Compare the keyWord strings in both objects.
    * @param cdata rhs of comparison.
    * @return true if lhs < rhs, otherwise false.
    */
   bool operator>(const CData& cdata) const { 
                                    return this->keyWord > cdata.keyWord; }

   /**
    * operator <
    * Implement the less than comparison operator.
    * Compare the keyWord strings in both objects.
    * @param cdata rhs of comparison.
    * @return true if lhs > rhs, otherwise false.
    */
   bool operator<(const CData &cdata) const { 
                                    return this->keyWord < cdata.keyWord; }

   /**
    * operator ==
    * Implements the equality comparison operator.
    * Compare the keyWord strings in both objects.
    * @param cdata rhs of comparison.
    * @return true if lhs == rhs, otherwise false.
    */
   bool operator==(const CData &cdata) const {
                                    return this->keyWord == cdata.keyWord; }

 private:
   std::string keyWord = ""; // key word for concordance
   // linked list containing before and after context strings
   CList context;
   
   
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
   friend std::ostream& operator<<(std::ostream& os, CData& data);
};