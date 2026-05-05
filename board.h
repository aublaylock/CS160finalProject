#ifndef BOARD_H
#define BOARD_H

#include "types.h"

int min(int a, int b);
int max(int a, int b);
int letterMultiplier(int row, int col);
int wordMultiplier(int row, int col);
int letterValue(char letter);
void placeWord(State *state, Word word, int row, int col, char direction);
void printBoard(State state);

#endif
