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
        void print_values() const;
        void print_values_excluded() const;
        void read_from_file(const std::string& filename);
        void solve();

    private:
        void exclude_values();
};

#endif
