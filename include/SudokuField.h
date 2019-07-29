#ifndef _SUDOKU_FIELD_
#define _SUDOKU_FIELD_

#include <string>
#include <vector>

class SudokuField
{
    private:
        int _values[9][9]; // all values that have already been found
        bool _values_excluded[9][9][10]; // for each of the 9x9 fields we have an boolean array which describes
                                         // if the corresponding number can be excluded for this field

    public:
        SudokuField();
        SudokuField(const SudokuField& other);
        void print_values() const;
        void print_values_excluded() const;
        void read_from_file(const std::string& filename);
        bool solve();

    private:
        bool solve_by_exclusion_only();
        bool solve_with_guessing(int n_fields_to_guess);

        void copy_values(const SudokuField& other);
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
        int get_number_of_solved_fields();
        std::vector<std::vector<int>> generate_guesses(int n_fields_to_guess);
        void apply_guess(int n_fields_to_guess);

        bool check_consistency();
};

#endif
