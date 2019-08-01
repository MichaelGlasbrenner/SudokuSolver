#include "SudokuField.h"
#include <string.h>
#include <stdio.h>

void print_start_message();


int main(int argc, char* argv[])
{
    print_start_message();
 
    SudokuField the_field;

    std::string input_file;
    
    if(argc > 1) 
    {    
        for(int i=0; i < argc; ++i) 
        {
            if(strcmp("-inp", argv[i]) == 0)
            {
                if(i+1 >= argc)
                {
                    printf("expected input file name after -inp");
                    exit(-1);
                }
                input_file = std::string(argv[i+1]);
            }    
        }    
    }    
    else 
    {    
        printf("too few input arguments \n");
        printf("call SudokuSolver as follows: \n");
        printf("./SudokuSolver -inp <input-file-name> \n");
        exit(-1);
    } 

    the_field.read_from_file(input_file); // FIXME

    the_field.print_values();

    bool solved_successfully = the_field.solve();

    if(solved_successfully)
    {
       the_field.print_values();
    }
    else
    {
        printf("\nThe solution could not be determined. \nTry rerunning the program and consider increasing the number of guesses. \n\n");
    }
}


void print_start_message()
{
    printf("\n\n**********************************************\n\n");
    printf("       Welcome to SudokuSolver \n");
    printf("       Version: 1.0 \n");
    printf("       Author: Michael Glasbrenner \n");
    printf("\n\n**********************************************\n\n");
}
