#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "hash.h"

int startingLetters[] = {9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1, 2}; //alphabetical w/ ? at end
int letterValues[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10, 0};

typedef struct {
    int len;
    char letters[16]; //Possible ending \0
} Word;

typedef struct {
    char letters[7];
} Rack;

typedef struct {
    char board[15][15];
} State;

typedef struct {
    int allowed[26];
    char forced;
} Allowed;

int letterValue(char letter){
    if(letter == '?'){
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

int allHorizontalWordsAreValid(HashSet* dict, State state){
    char curWord[16];
    int wordIndex = 0;

    for(int row = 0; row < 15; row++){
        wordIndex = 0;
        for(int col = 0; col < 15; col++){
            if(state.board[row][col] == ' '){
                curWord[wordIndex] = '\0';
                if(wordIndex > 1 /* Single letters are fine */ && !is_valid_word(dict, curWord)){
                    // printf("INVALID WORD: %s;\n", curWord);
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

int allVerticalWordsAreValid(HashSet* dict, State state){
    char curWord[16];
    int wordIndex = 0;

    for(int col = 0; col < 15; col++){
        wordIndex = 0;
        for(int row = 0; row < 15; row++){
            if(state.board[row][col] == ' '){
                curWord[wordIndex] = '\0';
                if(wordIndex > 1 /* Single letters are fine */ && !is_valid_word(dict, curWord)){
                    // printf("INVALID WORD: %s;\n", curWord);
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

// Makes grid of alloweds: considering a potential DOWN move
void horizontalAlloweds(HashSet* dict, Allowed output[][15], State state, Rack rack){
    for(int row = 0; row < 15; row++){
        for(int col = 0; col < 15; col++){
            //Initialize Allowed
            output[row][col].forced = ' ';
            for(int letterIndex = 0; letterIndex < 26; letterIndex++){
                output[row][col].allowed[letterIndex] = 0;
            }
            //Populate
            //Forced
            if(state.board[row][col] != ' '){
                output[row][col].forced = state.board[row][col];
            }
            //Allowed
            else{
                //Try letters in rack
                for(int i = 0; i < 7; i++){
                    // if(row == 7 && col == 10){
                    //     printf("Checking letter: %c\n", rack[i]);
                    // }
                    state.board[row][col] = rack.letters[i];
                    if(allHorizontalWordsAreValid(dict, state)){
                        // if(row == 7 && col == 10){
                        //     printf("Check Passed, allowing letter index: %i\n", rack[i] - 'A');
                        // }
                        output[row][col].allowed[rack.letters[i] - 'A'] = 1;
                    }
                }
                state.board[row][col] = ' ';
            }
        }
    }
}

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

void printValidWordsDownFromLocation(HashSet* dict, Allowed allowed[][15], int minLen, int row, int col, Rack rack, Word curWord){
    // printf("ROW: %i\n", row);
    if(is_valid_word(dict, curWord.letters) && curWord.len >= minLen){
        printf("Found word: %s\n", curWord.letters);
    }

    if(allowed[row][col].forced != ' '){
        printValidWordsDownFromLocation(dict, allowed, minLen, row + 1, col, rack, wordWithLetter(curWord, allowed[row][col].forced));
    }
    else{
        for(int allowedIndex = 0; allowedIndex < 25; allowedIndex++){
            char curLetter = allowedIndex + 'A';
            Rack rackUsingLetter = rackWithoutFirstOccurence(rack, curLetter);
            //if allowed letter
            if(allowed[row][col].allowed[allowedIndex] != 0){
                // printf("Checking Letter: %c\n", curLetter);
                //if we had the letter still in the rack
                if(strcmp(rack.letters, rackUsingLetter.letters) != 0){
                    printValidWordsDownFromLocation(dict, allowed, minLen, row + 1, col, rackUsingLetter, wordWithLetter(curWord, curLetter));
                }
            }
        }
    }
    return;
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
        }
    };

    placeWord(&testState, (Word){6, "BAANA"}, 5, 8, 'D');

    HashSet *dict = load_dictionary("Collins Scrabble Words (2019).txt");
    
    Allowed hAlloweds[15][15];
    

    // for(int i = 0; i < 26; i++){
    //     if(hAlloweds[9][9].allowed[i]){
    //         printf("%c  ", i + 'A');
    //     }
    // }
    Rack rack = {
        "ASBCDEG"
    };
    Word curWord = {
        0,
        ""
    };
    horizontalAlloweds(dict, hAlloweds, testState, rack);
    printValidWordsDownFromLocation(dict, hAlloweds, 3, 5, 9, rack, curWord);
    printf("\n");


    testState.board[5][9] = '*';
    printBoard(testState);

    free_set(dict);
    return 0;
} 