/**
 * Block.cpp
 * Thurman Gillespy
 * 11/2/17
 * 
 * class Block
 * A helper class for class Puzzle.
 * A 'block' is a 3x3 square subregion of a Sudoko puzzle.
 * This class determines if the Block region is 'valid' per 
 * Soduko rules.
 */

#include <cassert>
#include "Block.h"
#include "Square.h"

/**
 * isValid
 * Determine if the 3x3 Block region is valid per Soduko rules.
 * There can be no more than 1 instance of the digits [1...9].
 * Multiple 0s are allowed (represent empty squares).
 * @return true: no more than one digit 1 - 9;
 *   false: more than one digit 1 - 9.
 */
bool Block::isValid(Square& pos)
{
   Square* initPos = &pos;
   Square* curPos = initPos;
   // array for determining how many of each digit
   int nums[NUM_DIGITS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

   for (int i = 0; i < BLOCK_DIM; i++)
   {
      for (int j = 0; j < BLOCK_DIM; j++)
      {
         int curValue = curPos->getValue();
         assert(curValue >= 0 && curValue <= 9);
         nums[curValue]++;
         curPos++;
      }
      // set initPos to next row, and reset curPos
      initPos += PUZZLE_DIM;
      curPos = initPos;
   }
   int numDups = 0;
   // find the largest number in nums[], ignoring nums[0]
   for (int i = 1; i < NUM_DIGITS; i++)
   {
      if (numDups < nums[i])
         numDups = nums[i];
   }
   
   // if the numDups < 2, the block is valid and return true
   // else return false
   return (numDups < 2) ? true : false;
}