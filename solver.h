#ifndef SOLVER_H
#define SOLVER_H

#include "types.h"
#include "hash.h"

Rack rackWithoutFirstOccurence(Rack rack, char letter);
Word wordWithLetter(Word word, char letter);
void printValidWordsFromLocation(HashSet* dict, Allowed allowed[][15], int minLen, int row, int col, Rack rack, int lettersPlayed, Word curWord, int currentLettersScore, int currentWordMultiplier, int crossWordsScore, Move* bestMoveSoFar, char direction);
void calculateStarts(Starts* starts, State state);

#endif
