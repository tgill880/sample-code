/**
 * ContextIO.h
 * Declarations for class ContextIO.
 * 
 * Thurman Gillespy
 * 12/5/17
 */

#pragma once

#include <string>
#include <fstream>


/**
 * class ContextIO
 * This class handles reading the words from a concordance corpus text file, 
 * placing them into a buffer, and creating before and after context strings
 * for a BST concordance tree.
 * The buffer uses a "circular buffer" algorithm to insert words, and extract
 * the key word, before context and after context strings.
 */
class ContextIO
{
   static const int CONTEXT_SIZE = 5; // # of words in context strings
   // buffer size = (2 * # of context words) + 1
   static const int BUFFER_SIZE = (2 * CONTEXT_SIZE) + 1;
   static const int KEYWORD_OFFSET = CONTEXT_SIZE;
   
public:
   /**
    * getKeyWord
    * Get the key word, which is updated by update().
    * @returns string containing the keyWord
    */
   std::string getKeyWord()const { return this->keyWord; }

   /**
    * getBefore
    * Get the before context string, which is updated by update().
    * @returns before context string
    */
   std::string getBefore() const { return this->before; }

   /**
    * getAfter
    * Get the after context string, which is updated by update().
    * @returns after context string
    */
   std::string getAfter() const { return this->after; }

   /**
    * getWidth
    * Get the width of the largest before context string.
    * @returns leftWidth
    */
   int getWidth() const { return this->leftWidth; }

   /**
    * init
    * Initialize the context buffer.
    * Load the first CONTEXT_SIZE + 1 words from the corpus text file into the
    * buffer. Insert empty strings fewer words in file.
    * Print error message to cerr if corpus file could not be opened.
    * @param fileName file name of input file
    * @returns true if the input file could be opened and the key word 
    *   is not an empty string, otherwise false.
    */
   bool init(const std::string fileName);

   /**
    * loadWord
    * Load the next word into the buffer.
    * If EOF, then load an empty string.
    * Quit (return false) when enough empty strings have been inserted
    * that the key word is an empty string. Then the last word in the
    * file/buffer has been processed.
    * Calls: update, which updates key word and context strings
    * @param fin input file
    * @returns true if the mid word in the buffer is not empty string,
    *   false if key word is empty string
    */
   bool loadWord();

 private:
   // buffer[]: array of strings containing conecutive words in corpus text
   // key word and both context strings extracted from here
   std::string buffer[BUFFER_SIZE];
   std::string keyWord = ""; // key word of concordance
   std::string before = ""; // before context string
   std::string after = ""; // after context string
   std::ifstream fin;
   int leftWidth = 0; // used to format before context string
   // index to "start" of buffer using circular buffer algorithm
   int buff_start = 0; 

   /**
    * update
    * Update the keyWord, and before and after context strings
    * from the buffer.
    */
   void update();
};