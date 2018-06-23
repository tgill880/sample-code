/**
 * Sudoko.cpp
 * Thurman Gillespy
 * 11/7/17
 * 
 * Program to solve Sudoko puzzles using the Puzzle class.
 */

#include "Puzzle.h"
#include <iostream>

using namespace std;

int main(int argc, const char * argv[])
{
   Puzzle p;
   bool solved = false;

   // load the puzzle
   cin >> p;

   // exit if invalid input
   if (!p.getValidInput())
   {
      cout << endl;
      return 0;
   }

   // print the puzzle before solving
   cout << p;
   solved = p.solve(0);
   // exit if cannot be solved
   if (!solved)
   {
      cout << "the puzzle could not be solved" << endl << endl;
      return 0;
   }
   // print the solved puzzle
   cout << p;

   return 0;
}