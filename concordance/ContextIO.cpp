/**
 * ContextIO.cpp
 * Definitions for class ContextIO.
 * 
 * Thurman Gillespy
 * 12/5/17
 */

#include <iostream>
#include "ContextIO.h"

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
bool ContextIO::init(const std::string fileName)
{
   this->fin.open(fileName);
   if (fin.fail())
   {
      std::cerr << "The file <" << fileName;
      std::cerr << "> could not be opened, or is not present ";
      std::cerr << "in the specified directory." << std::endl;
      return false;
   }

   // load the first CONTEXT_SIZE + 1 words in to the buffer
   buff_start = 0;
   for (int i = 0; i < CONTEXT_SIZE + 1; i++)
   {
      // if no more words, insert an empty string
      std::string word = "";
      fin >> word; // on failure/eof, word remains an empty string
      buffer[buff_start] = word;
      buff_start = (buff_start + 1) % BUFFER_SIZE;
   }

   update();
   
   // invalid input if the key word is empty string
   return (buffer[ (buff_start + KEYWORD_OFFSET) % BUFFER_SIZE] != "");
}

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
bool ContextIO::loadWord()
{
   std::string word = "";

   // insert next word at buff_start, then increment
   // if no more words, insert an empty string
   fin >> word; // on failure/eof, word remains an empty string
   buffer[buff_start] = word;
   buff_start = (buff_start + 1) % BUFFER_SIZE;

   // done when the key word is empty string
   if (buffer[ (buff_start + KEYWORD_OFFSET) % BUFFER_SIZE ] == "")
   {
      this->fin.close();
      return false;
   }

   update();
   
   return true;
}

/**
 * update
 * Update the keyWord, and before and after context strings
 * from the buffer.
 */
void ContextIO::update()
{
   keyWord = "", before = "", after = "";

   keyWord = buffer[(buff_start + KEYWORD_OFFSET) % BUFFER_SIZE];
   // preload the key word into the after context string
   after += keyWord + " ";
   // create context stirngs
   for (int i = 0; i < CONTEXT_SIZE; i++)
   {
      before += buffer[ (i + buff_start) % BUFFER_SIZE ] + " ";
      after += 
         buffer[ (i + buff_start + KEYWORD_OFFSET + 1) % BUFFER_SIZE ] + " ";
   }

   // keep track of the longest left context string (for printing)
   int length = before.length();
   if (this->leftWidth < length)
      this->leftWidth = length;
}