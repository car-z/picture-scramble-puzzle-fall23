#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "puzzle.h"

// also tests puzzle_destroy
void test_puzzle_create(void) {

  Puzzle *p3 = puzzle_create(3);
  Puzzle *p5 = puzzle_create(5);

  assert(p3->size == 3);
  for (int i = 0; i < 9; ++i) {
    assert(p3->tiles[i] == 0);
  }

  assert(p5->size == 5);
  for (int i = 0; i < 25; ++i) {
    assert(p5->tiles[i] == 0);
  }

  puzzle_destroy(p3);
  puzzle_destroy(p5);
}

// also tests puzzle_get_tile
void test_puzzle_set_tile(void) {
  int init[4][4] = {
    { 5, 7, 1, 2 },
    { 13, 9, 3, 4 },
    { 0, 8, 6, 11 },
    { 14, 15, 10, 12 },
  };

  Puzzle *p4 = puzzle_create(4);
  for (int i = 0; i < 4; ++i){
    for (int j = 0; j < 4; ++j){
      int index = i*4 + j;
      puzzle_set_tile(p4, index, init[i][j]);
    }
  }
  for (int i = 0; i < 4; ++i){
    for (int j = 0; j < 4; ++j){
      int index = i*4 + j;
      assert(puzzle_get_tile(p4, index) == init[i][j]);
    }
  }
  puzzle_destroy(p4);
}

// ...other test functions...

void test_slide_valid(void){
  int a[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  Puzzle * p5 = puzzle_create(3);
  for(int i = 0; i < 9; i++){
    p5->tiles[i] = a[i];
  }

  assert(check_right(p5) == 1);
  assert(check_down(p5) == 1);

  p5->tiles[0] = 8;
  p5->tiles[8] = 0;
  
  assert(check_left(p5) == 1);
  assert(check_up(p5) == 1);

  puzzle_destroy(p5);
}

void test_slide(void){
  int a[25] = {0,1,2,3,4,5,6,7,8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
  Puzzle * p6 = puzzle_create(5);
  for(int i = 0; i < 25; i++){
    p6->tiles[i] = a[i];
  }

  assert(check_up(p6) == 0);
  slide_up(p6);
  assert(p6->tiles[0] == 5);
  assert(check_left(p6) == 0);
  slide_left(p6);
  assert(p6->tiles[5] = 6);
  assert(check_down(p6) == 0);
  slide_down(p6);
  assert(check_right(p6) == 0);
  slide_right(p6);
  assert(p6->tiles[11] = 11);

  puzzle_destroy(p6);
}

  

int main(void) {
  test_puzzle_create();
  test_puzzle_set_tile();
  // ...call other test functions...

  test_slide_valid();
  test_slide();
  
  printf("All tests passed!\n");
  return 0;
}
