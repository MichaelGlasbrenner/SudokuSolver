#include "SudokuField.h"


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


void SudokuField::print() const
{

}


void SudokuField::read_from_file(const std::string& filename)
{

}
