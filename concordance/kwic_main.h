/**
 * kwic.h
 * Function declarations for kwic.cpp (main).
 * 
 * Thurman Gillespy
 * 12/7/17
 */

#pragma once

#include <string>
#include "BST.h"

/**
 * stripPunctuation
 * Remove punctuation before and after a word.
 * Keep internal punctuation, alphanumberic chars.
 * Convert to lower case if requested.
 * Returns empty string if no alphanumeric chars.
 * @param sin string to convert
 * @param toLower convert to lower case if true, do not if false
 * @return the converted string.
 */
std::string stripPunctuation(std::string sin, bool toLower);

/**
 * isANum
 * Determine if a word is a number.
 * @return true if word contains digits but no alphabetic characters.
 */
bool isANum(std::string str);

/**
 * handleArgs
 * Process the command line arguements.
 * If number of args != 2, print error message.
 * @return file name (the command line argument)
 */
std::string handleArgs(int argc, char *argv[]);

/**
 * loadStopWords
 * Load the stop words in stopwords.txt into a BST<string>.
 * If file not found, leave the BST empty.
 */
void loadStopWords(BST<std::string> &killw);