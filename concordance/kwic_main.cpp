/**
 * kwic_main.cpp
 * Thurman Gillespy
 * 12/7/17
 * 
 * Concordance generator using key word in context (KWIC) format.
 * 
 * This program reads a text file, the corpus, and generates a sorted 
 * concordance. The text file name is passed as a single parameter on
 * the command line. Each key word in the concordance is printed with the
 * five words before and after the key word, if present. A file named
 * "stopwords.txt" is used to create a list of words to exclude from the
 * concordance. If the stop words file is absent, the concordance is
 * created with all the words in the corpus text.
 * 
 * Concordance output is printed to stdout. Output is formatted such that
 * the keywords are aligned on their left margin.
 * 
 * Usage: kwic <corpus_file.txt>
 *   assumes: <stopwords.txt> in same directory as program.
 */

#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include "kwic_main.h"
#include "BST.h"
#include "ContextIO.h"

using namespace std;

int main(int argc, char *argv[])
{
   ContextIO buffer; // handles concordance file input and processing
   BST<string> stopw; // BST for stop words
   BST<CData> kwic; // BST for concordance words and context strings
   int leftWidth = 0; // for printing format

   string fileName = handleArgs(argc, argv);
   // if error opening file, an empty string returned
   if (fileName == "")
      return 1;
   
   loadStopWords(stopw);

   // initialize the context buffer
   // exit if failure
   if (!buffer.init(fileName))
      return 1;
   
   // loop through the buffer until the last word
   // of the corpus text is processed
   do {
      string keyWordRaw = buffer.getKeyWord();
      string before = buffer.getBefore();
      string after = buffer.getAfter();
      string keyWord = stripPunctuation(keyWordRaw, true);

      // keep track of longest before context string
      int length = before.length();
      if (leftWidth < length)
         leftWidth = length;
      
      // add keyWord and context to BST if key word not a stop word,
      // not an empty string and is not a number
      if ((stopw.find(keyWord) == nullptr) && !isANum(keyWordRaw)
                                          && keyWord != "")
      {
         DataIn din(keyWord, before, after, leftWidth);
         CData cdata(din);

         CData* cdPtr = kwic.find(cdata);
         // insert if keyWord not in BST
         if (cdPtr == nullptr) // not found
         {
            kwic.insert(cdata);
         }
         else
         // update CData since keyWord already in BST
         {
            cdPtr->update(din);
         }
      }
      // load new word (or empty string) into the buffer
      // quit if last word processed
   } while (buffer.loadWord());

   kwic.print(buffer.getWidth());
   
   return 0;
}

/**
 * handleArgs
 * Process the command line arguements.
 * If number of args != 2, print error message.
 * @return file name (the command line argument)
 */
string handleArgs(int argc, char *argv[])
{
   // check command arguments
   if (argc != 2)
   {
      // remove ./ in front of command line file name
      string progName = argv[0];
      progName = stripPunctuation(progName, false);

      cout << "Usage: " << progName << " <corpus_file.txt>" << endl;
      return "";
   }
   return argv[1];
}

/**
 * loadStopWords
 * Load the stop words in stopwords.txt into a BST<string>.
 * If file not found, leave the BST empty.
 */
void loadStopWords(BST<string> &stopw)
{
   string word;
   ifstream fin;
   // load the stop words, if present
   fin.open("stopwords.txt");
   if (!fin.fail())
   {
      while (!fin.eof())
      {
         fin >> word;
         if (stopw.find(word) == nullptr)
            stopw.insert(word);
      }
      fin.close();
   }
}

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
string stripPunctuation(string sin, bool toLower)
{
   int len = sin.length();
   int first = 0;
   int last = 0;
   int i = 0;

   // get the position of the first alphanumeric character
   i = 0;
   while (i < len && !isalnum(sin[i]))
      i++;

   first = i;

   // get the position of the last alphanumeric character
   i = len - 1;
   while (i >= 0 && !isalnum(sin[i]))
      i--;
   last = i;

   // stip all punctuation at front or back of sin
   // convert to lower case if toLower is true
   string buf = "";
   for (i = first; i <= last && i < len; i++)
   {
      if (toLower)
         buf += tolower(sin[i]);
      else 
         buf += sin[i];
   }

   return buf;
}

/**
 * isANum
 * Determine if a word is a number
 * @return true if word contains digits but no alphabetic characters.
 */
bool isANum(string str)
{
   regex rgxa("[0-9]");
   regex rgxb("[a-zA-z]");
   smatch m;

   return regex_search(str, m, rgxa) && !regex_search(str, m, rgxb);
}