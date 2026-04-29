#include <stdio.h>
#include <string.h>
#include "solver.h"
#include "board.h"

Rack rackWithoutFirstOccurence(Rack rack, char letter){
    for(int rackIndex = 0; rackIndex < 7; rackIndex++){
        if(rack.letters[rackIndex] == letter){
            rack.letters[rackIndex] = ' ';
            return rack;
        }
    }
    return rack;
}
Word wordWithLetter(Word word, char letter){
    // printf("Length: %i | Word: %s\n", word.len, word.letters);
    word.letters[word.len] = letter;
    word.letters[word.len + 1] = '\0';
    word.len++;
    return word;
}

void printValidWordsFromLocation(HashSet* dict, Allowed allowed[][15], int minLen, int row, int col, Rack rack, int lettersPlayed, Word curWord, int currentLettersScore, int currentWordMultiplier, int crossWordsScore, Move* bestMoveSoFar, char direction){
    int pos = (direction == 'D') ? row : col;
    if((pos == 15 || allowed[row][col].forced == ' ') && is_valid_word(dict, curWord.letters) && lettersPlayed >= minLen){
        int moveScore = currentLettersScore * currentWordMultiplier + crossWordsScore + (50 * (lettersPlayed >= 7)); //bingo bonus
        if(moveScore > bestMoveSoFar->score){
            bestMoveSoFar->score = moveScore;
            bestMoveSoFar->word = curWord;
            bestMoveSoFar->direction = direction;
            bestMoveSoFar->row = (direction == 'D') ? row - curWord.len : row;
            bestMoveSoFar->col = (direction == 'D') ? col : col - curWord.len;
            printf("Found word: %s; %c from (%i, %i) for a score of %i + %i + %i = %i\n", curWord.letters, direction, bestMoveSoFar->row, bestMoveSoFar->col, currentLettersScore * currentWordMultiplier, crossWordsScore, (50 * (lettersPlayed >= 7)), moveScore);
        }
    }
    if(pos > 14){
        return;
    }
    int nextRow = row + (direction == 'D');
    int nextCol = col + (direction == 'R');
    char forcedLetter = allowed[row][col].forced;
    if(forcedLetter != ' '){
        printValidWordsFromLocation(dict, allowed, minLen, nextRow, nextCol, rack, lettersPlayed, wordWithLetter(curWord, forcedLetter), currentLettersScore + letterValue(forcedLetter), currentWordMultiplier, crossWordsScore, bestMoveSoFar, direction);
    }
    else{
        //for all of the letters
        for(int allowedIndex = 0; allowedIndex < 26; allowedIndex++){
            char curLetter = allowedIndex + 'A';
            Rack rackUsingLetter = rackWithoutFirstOccurence(rack, curLetter);
            //if the letter is allowed @ row, col
            int isAllowed = (direction == 'D') ? allowed[row][col].allowedVertical[allowedIndex] : allowed[row][col].allowedAcross[allowedIndex];
            if(isAllowed != 0){
                //and if we still have the letter in the rack
                if(strcmp(rack.letters, rackUsingLetter.letters) != 0){
                    //try that letter
                    int newLettersScore = currentLettersScore + letterValue(curLetter) * letterMultiplier(row, col);
                    int crossScore = (direction == 'D') ? allowed[row][col].scoresVertical[curLetter - 'A'] : allowed[row][col].scoresAcross[curLetter - 'A'];
                    printValidWordsFromLocation(dict, allowed, minLen, nextRow, nextCol, rackUsingLetter, lettersPlayed + 1, wordWithLetter(curWord, curLetter), newLettersScore, currentWordMultiplier * wordMultiplier(row, col), crossWordsScore + crossScore, bestMoveSoFar, direction);
                }
            }
        }
    }
    return;
}

void calculateStarts(Starts* starts, State state){
    //initialize
    for(int row = 0; row < 15; row++){
        for(int col = 0; col < 15; col++){
            starts->board[row][col].minLengthDown = 99;
            starts->board[row][col].minLengthAcross = 99;
        }
    }

    for(int row = 0; row < 15; row++){
        for(int col = 0; col < 15; col++){
            //if there is a tile
            if(state.board[row][col] != ' '){
                //Update all spaces that could reach that tile

                //DOWN starts
                //If there are no tiles above, you can start on a tile, you can't start below a tile.
                if(row - 1 > -1){
                    if(state.board[row-1][col] == ' '){
                        starts->board[row][col].minLengthDown = 1;
                    }
                    else{
                        starts->board[row][col].minLengthDown = -1;
                    }
                }
                else{
                    //row 0: no tile above, valid start position
                    starts->board[row][col].minLengthDown = 1;
                }
                if(row + 1 < 15){
                    starts->board[row+1][col].minLengthDown = -1; //can't start below
                }

                for(int updatingRow = row; (row - updatingRow) < 7 && updatingRow > 0; updatingRow--){
                    if(col - 1 > -1 && state.board[updatingRow][col-1] == ' '){
                        starts->board[updatingRow][col-1].minLengthDown = min((row - updatingRow)+1, starts->board[updatingRow][col-1].minLengthDown); //Update min length to get left adjacency
                    }
                    if(updatingRow - 1 >= 0 && state.board[updatingRow-1][col] == ' '){
                        starts->board[updatingRow-1][col].minLengthDown = min((row - updatingRow)+1, starts->board[updatingRow-1][col].minLengthDown); //Update directly above
                    }
                    if(col + 1 < 15 && state.board[updatingRow][col+1] == ' '){
                        starts->board[updatingRow][col+1].minLengthDown = min((row - updatingRow)+1, starts->board[updatingRow][col+1].minLengthDown); //Update min length to get right adjacency
                    }
                }

                //ACROSS starts
                //If there are no tiles to the left, you can start on a tile, you can't start to the right of a tile.
                if(col - 1 > -1){
                    if(state.board[row][col-1] == ' '){
                        starts->board[row][col].minLengthAcross = 1;
                    }
                    else{
                        starts->board[row][col].minLengthAcross = -1;
                    }
                }
                else{
                    //col 0: no tile to the left, valid start position
                    starts->board[row][col].minLengthAcross = 1;
                }
                if(col + 1 < 15){
                    starts->board[row][col+1].minLengthAcross = -1; //can't start to the right of a tile
                }

                for(int updatingCol = col; (col - updatingCol) < 7 && updatingCol > 0; updatingCol--){
                    if(row - 1 > -1 && state.board[row-1][updatingCol] == ' '){
                        starts->board[row-1][updatingCol].minLengthAcross = min((col - updatingCol)+1, starts->board[row-1][updatingCol].minLengthAcross); //Update min length to get above adjacency
                    }
                    if(updatingCol - 1 >= 0 && state.board[row][updatingCol-1] == ' '){
                        starts->board[row][updatingCol-1].minLengthAcross = min((col - updatingCol)+1, starts->board[row][updatingCol-1].minLengthAcross); //Update directly left
                    }
                    if(row + 1 < 15 && state.board[row+1][updatingCol] == ' '){
                        starts->board[row+1][updatingCol].minLengthAcross = min((col - updatingCol)+1, starts->board[row+1][updatingCol].minLengthAcross); //Update min length to get below adjacency
                    }
                }
            }
        }
    }
}
