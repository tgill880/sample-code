/**
 * Puzzle.h
 * Thurman Gillespy
 * 11/5/17
 * 
 * This class represents a Sudoku puzzle board.
 * The class contains a board array of Square objects, with
 * operations to add a value, erase a value, and check the validity of
 * a proposed digit in an empty square.
 * The puzzle is solved with a recursive backtracking algorithm.
 */
#pragma once

#include "Square.h"
#include "Block.h"
#include <iostream>

class Puzzle
{
   const static int PUZZLE_SIZE = 81;
   const static int PUZZLE_DIM = 9;
   // upper left corner offsets for each block
   enum {BK_1 = 0,  BK_2 = 3,  BK_3 = 6,
         BK_4 = 27, BK_5 = 30, BK_6 = 33,
         BK_7 = 54, BK_8 = 57, BK_9 = 60};

public:
   /**
    * default constructor
    */
   Puzzle() {}

   /**
   * size 
   * Get the number of Squares that are not fixed, ie variable.
   * @return int Number of Squares that are not fixed.
   */
   int size() const { return this->numVariable; }


   /** 
    * getNumEmpty
    * Get the number of Squares in the Puzzle board that are empty
    * (value == 0).
    */
   int getNumEmpty() const { return this->numEmpty; }

   /**
    * incrementNumEmpty
    * Increase numEmpty by one.
    */
   void incrementNumEmpty() { this->numEmpty++; }

   /**
    * decrementNumEmpty
    * Decrease numEmpty by one.
    */
   void decrementNumEmpty() { this->numEmpty--; }

   /**
    * incrementNumVariable
    * increase numVariable by one.
    */
   void incrmentNumVariable() { this->numVariable++; }

   /**
    * setValidInput
    * Set the validInput property.
    * @param valid Boolean - true: valid input; false: invalid
    */
   void setValidInput(bool valid) { this->validInput = valid; }

   /**
    * getValidInput
    * Get the validInput property. If valid, correct number of digits
    * was entered into the Puzzle.
    * @return the validInput property
    */
   bool getValidInput() { return this->validInput; }
   
   /**
    * getPuzzleBoard
    * Get a reference to the first Square in the Puzzle board
    * @return Reference to first Square in Puzzle board
    */
   Square& getPuzzleBoard() { return board[0]; }

   /**
    * get
    * Get a reference to the Square at location pos in the Puzzle board.
    * @param pos Position of the square on the Puzzle board
    * @return Reference to a Square object
    */
   Square& get(int pos){ return board[pos]; }

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
   bool set(int pos, int value);

   /**
    * solve
    * Solve the Puzzle.
    * Recursive method: solve Puzzle beginning at pos.
    * @param pos The index postion of the Puzzle board to start.
    * @return true: Puzzle was solved; false: could not be solved
    */
   bool solve(int pos);

 private:
   // Puzzle board, a 9x9 matrix of Squares
   // maintained internally as a single dimension array
   Square board[PUZZLE_SIZE];
   // the 9 blocks associated with the Puzzle board
   Block blocks[9] = 
            {Block(board[BK_1]), Block(board[BK_2]), Block(board[BK_3]),
             Block(board[BK_4]), Block(board[BK_5]), Block(board[BK_6]),
             Block(board[BK_7]), Block(board[BK_8]), Block(board[BK_9])};
   
   int numVariable = 0; // number of non-fixed squares in Puzzle board
   int numEmpty = 0; // number of empty squares
   bool validInput = false; // was correct number of digits entered?

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
   bool isValidRow(int pos, int value);

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
   bool isValidCol(int pos, int value);

   /**
    * blocksAreValid
    * Check of all the Blocks associated with the Puzzle board are valide.
    * Proposed new value has already been set.
    * @return true: all blocks are valid; false: at least one is not valid.
    */
   bool blocksAreValid();

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
   int getNextEmptySquarePos(int pos);

   /**
    * setValue
    * Set the value of the Square at pos.
    * No checking of value 'validity'. Used by >> operator to directly write
    * values in the Puzzle board.
    * numVariable and numEmpty are not updated.
    * @param pos The index position of the Square in the Puzzle board.
    * @param value The value to insert.
    * @pre The Square value at pos is 0 (set by default constructor).
    * @post The Square value at pos is set to value (0 - 9).
    */
   void setValue(int pos, int value) { board[pos].setValue(value); }

    /**
    * erase
    * Erase (set to 0) the Square at position pos.
    * Increment numEmpty.
    * @param pos The index position of the Square in the Puzzle board.
    * @pre The value at pos is a digit 0 - 9.
    * @post The value at pos is set to 0.
   */
   void erase(int pos) { board[pos].setValue(0); this->numEmpty++; }

   /**
    * operator <<
    * Overloaded output stream operator.
    * Print the Puzzle board to the output stream, regardless of state.
    * @param os Reference to ostream.
    * @param p Reference to Puzzle instance.
    * @output Reference to ostream after operation.
    */
   friend std::ostream& operator<<(std::ostream& os, Puzzle& p);

   /**
    * operator >>
    * Overloaded input stream operator.
    * Accepts character input.
    * Reads first 81 digits, then attempts to solve puzzle.
    * If < 81 digits, do not alter puzzle, print error message.
    * If > 81 digits, only read the first 81.
    * @param is Reference to the input stream (istream&).
    * @param p Reference to the Puzzle instance.
    * @return Reference to istream after reading characters.
    */
   friend std::istream& operator>>(std::istream& is, Puzzle& p);
};