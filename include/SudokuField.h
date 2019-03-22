#ifndef _SUDOKU_FIELD_
#define _SUDOKU_FIELD_

#include <string>

class SudokuField
{
    private:
        int _values[9][9];
        bool _values_excluded[9][9][10];

    public:
        SudokuField();
        void print_values() const;
        void print_values_excluded() const;
        void read_from_file(const std::string& filename);
        void solve();

    private:
        void exclude_values();
        void exclude_in_row(int which_row, int which_col, int which_value);
        void exclude_in_col(int which_row, int which_col, int which_value);
        void exclude_in_block(int which_row, int which_col, int which_value);

        void set_new_values();

        enum block
        {
            top_left, top_middle, top_right,
            middle_left, middle_middle, middle_right,
            bottom_left, bottom_middle, bottom_right
        };

        block get_block(int which_row, int which_col); 
        void get_block_borders(block the_block, int& start_col, int& start_row, int& end_col, int& end_row);
};

#endif
