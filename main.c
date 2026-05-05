#include <stdio.h>
#include "hash.h"
#include "types.h"
#include "board.h"
#include "alloweds.h"
#include "solver.h"

int main(){
    State state = {
        {
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'Q', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'S', 'T', 'O', 'U', 'T', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'I', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', 'F', 'L', 'I', 'P', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }
    };

    HashSet *dict = load_dictionary("Collins Scrabble Words (2019).txt");

    Allowed alloweds[15][15];

    Rack rack = {
        "RETINAS"
    };
    Word curWord = {
        0,
        ""
    };
    calculateAlloweds(dict, alloweds, state, rack);


    Starts starts;
    calculateStarts(&starts, state);


    Move bestMove = {0};
    for(int row = 0; row < 15; row++){
        for(int col = 0; col < 15; col++){
            if(0 < starts.board[row][col].minLengthAcross && starts.board[row][col].minLengthAcross < 99){
                printValidWordsFromLocation(dict, alloweds, starts.board[row][col].minLengthAcross, row, col, rack, 0, curWord, 0, 1, 0, &bestMove, 'R');
            }
            if(0 < starts.board[row][col].minLengthDown && starts.board[row][col].minLengthDown < 99){
                printValidWordsFromLocation(dict, alloweds, starts.board[row][col].minLengthDown, row, col, rack, 0, curWord, 0, 1, 0, &bestMove, 'D');
            }
        }
    }

    placeWord(&state, bestMove.word, bestMove.row, bestMove.col, bestMove.direction);

    printBoard(state);

    free_set(dict);
    return 0;
}
