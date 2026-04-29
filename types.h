#ifndef TYPES_H
#define TYPES_H

extern int letterValues[];

typedef struct {
    int len;
    char letters[16]; //Possible ending \0
} Word;

typedef struct {
    char letters[8];
} Rack;

typedef struct {
    char board[15][15];
} State;

typedef struct {
    int minLengthAcross;
    int minLengthDown;
} Start;

typedef struct {
    Start board[15][15];
} Starts;


typedef struct {
    int allowedAcross[26];
    int scoresAcross[26];
    int allowedVertical[26];
    int scoresVertical[26];
    char forced;
} Allowed;

typedef struct {
    int row;
    int col;
    char direction;
    int score;
    Word word;
} Move;

#endif
