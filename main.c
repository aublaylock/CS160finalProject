#include <stdio.h>
#include <ctype.h>
#include "hash.h"

int startingLetters[] = {9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1, 2}; //alphabetical w/ ? at end
int letterValues[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10, 0};

typedef struct {
    int len;
    char letters[15];
} Word;

typedef struct {
    char board[15][15];
    int tilebag[27];
} State;

int letterValue(char letter){
    if(letter = '?'){
        return 0;
    }
    return letterValues[tolower(letter) - 'a'];
}

void placeWord(State* state, Word word, int row, int col, char direction){
    for(int i = 0; i < word.len; i++){
        if( ((*state).board)[row][col] == ' '){
            ((*state).board)[row][col] = word.letters[i];
        }
        else{
            i--;
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
    for(int row = 0; row < 15; row++){
        for(int col = 0; col < 15; col++){
            printf("%c", state.board[row][col]);
            if(col != 14){
                printf(" | ");
            }
        }
        printf("\n");
        if(row != 14){
            printf("_______________________________________________________\n");
        }
    }
}

int allWordsAreValid(HashSet* dict, State state){
    char curWord[16];
    int wordIndex = 0;

    //check horizontals
    for(int row = 0; row < 15; row++){
        wordIndex = 0;
        for(int col = 0; col < 15; col++){
            if(state.board[row][col] == ' '){
                curWord[wordIndex] = '\0';
                if(wordIndex > 1 /* Single letters are fine */ && !is_valid_word(dict, curWord)){
                    printf("INVALID WORD: %s;\n", curWord);
                    return 0;
                }
                else{
                    wordIndex = 0;
                }
            }
            else{
                curWord[wordIndex] = state.board[row][col];
                wordIndex++;
            }
        }
    }
    //check verticals
    for(int col = 0; col < 15; col++){
        wordIndex = 0;
        for(int row = 0; row < 15; row++){
            if(state.board[row][col] == ' '){
                curWord[wordIndex] = '\0';
                if(wordIndex > 1 /* Single letters are fine */ && !is_valid_word(dict, curWord)){
                    printf("INVALID WORD: %s;\n", curWord);
                    return 0;
                }
                else{
                    wordIndex = 0;
                }
            }
            else{
                curWord[wordIndex] = state.board[row][col];
                wordIndex++;
            }
        }
    }



    return 1;
}

int main(){
    State testState = {
        {
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', 'C', 'R', 'A', 'N', 'E', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
        }, 
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
    };

    placeWord(&testState, (Word){6, "CRABIN"}, 6, 7, 'D');

    printBoard(testState);

    HashSet *dict = load_dictionary("Collins Scrabble Words (2019).txt");
    printf("Is valid: %i", allWordsAreValid(dict, testState));


    free_set(dict);
    
    return 0;
} 