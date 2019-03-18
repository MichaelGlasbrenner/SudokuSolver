#ifndef _SUDOKU_FIELD_
#define _SUDOKU_FIELD_

#include <string>

class SudokuField
{
    private:
        int _values[9][9];
        bool _values_excluded[9][9][9];

    public:
        SudokuField();
        void print() const;
        void read_from_file(const std::string& filename);
};

#endif
