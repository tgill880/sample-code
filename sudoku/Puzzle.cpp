/**
 * Puzzle.cpp
 * Thurman Gillespy
 * 11/14/17
 * 
 * This class represents a Sudoku puzzle board.
 * The class contains a board array of Square objects, with
 * operations to add a value, erase a value, and check the validity of
 * a proposed digit in an empty square.
 * The puzzle is solved with a recursive backtracking algorithm.
 */

#include "Puzzle.h"
#include <cassert>

using namespace std;

/**
 * solve
 * Solve the Puzzle.
 * Recursive method: solve Puzzle beginning at pos.
 * @param pos The index postion of the Puzzle board to start.
 * @return true: Puzzle was solved; false: could not be solved
 */
bool Puzzle::solve(int pos)
{
   // finished when no more empty squares
   if (getNumEmpty() == 0) return true;

   int position = getNextEmptySquarePos(pos);
   for (int value = 1; value <= 9; value++)
   {
      if ( set(position, value) )
      {
         if (solve(position + 1)) return true;
         
         erase(position);
      }
   } // end for
   return false;
}

/**
 * set
 * Set the Square at location pos to value if valid.
 * Check that the new value is valid for the column
 * and row of the position, and all the blocks.
 * The method sets the value of the square, then checks for validity.
 * If not valid, then the square is reset to 0.
 * Calls:
 *   isValidRow(pos, value)
 *   isValidCol(pos, value)
 *   blocksAreValid()
 *   erase(pos)
 * @param pos Location of the Square in board[]
 * @param value Proposed value to add to Square
 * @return true: new value is valid, and was added; false: new value is
 * not valid and was not added.
 */
bool Puzzle::set(int pos, int value)
{
   assert(value >= 1 && value <= 9);
   assert(board[pos].getIsFixed() == false);
   // set the Square at pos to value, then check if valid
   board[pos].setValue(value);
   decrementNumEmpty();
   if (isValidRow(pos, value) && isValidCol(pos, value) &&
                                                blocksAreValid())
   {
      return true;
   }
   // not valid, so erase
   erase(pos); // increments numEmpty

   return false;
}

/**
 * getNextEmptySquarePos
 * Get the next 'empty' Square beginning at pos.
 * May include the current Square at pos. If there are no more empty
 * squares, return PUZZLE_SIZE.
 * @param pos The position to begin search. Offset from the
 * beginning of the Puzzle board.
 * @return The position of the next empty Square, or PUZZLE_SIZE if
 * none remaining.
 */
int Puzzle::getNextEmptySquarePos(int pos)
{
   int value = 0;
   // get the next empty Square, unless past end of board
   while (pos < PUZZLE_SIZE && (value = board[pos].getValue()) != 0)
   {
      pos++;
   }
   return pos;
}

/**
 * isValidRow
 * Determine if the new value would be valid for the row associated
 * with this position in the Puzzle board.
 * row = pos / 9 (ie, PUZZLE_DIM)
 * @param pos Location of the current Square in the Puzzle board to test
 * @param value The proposed new value to add to the current Square
 * @return true: adding this value would result in a valid Row;
 *   false: would not be valid.
 */
bool Puzzle::isValidRow(int pos, int value)
{
   // get the row associated with this position
   int row = pos / PUZZLE_DIM;
   // search for duplicate values > 1
   int numDups = 0;
   // pointer to the first Square of the row
   Square *curPos = &board[row * PUZZLE_DIM];
   for (int i = 0; i < PUZZLE_DIM && numDups < 2; i++)
   {
      int curValue = curPos->getValue();
      if (curValue == value && curValue != 0)
      {
         numDups++;
      }
      curPos++;
   }

   // if the numDups < 2, the row is valid and return true
   // else return false
   return (numDups < 2) ? true : false;
}

/**
 * isValidCol
 * Determine if the new value would be valid for the column associated
 * with this position in the Puzzle board.
 * column = pos % 9 (ie, PUZZLE_DIM)
 * @param pos Location of the current Square in the Puzzle board to test
 * @param value The proposed new value to add to the current Square
 * @return true: adding this value would result in a valid column;
 *   false: would not be valid.
 */
bool Puzzle::isValidCol(int pos, int value)
{
   // get the column associated with this position
   int col = pos % PUZZLE_DIM;
   // search for duplicate value
   int numDups = 0; // number of duplicates found
   // pointer to the first Square of the column
   Square* curPos = &board[col];
   for (int i = 0; i < PUZZLE_DIM && numDups < 2; i++)
   {
      int curValue = curPos->getValue();
      // count the number of duplicate values, ignoring 0
      if (curValue == value && curValue != 0)
      {
         numDups++;
      }
      curPos += PUZZLE_DIM; // next row
   }
   
   // if the numDups < 2, the column is valid and return true
   // else return false
   return (numDups < 2) ? true : false;
}

/**
 * blocksAreValid
 * Check of all the Blocks associated with the Puzzle board are valide.
 * Proposed new value has already been set.
 * @return true: all blocks are valid; false: at least one is not valid.
 */
bool Puzzle::blocksAreValid()
{
   bool valid = true;

   // check each Block in blocks
   for (int i = 0; i < PUZZLE_DIM && valid; i++)
   {
      // set valid to false if any block is invalid
      if (!blocks[i].isValid())
      {
         valid = false;
      }
   }
   
   return valid;
}

/**
 * operator <<
 * Overloaded output stream operator.
 * Print the Puzzle board to the output stream, regardless of state.
 * @param os Reference to ostream.
 * @param p Reference to Puzzle instance.
 * @output Reference to ostream after operation.
 */
ostream& operator<<(ostream& os, Puzzle& p)
{
   Square& firstSquare = p.getPuzzleBoard();
   Square* initPos = &firstSquare;
   Square* curPos = initPos;
   const string BORDER_BAR = "+-------+-------+-------+";
   const string MID_BAR = "|-------+-------+-------|";

   // top row
   cout << BORDER_BAR << endl;
   for (int i = 0; i < Puzzle::PUZZLE_DIM; i++)
   {
      cout << "| ";
      for (int j = 0; j < Puzzle::PUZZLE_DIM; j++)
      {
         int value = curPos->getValue();
         if (value == 0)
            cout << "  ";
         else
            cout << value << " ";
         if ( (j + 1) % 3 == 0) cout << "| ";
         curPos++;
      }
      cout << endl;
      if ( (i + 1) % 3 == 0 && i < Puzzle::PUZZLE_DIM - 1)
         cout << MID_BAR << endl;
      // next row
      initPos += Puzzle::PUZZLE_DIM;
      curPos = initPos;
   }
   cout << BORDER_BAR << endl << endl;
   
   return os;
}

/**
 * operator >>
 * Overloaded input stream operator.
 * Accepts character input.
 * Reads first 81 digits, then attempts to solve puzzle.
 * Ignore any chars that are not digits.
 * If < 81 digits, do not alter puzzle, print error message.
 * If > 81 digits, only read the first 81.
 * @param is Reference to the input stream (istream&).
 * @param p Reference to the Puzzle instance.
 * @return Reference to istream after reading characters.
 */
istream& operator>>(istream& is, Puzzle& p)
{
   string str = "";
   bool success = true;
   char ch = '\0';
   int count = 0;

   // read until Puzzle:PUZZLE_SIZE chars read or EOF
   while (is.get(ch) && count < Puzzle::PUZZLE_SIZE)
   {
      if (ch >= '0' && ch <= '9')
      {
         str += ch;
         count++;
      }
   }

   // if string length < PUZZLE_SIZE (81), an invalid input
   if (str.length() < Puzzle::PUZZLE_SIZE) success = false;

   // load digits into Puzzle if success
   if (success)
   {
      // correct input data
      assert(str.length() == Puzzle::PUZZLE_SIZE);
      p.setValidInput(true);
      for (int i = 0; i < Puzzle::PUZZLE_SIZE; i++)
      {
         // put the digit into the Square
         // subtract '0' to convert to an int
         Square& square = p.get(i);
         char digit = str.at(i);
         p.setValue(i, digit - '0');
         
         if (digit == '0')
         {
            // an empty square
            square.setIsFixed(false);
            p.incrementNumEmpty();
            p.incrmentNumVariable();
         }
         else
         {
            // square with initial value
            square.setIsFixed(true);
         }
            
      } // end for
   } // end if
   else
   {
      // incorrect input data
      p.setValidInput(false);
      cout << "invalid input to Sudoku puzzle" << endl;
   }

   return is;
}
