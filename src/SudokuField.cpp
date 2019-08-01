#include "SudokuField.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()


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
    
    // seed the random number generator
    srand(time(0));
}


SudokuField::SudokuField(const SudokuField& other)
{
    this->copy_values(other);    
}


void SudokuField::copy_values(const SudokuField& other)
{
    for(int row=0; row<9; ++row)
    {
        for(int col=0; col<9; ++col)
        {
            _values[row][col] = other._values[row][col];
        }
    }

    for(int row=0; row<9; ++row)
    {
        for(int col=0; col<9; ++col)
        {
            for(int k=1; k <= 9; ++k)
            {
                _values_excluded[row][col][k] = other._values_excluded[row][col][k];
            }
        }
    }
}


void SudokuField::print_values() const
{
    printf("\nthe Sudoku field : \n\n");

    for(int row=0; row<9; ++row)
    {
        printf("  ");
        for(int col=0; col<9; ++col)
        {
            printf("%d ", _values[row][col]);
            if(col==2 || col == 5) printf("  ");
        }
        if(row==2 || row == 5) printf("\n");
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
    else
    {
        std::cout << "Unable to open input file\n\n";
        exit(-1);
    }
}


bool SudokuField::solve()
{
    bool solved_by_exclusion = this->solve_by_exclusion_only(); 

    if(solved_by_exclusion) return true;

    int number_of_solved_fields =  this->get_number_of_solved_fields();

    for(int n_fields_to_guess=1; n_fields_to_guess < (81 - number_of_solved_fields); ++n_fields_to_guess)
    {
       printf("\ntrying to find solution by guessing %d fields \n", n_fields_to_guess);
       for(int trial=0; trial < 10000; ++trial) // FIXME
       {
           printf("trial %d \n", trial);
           bool found_solution = this->solve_with_guessing(n_fields_to_guess);
           if(found_solution == true) return true;
       }
    }

    return false;
}


bool SudokuField::solve_with_guessing(int n_fields_to_guess)
{
    SudokuField copied_field(*this);

    copied_field.apply_guess(n_fields_to_guess);

    if( ! copied_field.check_consistency())
    {
       return false;
    }

    bool found_solution = copied_field.solve_by_exclusion_only();

    if(found_solution == true)
    {
        printf("found solution after guessing\n");

        if( ! copied_field.check_consistency())
        {
            printf("inconsistent field after guessing \n");
        }

        this->copy_values(copied_field);
        return true;
    }

    return false;
}


void SudokuField::apply_guess(int n_fields_to_guess)
{
    struct field
    {
       int row;
       int col;
    };

    auto get_list_of_free_fields = [=] (std::vector<field>& free_fields)
    {
        free_fields.clear();

        for(int row=0; row<9; ++row)
        {
            for(int col=0; col<9; ++col)
            {
                int current_value = _values[row][col];

                field current_field;
                current_field.row = row;
                current_field.col = col;

                if(current_value == 0) free_fields.push_back(current_field);
            }
        }
    };

    auto get_random_number = [=] (int& random_number, int lower_limit, int upper_limit)
    {    
        random_number = ((rand() % upper_limit) + lower_limit);
    };

    int guesses_remaining = n_fields_to_guess;
    std::vector<field> all_free_fields; 
    get_list_of_free_fields(all_free_fields); 

    while(guesses_remaining > 0)
    {
        int remaining_free_fields = all_free_fields.size();
        
        int random_position;
        get_random_number(random_position, 0, remaining_free_fields-1);

        const field& random_field = all_free_fields.at(random_position);

        int random_val; 
        get_random_number(random_val, 1, 9);

        printf("setting [%d][%d] to value %d \n", random_field.row, random_field.col, random_val);
        _values[random_field.row][random_field.col] = random_val;

        all_free_fields.erase(all_free_fields.begin() + random_position);

        guesses_remaining--;
    }
}


bool SudokuField::solve_by_exclusion_only()
{
    int number_of_solved_fields =  this->get_number_of_solved_fields();
    int previous_number_of_solved_fields = number_of_solved_fields;

    for(int iter=0; iter<100; ++iter)
    {
        printf("starting iteration %d \n", iter);
        this->exclude_values();

        this->set_new_values();

        int number_of_solved_fields = this->get_number_of_solved_fields();

        if(number_of_solved_fields == 81) return true; // all fields found

        if(number_of_solved_fields == previous_number_of_solved_fields) // this means no progress in the last iteration
        {
           return false;
        }

        previous_number_of_solved_fields = number_of_solved_fields;
    }

    return false;
}


int SudokuField::get_number_of_solved_fields()
{
    int n_solved = 0;

    for(int row=0; row<9; ++row)
    {
        for(int col=0; col<9; ++col)
        {
            int current_value = _values[row][col];

            if(current_value > 0) n_solved++;
        }
    }
   
    return n_solved;
}


std::vector<std::vector<int>> SudokuField::generate_guesses(int n_fields_to_guess)
{
   std::vector<std::vector<int>> guesses;
   guesses.resize(n_fields_to_guess);

   /*auto find_free_field = [] ()
   {
       
   };

   int current_start_row = 0;
   int current_start_col = 0;

   while(remaining_fields_to_guess > 0)
   {
       for(int row=current_start_row; row<9; ++row)
       {
           for(int col=current_start_col; col<9; ++col)
           {

           }
       }

       for(int row=current_start_row+1; row<9; ++row)
       {
           for(int col=0; col<9; ++col)
           {

           }
       }
   }*/

   return guesses;
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
                
                this->exclude_in_row(row, col, value_for_setting);
                this->exclude_in_col(row, col, value_for_setting);
                this->exclude_in_block(row, col,value_for_setting);
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


bool SudokuField::check_consistency()
{
    bool field_is_consistent = true;

    // each number only once per column
    for(int row=0; row<9; ++row)
    {
        std::vector<int> occurencies;
        for(int i=0; i<10; ++i) occurencies.push_back(0);

        for(int col=0; col<9; ++col)
        {
            //printf("val[%d]: %d \n", col, _values[row][col] );
            int value = _values[row][col];
            occurencies[value] += 1;
        }
        
        for(int i=1; i<10; ++i) 
        {
            //printf("occurencies[%d] : %d \n", i, occurencies[i]);
            if(occurencies[i] > 1) return false;
        }
    }

    // each number only once per row
    for(int col=0; col<9; ++col)
    {
        std::vector<int> occurencies;
        for(int i=0; i<10; ++i) occurencies.push_back(0);

        for(int row=0; row<9; ++row)
        {
            int value = _values[row][col];
            occurencies[value] += 1;
        }

        for(int i=1; i<10; ++i) 
        {
            if(occurencies[i] > 1) return false;
        }
    }

    // each number only once per 3x3 block
    auto check_3by3_block = [&](int upper_left_row, int upper_left_col) -> bool
    {
        std::vector<int> occurencies;
        for(int i=0; i<10; ++i) occurencies.push_back(0);

        for(int col=upper_left_col; col<upper_left_col+3; ++col)
        {
            for(int row=upper_left_row; row<upper_left_row+3; ++row)
            {
                int value = _values[row][col];
                occurencies[value] += 1;
            }
        }

        for(int i=1; i<10; ++i) 
        {
            if(occurencies[i] > 1) return false;
        }

    };

    check_3by3_block(0,0);
    check_3by3_block(3,0);
    check_3by3_block(6,0);
    check_3by3_block(0,3);
    check_3by3_block(3,3);
    check_3by3_block(6,3);
    check_3by3_block(0,6);
    check_3by3_block(3,6);
    check_3by3_block(6,6);

    return field_is_consistent;
}
