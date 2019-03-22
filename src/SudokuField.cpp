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
            for(int k=1; k <= 9; ++k)
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
    for(int k=1; k <= 9; ++k)
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

            ss  >> _values[k][0] >> _values[k][1] >> _values[k][2] 
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
    for(int iter=0; iter<100; ++iter)
    {
        this->exclude_values();

        this->set_new_values();
    }
}


void SudokuField::exclude_values()
{
    for(int row=0; row<9; ++row)
    {
        for(int col=0; col<9; ++col)
        {
            int current_value = _values[row][col];

            if(current_value != 0)
            {
                this->exclude_in_row(row, col, current_value);
                this->exclude_in_col(row, col, current_value);
                this->exclude_in_block(row, col,current_value);
            }
        }
    }

}


void SudokuField::set_new_values()
{
    for(int row=0; row<9; ++row)
    {
        for(int col=0; col<9; ++col)
        {
            if(_values[row][col] != 0) continue; // field has already been set

            int possible_value_count = 0;
            int value_for_setting = -1;

            for(int k=1; k <= 9; ++k)
            {
                if(! _values_excluded[row][col][k])
                {
                   value_for_setting = k;
                   possible_value_count++;
                }
            }

            if(possible_value_count == 0)
            {
               printf("impossible situation\n");
            }
            else if(possible_value_count == 1)
            {
               printf("setting field[%d,%d] to value %d \n", row, col, value_for_setting);
               _values[row][col] = value_for_setting;
            }
        }
    }

}


void SudokuField::exclude_in_row(int which_row, int which_col, int which_value)
{
    for(int col=0; col<9; ++col)
    {
        if(col == which_col) continue;

        _values_excluded[which_row][col][which_value] = true;
    }
}


void SudokuField::exclude_in_col(int which_row, int which_col, int which_value)
{
    for(int row=0; row<9; ++row)
    {
        if(row == which_row) continue;

        _values_excluded[row][which_col][which_value] = true;
    }
}


void SudokuField::exclude_in_block(int which_row, int which_col, int which_value)
{
    block the_block = this->get_block(which_row, which_col);

    int start_col=-1;
    int start_row=-1;
    int end_col=-1;
    int end_row=-1;

    this->get_block_borders(the_block, start_col, start_row, end_col, end_row);

    for(int col=start_col; col<=end_col; ++col)
    {
        for(int row=start_row; row<=end_row; ++row)
        {
            if(row == which_row && col == which_col) continue;

            _values_excluded[row][col][which_value] = true;
        }
    }
}


SudokuField::block SudokuField::get_block(int which_row, int which_col)
{
    int row_block = which_row / 3; // division without remainder -> maps [0,1,2] to 0, [3,4,5] to 1 ...
    int col_block = which_col / 3;

    if(row_block == 0 && col_block == 0) return top_left;
    if(row_block == 0 && col_block == 1) return top_middle;
    if(row_block == 0 && col_block == 2) return top_right;
    if(row_block == 1 && col_block == 0) return middle_left;
    if(row_block == 1 && col_block == 1) return middle_middle;
    if(row_block == 1 && col_block == 2) return middle_right;
    if(row_block == 2 && col_block == 0) return bottom_left;
    if(row_block == 2 && col_block == 1) return bottom_middle;
    if(row_block == 2 && col_block == 2) return bottom_right;
}


void SudokuField::get_block_borders(block the_block, int& start_col, int& start_row, int& end_col, int& end_row)
{
    switch(the_block)
    {
       case top_left:
           start_col = start_row = 0;
           break;
       case top_middle:
           start_row = 0;
           start_col = 3;
           break;
       case top_right:
           start_row = 0;
           start_col = 6;
           break;
       case middle_left:
           start_row = 3;
           start_col = 0;
           break;
       case middle_middle:
           start_col = start_row = 3;
           break;
       case middle_right:
           start_row = 3;
           start_col = 6;
           break;
       case bottom_left:
           start_row = 6;
           start_col = 0;
           break;
       case bottom_middle:
           start_row = 6;
           start_col = 3;
           break;
       case bottom_right:
           start_col = start_row = 6;
           break;

    }

    end_row = start_row + 2;
    end_col = start_col + 2;

}
