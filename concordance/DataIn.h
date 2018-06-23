/**
 * DataIn.h
 * Data structure for creating and updating a CData object.
 * 
 * Thurman Gillespy
 * 12/5/17
 */

#pragma once

#include <string>
#include <iostream>

/**
 * Data structure for creating or updating a CData object.
 * Contains concordance key word, before context string, after context
 * string, and an integer that is the length of the before context string.
 */
struct DataIn
{
   std::string keyWord;
   std::string before;
   std::string after;
   int width;
   DataIn(std::string key) : keyWord(key), before(""), after(""), width(0) {}
   DataIn(std::string key, std::string before, std::string after, int width) :
                  keyWord(key), before(before), after(after), width(width) {}
};