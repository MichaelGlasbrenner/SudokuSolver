#include "SudokuField.h"

void print_start_message();


int main()
{
    print_start_message();
 
    SudokuField the_field;

    the_field.read_from_file("input_field"); // FIXME

    the_field.print();

}


void print_start_message()
{
    printf("\n\n**********************************************\n\n");
    printf("       Welcome to SudokuSolver \n");
    printf("       Version: 1.0 \n");
    printf("       Author: Michael Glasbrenner \n");
    printf("\n\n**********************************************\n\n");
}
