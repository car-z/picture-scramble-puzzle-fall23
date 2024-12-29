#include "ppm_io.h"
#include "puzzle.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Puzzle *puzzle_create(int size) {
  Puzzle *newPuzzle = (Puzzle *)malloc(sizeof(Puzzle));
  if (newPuzzle == NULL) {
    fprintf(stderr, "Error allocating new Puzzle\n");
    return NULL;
  }
  newPuzzle->size = size;

  //initializing gapNumber. In T command, gapNumber will be set to the value that is misssing in the puzzle
  newPuzzle->gapNumber = -1;

  //"2D" array (actually 1D)
  int *array = malloc(sizeof(int) * size * size);
  if (array == NULL) {
    fprintf(stderr, "Error allocating array for Puzzle struct\n");
    return NULL;
  }
  for (int i = 0; i < size * size; i++) {
    *(array + i) = 0;
  }
  newPuzzle->tiles = array;

  //returns pointer to a dynamically allocated puzzle struct
  return newPuzzle;
}

void puzzle_destroy(Puzzle *p) {
  //if there is no puzzle pointed to by p
  if (p == NULL) {
    return;
  }
  //if there is a puzzle pointed to by p
  free(p->tiles);
  p->tiles = NULL;
  free(p);
  p = NULL;
}

int handle_C_command(FILE *in, Puzzle **p) {
  int size = -1;
  int parse = fscanf(in, "%d", &size);
  if (parse != 1) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  }
  if (size < 2 || size > 20) {
    fprintf(stderr, "Invalid puzzle size\n");
    return 1;
  }
  (*p) = puzzle_create(size);
  return 0;
}

int handle_T_command(FILE *in, Puzzle *p) {
  if (p == NULL) {
    fprintf(stderr, "No puzzle\n");
    return 1;
  }
  int numTiles = (p->size) * (p->size);

  //variable to act as counter for how many digits added to tiles array
  int count = 0;

  //variable to save input value in to
  int value = -1;

  //declaring and initializing array to check which numbers have been entered 
  int checkBlank[numTiles + 1];
  for (int i = 0; i < numTiles + 1; i++) {
    checkBlank[i] = 0;
  }

  //while there are still uninitialized puzzle values 
  while (count < numTiles) {
    if (fscanf(in, "%d", &value) != 1) {
      fprintf(stderr, "Invalid input\n");
      return 1;
    }
    if (value < 0 || value > numTiles) {
      fprintf(stderr, "Invalid tile value\n");
      return 1;
    }
    p->tiles[count] = value;
    checkBlank[value] += 1;
    count++;
  }
  // no need to check zero because we assume that zero must exist
  for (int i = 1; i < (numTiles + 1); i++) {
    //finding which index position (which corresponds to tile value) is unintiailized - this is the missing tile in the image
    if (checkBlank[i] == 0) {
      p->gapNumber = i;
      break;
    }
  }
  return 0;
}

int handle_I_command(FILE *in, Image **img) {
  char fileName[255];
  if (fscanf(in, "%s", fileName) != 1) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  }

  FILE *fp = fopen(fileName, "r");

  if (fp == NULL) {
    fprintf(stderr, "Could not open image file '%s'\n", fileName);
    return 1;
  }

  //create a pointer to an Image struct using ReadPPM
  Image *n = ReadPPM(fp);
  fclose(fp);
  //if ReadPPM failed, return with error
  if (n == NULL) {
    return 1;
  }
  (*img) = n;
  n = NULL;
  return 0;
}

void image_destroy(Image *img) {
  //if img pointer points to nothing, do nothing
  if (img == NULL) {
    return;
  }
  //if img pointer points to valid image struct, free it
  free(img->data);
  free(img);
  img = NULL;
}

int handle_P_command(Puzzle *p) {
  if (p == NULL) {
    fprintf(stderr, "No puzzle\n");
    return 1;
  }
  for (int i = 0; i < (p->size) * (p->size); i++){
    //printing new line at end of file to match test cases
    if(i == ((p->size)*(p->size) - 1)){
      printf("%d\n",p->tiles[i]);
    }
    else{printf("%d ", p->tiles[i]);}
  }
  return 0;
}

int handle_S_command(FILE *in, Puzzle *p) {
  if (p == NULL) {
    fprintf(stderr, "No puzzle\n");
    return 1;
  }

  //variable to save specified direction (input value) into
  char direction = 0;

  if (fscanf(in, " %c", &direction) != 1) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  }
  
  switch (direction) {

    // down direction
  case 'd':
    if(check_down(p) == 1){
      fprintf(stderr,"Puzzle cannot be moved in specified direction\n");
      return 1;
    }
    else{
      slide_down(p);
    }
      break;

    // up direction
  case 'u':
    if(check_up(p) == 1){
      fprintf(stderr,"Puzzle cannot be moved in specified direction\n");
      return 1;
    }
    else{
      slide_up(p);
    }
    break;

    // left direction
  case 'l':
    if(check_left(p) == 1){
      fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
      return 1;
    } else{
      slide_left(p);
    }
    break;

    // right direction
  case 'r':
    if(check_right(p) == 1){
      fprintf(stderr,"Puzzle cannot be moved in specified direction\n");
      return 1;
    } else{
      slide_right(p);
    }
    break;

    // incorrect argument command
  default:
    fprintf(stderr, "Invalid input\n");
    return 1;
  }    
  return 0;
}

//the check_* functions check if the directions we're moving works, if it doesn't work, it returns 1.
//if it does work, returns 0

int check_down(Puzzle *p){
  //finding index position of gap tile
  int indexGap = find_tile(p,0);
  int size = p->size;
  if(indexGap < size){
    return 1;
  } else{
    return 0;
  }
}

//the slide_* functions slide the gap tile according to the specified direction

void slide_down(Puzzle *p){
  //finding index position of gap tile
  int indexGap = find_tile(p,0);
  int size = p->size;
  //finding index position that gap tile needs to move to
  int move = indexGap - size;
  //getting value of tile which is at the index position which the gap tile needs to move to
  int tileValue = puzzle_get_tile(p,move);
  //setting the two tiles to be moved down in their new positions and assigning the appropriate values
  puzzle_set_tile(p,move,0);
  puzzle_set_tile(p,indexGap,tileValue);
}

int check_up(Puzzle * p){
  int indexGap = find_tile(p,0);
  int size = p->size;
  if(indexGap >= (size*size - size)){
    return 1;
  }
  else{
    return 0;
  }
}

void slide_up(Puzzle * p){
  int indexGap = find_tile(p,0);
  int size = p->size;
  int move = indexGap + size;
  int tileValue = puzzle_get_tile(p,move);
  puzzle_set_tile(p,move,0);
  puzzle_set_tile(p,indexGap,tileValue);
}

int check_left(Puzzle *p){
  int indexGap = find_tile(p,0);
  int size = p->size;
  if((indexGap+1) % size == 0){
    return 1;
  }
  else{
    return 0;
  }
}

void slide_left(Puzzle *p){
  int indexGap = find_tile(p,0);
  int move = indexGap + 1;
  int tileValue = puzzle_get_tile(p,move);
  puzzle_set_tile(p,move,0);
  puzzle_set_tile(p,indexGap,tileValue);
}

int check_right(Puzzle *p){
  int indexGap = find_tile(p,0);
  int size = p->size;
  if(indexGap % size == 0){
    return 1;
  }
  else{
    return 0;
  }
}

void slide_right(Puzzle *p){
  int indexGap = find_tile(p,0);
  int move = indexGap - 1;
  int tileValue = puzzle_get_tile(p,move);
  puzzle_set_tile(p,move,0);
  puzzle_set_tile(p,indexGap,tileValue);
}

//finds the tile in p with the specified val using for loop 
int find_tile(Puzzle *p, int val) {
  for (int i = 0; i < (p->size) * (p->size); i++) {
    if (p->tiles[i] == val) {
      return i;
    }
  }
  //if there is no tile in p with the specified value
  return -1;
}

//returns the value of a tile in p at the specified index position
int puzzle_get_tile(const Puzzle *p, int index) {
  //if the index parameter is negative or greater than can be allowed for this puzzle, return -1
  if (index < 0 || index >= (p->size) * (p->size)) {
    return -1;
  }
  return p->tiles[index];
}

void puzzle_set_tile(Puzzle *p, int index, int value) {
  //if the index parameter is invalid, do nothing
  if (index < 0 || index >= (p->size) * (p->size)) {
    return;
  }

  p->tiles[index] = value;
}

int handle_W_command(FILE *in, Puzzle *p, Image *img) {

  if (p == NULL) {
    fprintf(stderr, "No puzzle\n");
    return 1;
  }

  if (img == NULL) {
    fprintf(stderr, "No image\n");
    return 1;
  }

  if ((img->rows) % (p->size) != 0 || (img->cols) % (p->size) != 0) {
    fprintf(stderr, "Invalid image dimensions\n");
    return 1;
  }

  // variable to save horizontal length in pixels of each tile
  int tileLength = (img->rows) / (p->size);

  // variable to save vertical length in pixels of each tile
  int tileWidth = (img->cols) / (p->size);

  char filename1[255];
  char filename2[255];

  if (fscanf(in, "%s %s", filename1, filename2) != 2) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  }

  FILE *fp1 = fopen(filename1, "w");
  FILE *fp2 = fopen(filename2, "w");

  if (fp1 == NULL) {
    fprintf(stderr, "Could not open output image file '%s'\n", filename1);
    return 1;
  }
  if (fp2 == NULL) {
    fprintf(stderr, "Could not open output puzzle file '%s'\n", filename2);
    return 1;
  }

  // black pixel
  Pixel black = {0, 0, 0};

  //pointer to dynamically allocated array to save the scrambled pixels
  Pixel *scramble = malloc((img->rows) * (img->cols) * sizeof(Pixel));

  //ALGORITHM TO FIND THE ORDER OF THE SCRAMBLED PIXELS:

  // variable to act as counter for number of pixels proccessed (at end should equal img->rows * img->cols)
  int numPixel = 0;

  // for all rows in puzzle p
  for (int r = 0; r < (p->size); r++) {
    // variable to save for which row of pixels currently on within each tile 
    int tileFilled = 0;
    // while we have yet to draw a complete tile (tilefilled != tileWidth)
    while (tileFilled < tileWidth) {
      // for all columns in each row r of puzzle p
      for (int c = 0; c < (p->size); c++) {
        //index of the tile at row r column c
        int index = (r * (p->size)) + c;
        // tile value for tile at row r column c in unscrambled puzzle
        int value = puzzle_get_tile(p, index);
	//the row in the unscrambled image which the tile is at
        int unscrambledRow = value / (p->size);
	//the column in the unscrambled image which the tile is at
	int position = value % (p->size) - 1;
	//handles edge cases (when tile is on right edge of puzzle)
	if(value % (p->size) == 0){
	  unscrambledRow--;
	  position = (p->size) - 1;
	}
	//calculates leftmost position for this row of pixels in the specified tile in the unscrambled image
        int start = tileWidth * unscrambledRow* (img->rows) +
                    tileFilled * (img->rows) + position * tileLength;
	//for every pixel in this row of pixels in this tile 
	for (int i = start; i < start + tileLength; i++) {
	  //if the value of this tile is 0, draw black pixels
          if (value == 0) {
            scramble[numPixel] = black;
          } else {
	    //otherwise, add to the scrambled pixel array the pixel at the calculated location in the unscrambled image
            scramble[numPixel] = img->data[i];
          }
          numPixel++;
        }
      }
      tileFilled++;
    }
  }


  //dynamically allocate new Image struct to hold array of scrambled pixels
  Image *newImage = malloc(sizeof(Image));
  newImage->data = scramble;
  newImage->rows = img->rows;
  newImage->cols = img->cols;

  //use WritePPM to write scrambled image to fp1
  int result = WritePPM(fp1, newImage);
  free(newImage->data);
  free(newImage);
  //if WritePPM fails, return with error
  if (result != (img->rows)*(img->cols)) {
    return 1;
  }

  //variable to check if an error occurred when writing information to fp2
  int errorCheck = 1;
  //write the current puzzle configuration to the second output file
  for (int i = 0; i < (p->size) * (p->size); i++) {
    errorCheck = fprintf(fp2, "%d ", p->tiles[i]);
    if(errorCheck < 0){
      fprintf(stderr,"Could not write puzzle data '%s'\n",filename2);
      return 1;
    }
  }
  if (ferror(fp2)) {
    fprintf(stderr, "Could not write puzzle data '%s'\n", filename2);
    return 1;
    }
  fclose(fp1);
  fclose(fp2);
  return 0;
}

int handle_K_command(Puzzle *p){
  if(p == NULL){
    fprintf(stderr,"No puzzle\n");
    return 1;
  }
  //if check_puzzle returns 1
  if(check_puzzle(p)){
    printf("Solved\n");
  }
  else{
    printf("Not solved\n");
  }
  return 0;
}
  
int check_puzzle(Puzzle *p){
  int prev = p->tiles[0];
  for(int i = 0; i < (p->size)*(p->size); i++){
    if(p->tiles[i] == 0)
    {
      //if zero is not in correct location of the missing tile value
      if(i+1 != p->gapNumber){
     	//NOT SOLVED
	return 0;
      }
    }
    //if the tile values are not ordered in increasing order
    else if((p->tiles[i]) < prev){
      //NOT SOLVED
      return 0;
    }
    prev = p->tiles[i];
  }
  //SOLVED
  return 1;
}


int handle_V_command(Puzzle *p){
  if(p == NULL){
    fprintf(stderr,"No puzzle\n");
    return 1;
  }
  if(p->size != 3){
    fprintf(stderr,"Size of puzzle incompatible with V command\n");
    return 1;
  }
  
  int max_steps = 31;
  char steps[max_steps];
  for(int i = 0; i < max_steps; i++){
    steps[i] = 0;
  }
  int result = -1;
  result = solve_puzzle(p, steps, max_steps, 0, 'b');

  //if solution is found, print steps as saved in steps array
  if(result != -1){
    int i = 0;
    while(steps[i] != 0){
      printf("S %c\n",steps[i]);
      i++;
    }
  }
  //if no solution is found
  else {
    printf("No solution found\n");
  }
  return 0;
}

int solve_puzzle(Puzzle *p, char steps[], int max_steps, int cur_steps, char prev){
  //if p is solved
  if(check_puzzle(p) == 1){
    return cur_steps; //success
  }
  //if you've tried more steps than max_steps allows
  if(cur_steps >= max_steps){
    return -1; //FAILURE
  }

  //variable to hold return value of recursively calling solve_puzzle
  int result = -2;
  
  //move to the left
  //if the previous move was not right and left is a valid move
  if(prev != 'r' && check_left(p) == 0){
    //create a copy of the puzzle
    Puzzle *new = copy_puzzle(p);
    //perform the left operation on the copy
    slide_left(new);
    //recursively call the solve_puzzle function. cur_steps increases by 1 and prev is now l for left
    //if solve_puzzle does not fail (return -1)
    if((result = solve_puzzle(new,steps,max_steps,cur_steps+1,'l')) != -1){
      //add 'l' for left to the steps array
      steps[cur_steps] = 'l';
      //destroy (free) the copy of the puzzle
      puzzle_destroy(new);
      return result;
    }
    else{
      //if solve_puzzle fails, still destroy (free) the copy of the puzzle
      puzzle_destroy(new);
    }
  }

  if(prev != 'l' && check_right(p) == 0){
    Puzzle *new = copy_puzzle(p);
    slide_right(new);
    if((result = solve_puzzle(new,steps,max_steps,cur_steps+1,'r')) != -1){
      steps[cur_steps] = 'r';
      puzzle_destroy(new);
      return result;
    }
    else{
      puzzle_destroy(new);
    }
  }
  
  if(prev != 'd' && check_up(p) == 0){
    Puzzle *new = copy_puzzle(p);
    slide_up(new);
    if((result = solve_puzzle(new,steps,max_steps,cur_steps+1,'u')) != -1){
      steps[cur_steps] = 'u';
      puzzle_destroy(new);
      return result;
    }
    else{
      puzzle_destroy(new);
    }
  }

  if(prev != 'u' && check_down(p) == 0){
    Puzzle *new = copy_puzzle(p);
    slide_down(new);
    if((result = solve_puzzle(new,steps,max_steps,cur_steps+1,'d')) != -1){
      steps[cur_steps] = 'd';
      puzzle_destroy(new);
      return result;
    }
    else{
      puzzle_destroy(new);
    }
  }
  
  return -1; //FAILURE
}

Puzzle * copy_puzzle(Puzzle *p){
  Puzzle *copy = (Puzzle*) malloc(sizeof(Puzzle));

  //copying the values for the fields in p into copy
  copy->size = p->size;
  copy->gapNumber = p->gapNumber;

  
  copy->tiles = malloc(sizeof(int)*(p->size)*(p->size));

  for(int i = 0; i < (p->size)*(p->size); i++){
    copy->tiles[i] = p->tiles[i];
  }

  return copy;
}
