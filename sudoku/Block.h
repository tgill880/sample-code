/**
 * Block.h
 * Thurman Gillespy
 * 11/2/17
 * 
 * class Block
 * A helper class for class Puzzle.
 * A 'block' is a 3x3 square subregion of a Sudoko puzzle.
 * This class determines if the Block region is 'valid' per 
 * Soduko rules.
 */

#pragma once

class Square;// forward declaration

class Block
{
   static const int BLOCK_DIM = 3;
   static const int PUZZLE_DIM = 9;
   static const int NUM_DIGITS = 10;

public:
   /**
    * single parameter constructor
    * Set position value.
    */
   Block(Square& square) : position(square) {}

    /**
     * isValid
     * Determine if the 3x3 Block region is valid per Soduko rules.
     * There can be no more than 1 instance of the digits [1...9].
     * Multiple 0s are allowed (represent empty squares).
     * @return true: no more than one digit 1 - 9;
     *   false: more than one digit 1 - 9.
     */
   bool isValid() const { return isValid(this->position); }


private:
  // Reference to the Square in the Puzzle board that is
  // the upper left corner of the Block.
  Square &position;

  /**
     * isValid
     * Determine if the 3x3 Block region is valid per Soduko rules.
     * There can be no more than 1 instance of the numbers [1...9] in block.
     * Multiple 0s are allowed (represent empty squares).
     * @param square - reference to upper left corner Square
     * @return true: no duplicates in Block; 
     *   false: duplicates in Block
     */
  static bool isValid(Square &square);
};