
This is a small program for solving Sudoku puzzles.

Easy Sudokus are solved by the excluding all forbidden values for 
the empty fields. 

For more difficult Sudokus, where this is not sufficient, 
either 1,2,3 or more fields are guessed at random, after which
an exclusion approach is applied. If this leads to a solved
field without any rule violations, the correct solution
has been found; otherwise, a new random guess is made. 

If the program fails to find a solution, it might find one if
it is started again (due to the random guesses).

------------------------------------------------------------------

compilation :

cmake .
make

------------------------------------------------------------------

invocation:
./Sudokusolver -inp name-of-inputfile

------------------------------------------------------------------

format of the inputfile:
the inputfile should contain 9 lines filled with the 9 numbers in 
each row.


  0 8 0  0 1 0  0 7 0
  0 0 0  6 0 9  0 0 0
  4 0 2  0 0 0  3 0 9
  0 0 3  0 0 0  5 0 0
  8 0 0  0 3 0  0 0 7
  0 0 4  0 0 0  2 0 0
  5 0 7  0 0 0  8 0 1
  0 0 0  5 0 6  0 0 0
  0 2 0  0 8 0  0 4 0
