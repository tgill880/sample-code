/**
 * Square.h
 * Thurman Gillespy
 * 11/8/17
 * 
 * This class represents an individual square in a Sudoku puzzle board.
 * The Square has a value (digits 0 - 9), and a boolean indicating
 * whether the Square is fixed (has an initial value which cannot be changed),
 * or not.
 */

#pragma once

class Square
{
   friend class Puzzle; // Puzzle calls Square::setValue

 public:
   /**
    * default constructor
    * Set value to 0, fixedValue to false
    */
   Square() {value = 0, isFixed = false;}

   /**
    * single parameter constructor
    */
   Square(int value) { this->value = value; isFixed = true; }
   
   /**
    * getValue
    * get the value of the square [0...9]
    * @return int: the value of the square
    */
   int getValue() const { return this->value;}

   /**
    * getIsFixed
    * Is the value of square fixed, ie, can it be changed.
    * @return true: value is fixed; false: not fixed (can be changed)
    */
   bool getIsFixed() const {return this->isFixed;}

   /**
    * setIsFixed
    * Set the isFixed flag.
    * true: value cannot be changed; false: value can be changed.
    */
   void setIsFixed(bool fixed) { this->isFixed = fixed; }

 protected:
   /**
    * setValue
    * Set the value of the square. Must be [0...9].
    * 0 means the square is empty.
    * No checking for value 'validity'.
    * @param value - new value for square
    * @pre square has value [0...9]
    * @post square has value set to value
    */
   void setValue(int value) { this->value = value; }

private:
   // the value of the square [0...9], 0 = empty
   int value;
   // is the value fixed, ie, set at game beginning
   bool isFixed;
};