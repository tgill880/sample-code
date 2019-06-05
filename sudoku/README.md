## Sudoku puzzle solver
Class assignment to create a Sudoku puzzle solver.

The program uses a recursive backtracking alogithm to solve a partially filled Sudoku matrix.

make: `g++ -std=c++11 -Werror -Wall Block.cpp Puzzle.cpp Square.cpp Sudoku.cpp -o sudoku`

usage: `./sudoku < puzzle.txt`

where `puzzle.txt` is a file with a string of 91 digits, each representing the initial value of a Sudoku square (1 - 9), or 
0 if empty.

The files `T1.txt`, `T2.txt` and `T3.txt` are 3 puzzles in the repository.

The initial and solved puzzles are printed to stdout.