#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ppm_io.h"
#include "puzzle.h"

int main(int argc, char **argv) {
  if (argc < 1 || argc > 2) {
    fprintf(stderr, "Usage: ./puzzle [<command file>]\n");
    return 1;
  }

  //should read commands from stdin
  FILE* input = stdin;

  //if a filename is supplied in command-line argument, read from the file
  if(argc == 2){
    input = fopen(argv[1],"r");
    if(input == NULL){
      fprintf(stderr,"Error: could not open input file.\n");
      return 1;
    }
  }

  //variable to save the letter input into
  char command;
  //pointer to current instance of Puzzle
  Puzzle * curPuzzle = NULL;

  //pointer to background image of Puzzle
  Image * backgroundImage = NULL;
  
  //loop runs until ctrl-d is pressed
  while(fscanf(input," %c",&command) == 1){
    //if 'Q' is pressed, loop immediately breaks
    if(command == 'Q'){
      break;
    }

    //variable to save the return value of the handle_X_command s. If result = 1, there was an error in the handle_X_command 
    int result  = -1;
    
    switch(command){
    case 'C':
      if(curPuzzle != NULL){
	puzzle_destroy(curPuzzle);
      }
      result = handle_C_command(input,&curPuzzle);
      if(result == 1){
        puzzle_destroy(curPuzzle);
	image_destroy(backgroundImage);
	return 1;
      }
      break;
    case 'T':
      result = handle_T_command(input,curPuzzle);
      if(result == 1){
        puzzle_destroy(curPuzzle);
	image_destroy(backgroundImage);
	return 1;
      }
      break;
    case 'I':
      result = handle_I_command(input,&backgroundImage);
      if(result == 1){
	puzzle_destroy(curPuzzle);
	image_destroy(backgroundImage);
	return 1;
      }
      break;
    case 'P':
      result = handle_P_command(curPuzzle);
      if(result == 1){
	puzzle_destroy(curPuzzle);
	image_destroy(backgroundImage);
	return 1;
      }
      break;
    case 'S':
      result = handle_S_command(input, curPuzzle);
      if(result == 1){
	puzzle_destroy(curPuzzle);
	image_destroy(backgroundImage);
	return 1;
      }
      break;
    case 'W':
      result = handle_W_command(input, curPuzzle, backgroundImage);
      if(result == 1){
	puzzle_destroy(curPuzzle);
	image_destroy(backgroundImage);
	return 1;
      }
      break;
    case 'K':
      result = handle_K_command(curPuzzle);
      if(result == 1){
	puzzle_destroy(curPuzzle);
	image_destroy(backgroundImage);
	return 1;
      }
      break;
    case 'V':
      result = handle_V_command(curPuzzle);
      if(result == 1){
	puzzle_destroy(curPuzzle);
	image_destroy(backgroundImage);
	return 1;
      }
      break;
    default: //if the entered command is not any of the above
      fprintf(stderr,"Invalid command '%c'\n",command);
      puzzle_destroy(curPuzzle);
      image_destroy(backgroundImage);
      return 1;
    }
  }

  fclose(input);
  puzzle_destroy(curPuzzle);
  image_destroy(backgroundImage);
  return 0;
}
