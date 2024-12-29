#ifndef PUZZLE_H
#define PUZZLE_H

#include "ppm_io.h" // for Image data type

typedef struct {
  // ...fields go here...
  int size; //number of Rows = number of Columns
  int gapNumber; //tile value (NOT INDEX POSITION) in Image that is missing (gap)
  int * tiles;//pointer to array to store int numbers to represent puzzle configuration
} Puzzle;

//creates a pointer to an instance of the Puzzle struct with specified size
Puzzle *puzzle_create(int size);

//checks to see if the parameter points to an instance of the Puzzle struct and if it does, it frees it
void puzzle_destroy(Puzzle *p);

//called when C is pressed. Passes in a filepointer to read input from a double pointer to a Puzzle instance
int handle_C_command(FILE *in, Puzzle **p);

//called when T is pressed. Passes in a filepointer to read input from and a pointer to a puzzle instance
int handle_T_command(FILE* in, Puzzle *p);

//called when I is pressed. Passes in a filepointer to read input from and a double pointer to an instance of an Image struct
int handle_I_command(FILE* in, Image** img);

//checks to see if image pointer in the argument  points to an instance of the Image struct and if it does, it frees it
void image_destroy(Image *img);

//called when P is pressed. Parameter is a pointer to a puzzle instance.
int handle_P_command(Puzzle *p);

//called when S is pressed. Parameters are a filepointer to read input from and a pointer to a puzzle instance.
int handle_S_command(FILE* in, Puzzle *p);

//performs the down operation on the puzzle pointed to by the parameter
void slide_down(Puzzle *p);

//performs the up operation on the puzzle pointed to by the parameter
void slide_up(Puzzle *p);

//performs the right operation on the puzzle pointed to by the parameter
void slide_right(Puzzle *p);

//performs the left operation on the puzzle pointed to by the parameter
void slide_left(Puzzle *p);

//checks to see if the down operation can be performed on the puzzle p
int check_down(Puzzle *p);

//checks to see if the up operation can be performed on the puzzle p
int check_up(Puzzle *p);

//checks to see if the right operation can be performed on the puzzle p
int check_right(Puzzle *p);

//checks to see if the left operation can be performed on the puzzle p
int check_left(Puzzle *p);

//finds the tile in Puzzle p with the specified value val and returns the tiles index position in the array. If the specified tile value doesn't exist, return -1.
int find_tile(Puzzle *p, int val);

//returns the tile value of the tile in Puzzle p at the specified index position in the puzzle. If the specified index doesn't exist, returns -1. 
int puzzle_get_tile(const Puzzle *p, int index);

//sets a tile of specified value in the specified index position of Puzzle p
void puzzle_set_tile(Puzzle *p, int index, int value);

//called when W is pressed. Parameters are a filepointer to read from, a pointer to an instance of a puzzle struct and a pointer to an instance of an image struct
int handle_W_command(FILE* in, Puzzle *p, Image* img);

//called when K is pressed. Argument is a pointer to a puzzle struct.
int handle_K_command(Puzzle *p);

//called to check if puzzle p is in solved configuration, returns 1 if solved and 0 if not solved
int check_puzzle(Puzzle *p);

//called when V is pressed. p is a pointer to an instance of a puzzle struct
int handle_V_command(Puzzle *p);

//function to recurisvely find a solution to solve the scrambled puzzle p, if it exists. steps is an array to save the sequence of steps to a valid solution. max_steps is the maximum number of steps to try, cur_steps is the number of steps already tried, and prev saves the previous move tried
int solve_puzzle(Puzzle *p, char steps[], int max_steps, int cur_steps, char prev);

//function to create a pointer to a copy of the Puzzle array
Puzzle * copy_puzzle(Puzzle* p);

#endif // PUZZLE_H
