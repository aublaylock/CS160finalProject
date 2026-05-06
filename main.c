#include <stdio.h>
#include "hash.h"
#include "types.h"
#include "board.h"
#include "alloweds.h"
#include "solver.h"

/*
ASSUMPTIONS:

Rack w/ 7 tiles
First word has been placed

*/

int main(){
    State state = {
        {
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'B', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'C', 'R', 'A', 'N', 'E', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }
    };

    HashSet *dict = load_dictionary("Collins Scrabble Words (2019).txt");

    Rack rack = {
        "NATESHG"
    };

    Word curWord = {
        0,
        ""
    };

    Allowed alloweds[15][15];
    calculateAlloweds(dict, alloweds, state, rack);
    Starts starts;
    calculateStarts(&starts, state);


    Move bestMove = {0};
    //For each position on the board
    for(int row = 0; row < 15; row++){
        for(int col = 0; col < 15; col++){
            //if it is a valid start position; try all valid words from that location
            if(0 < starts.board[row][col].minLengthAcross && starts.board[row][col].minLengthAcross < 99){
                printValidWordsFromLocation(dict, alloweds, starts.board[row][col].minLengthAcross, row, col, rack, 0, curWord, 0, 1, 0, &bestMove, 'R');
            }
            if(0 < starts.board[row][col].minLengthDown && starts.board[row][col].minLengthDown < 99){
                printValidWordsFromLocation(dict, alloweds, starts.board[row][col].minLengthDown, row, col, rack, 0, curWord, 0, 1, 0, &bestMove, 'D');
            }
        }
    }

    //place the best word found on the board
    placeWord(&state, bestMove.word, bestMove.row, bestMove.col, bestMove.direction);

    printBoard(state);
    free_set(dict);
    return 0;
}
