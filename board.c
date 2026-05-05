#include <stdio.h>
#include "board.h"

#define RED   "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

int letterValues[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10, 0};

int min(int a, int b){
    if(a < b){
        return a;
    }
    return b;
}
int max(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}

int letterMultiplier(int row, int col){
    //Triple Letter Score
    if((row == 1 || row == 13) && (col == 5 || col == 9)) return 3;
    if((row == 5 || row == 9) && (col == 1 || col == 5 || col == 9 || col == 13)) return 3;
    //Double Letter Score
    if((row == 0 || row == 14) && (col == 3 || col == 11)) return 2;
    if((row == 2 || row == 12) && (col == 6 || col == 8)) return 2;
    if((row == 3 || row == 11) && (col == 0 || col == 7 || col == 14)) return 2;
    if((row == 6 || row == 8) && (col == 2 || col == 6 || col == 8 || col == 12)) return 2;
    if(row == 7 && (col == 3 || col == 11)) return 2;
    return 1;
}

int wordMultiplier(int row, int col){
    //Triple Word Score
    if( row == 0 && col == 0 || row == 0 && col == 7 || row == 0 && col == 14 ||
        row == 7 && col == 0 ||                         row == 7 && col == 14 ||
        row == 14 && col == 0 || row == 14 && col == 7 || row == 14 && col == 14){
        return 3;
    }
    //Double Word Score
    if((row == 7 && col == 7) ||
       ((row == col || row + col == 14) && ((row >= 1 && row <= 4) || (row >= 10 && row <= 13)))){
        return 2;
    }
    return 1;
}

int letterValue(char letter){
    if(letter == '?'){
        return 0;
    }
    return letterValues[letter - 'A'];
}

void placeWord(State* state, Word word, int row, int col, char direction){
    for(int i = 0; i < word.len; i++){
        if( ((*state).board)[row][col] == ' '){
            ((*state).board)[row][col] = word.letters[i] + ('a' - 'A');
        }
        if(direction == 'R'){
            col++;
        }
        else{
            row++;
        }
    }
}

void printBoard(State state){
    printf("   ");
    for(int row = 0; row < 15; row++){
        printf(" %2i ", row);
    }
    printf("\n");
    for(int row = 0; row < 15; row++){
        printf("%2i | ", row);
        for(int col = 0; col < 15; col++){
            if(state.board[row][col] > 'Z'){
                printf(RED "%c" RESET, state.board[row][col] - ('a'-'A'));
            }
            else{
                printf("%c", state.board[row][col]);
            }
            printf(" | ");
        }
        printf("\n");
        if(row != 14){
            printf("    ____________________________________________________________\n");
        }
    }
}
