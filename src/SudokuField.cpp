#include "SudokuField.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


SudokuField::SudokuField()
{
    for(int row=0; row<9; ++row)
    {
        for(int col=0; col<9; ++col)
        {
            _values[row][col] = 0;
        }
    }

    for(int row=0; row<9; ++row)
    {
        for(int col=0; col<9; ++col)
        {
            for(int k=0; k < 9; ++k)
            {
                _values_excluded[row][col][k] = false;
            }
        }
    }
}


void SudokuField::print_values() const
{
    printf("the Sudoku field : \n\n");

    for(int row=0; row<9; ++row)
    {
        printf("  ");
        for(int col=0; col<9; ++col)
        {
            printf("%d ", _values[row][col]);
        }
        printf("\n");
    }
    printf("\n\n");
}


void SudokuField::print_values_excluded() const
{
    printf("the excluded values :\n\n");
    for(int k=0; k < 9; ++k)
    {
        printf("\n\nk = %d \n\n", k);
        for(int row=0; row<9; ++row)
        {
            printf("  ");
            for(int col=0; col<9; ++col)
            {
                printf("%s ", _values_excluded[row][col][k] ? "t" : "f");
            }
            printf("\n");
        }
    }
}


// FIXME : do some format checking for the input file
void SudokuField::read_from_file(const std::string& filename)
{
    std::string line;
    std::ifstream myfile (filename);
  if (myfile.is_open())
  {
    int k=0;
    while ( std::getline (myfile,line) )
    {
        std::stringstream ss(line);

        ss   >> _values[k][0] >> _values[k][1] >> _values[k][2] 
             >> _values[k][3] >> _values[k][4] >> _values[k][5] 
             >> _values[k][6] >> _values[k][7] >> _values[k][8] ;
        ++k;
    }


    myfile.close();
  }
  else std::cout << "Unable to open file";

}


void SudokuField::solve()
{
   this->exclude_values();
}


void SudokuField::exclude_values()
{
    for(int row=0; row<9; ++row)
    {
        for(int col=0; col<9; ++col)
        {
            int current_value = _values[row][col];

            //this->exclude_in_row(row, current_value);
            //this->exclude_in_col(row, current_value);
            //this->exclude_in_block(row, col,current_value);
        }
    }

}
