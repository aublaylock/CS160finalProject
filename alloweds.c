#include "alloweds.h"
#include "board.h"

int allHorizontalWordsAreValid(HashSet *dict, State state)
{
    char curWord[16];
    int wordIndex = 0;

    for (int row = 0; row < 15; row++)
    {
        wordIndex = 0;
        for (int col = 0; col < 15; col++)
        {
            if (state.board[row][col] == ' ')
            {
                curWord[wordIndex] = '\0';
                if (wordIndex > 1 /* Single letters are fine */ && !is_valid_word(dict, curWord))
                {
                    return 0;
                }
                else
                {
                    wordIndex = 0;
                }
            }
            else
            {
                curWord[wordIndex] = state.board[row][col];
                wordIndex++;
            }
        }
        curWord[wordIndex] = '\0';
        if (wordIndex > 1 && !is_valid_word(dict, curWord))
        {
            return 0;
        }
    }
    return 1;
}

int allVerticalWordsAreValid(HashSet *dict, State state)
{
    char curWord[16];
    int wordIndex = 0;

    for (int col = 0; col < 15; col++)
    {
        wordIndex = 0;
        for (int row = 0; row < 15; row++)
        {
            if (state.board[row][col] == ' ')
            {
                curWord[wordIndex] = '\0';
                if (wordIndex > 1 /* Single letters are fine */ && !is_valid_word(dict, curWord))
                {
                    // printf("INVALID WORD: %s;\n", curWord);
                    return 0;
                }
                else
                {
                    wordIndex = 0;
                }
            }
            else
            {
                curWord[wordIndex] = state.board[row][col];
                wordIndex++;
            }
        }
        curWord[wordIndex] = '\0';
        if (wordIndex > 1 && !is_valid_word(dict, curWord))
        {
            return 0;
        }
    }
    return 1;
}

// Makes grid of alloweds for both ACROSS and DOWN moves
void calculateAlloweds(HashSet *dict, Allowed output[][15], State state, Rack rack)
{
    for (int row = 0; row < 15; row++)
    {
        for (int col = 0; col < 15; col++)
        {
            // Initialize Allowed
            output[row][col].forced = ' ';
            for (int letterIndex = 0; letterIndex < 26; letterIndex++)
            {
                output[row][col].allowedAcross[letterIndex] = 0;
                output[row][col].scoresAcross[letterIndex] = 0;
                output[row][col].allowedVertical[letterIndex] = 0;
                output[row][col].scoresVertical[letterIndex] = 0;
            }
            // Populate
            // Forced
            if (state.board[row][col] != ' ')
            {
                output[row][col].forced = state.board[row][col];
            }
            // Allowed
            else
            {
                // Try letters in rack
                for (int i = 0; i < 7; i++)
                {
                    state.board[row][col] = rack.letters[i];
                    // If the letter keeps horizontal words valid (for a potential DOWN move)
                    if (allHorizontalWordsAreValid(dict, state))
                    {
                        // Add the letter as allowed for vertical placement
                        output[row][col].allowedVertical[rack.letters[i] - 'A'] = 1;
                        // Add the score of the horizontal cross-word that the letter creates
                        int score = letterValue(rack.letters[i]) * letterMultiplier(row, col);
                        for (int leftIndex = col - 1; leftIndex > -1 && state.board[row][leftIndex] != ' '; leftIndex--)
                        {
                            score += letterValue(state.board[row][leftIndex]);
                        }
                        for (int rightIndex = col + 1; rightIndex < 15 && state.board[row][rightIndex] != ' '; rightIndex++)
                        {
                            score += letterValue(state.board[row][rightIndex]);
                        }
                        // if it's just the one letter
                        if (score == letterValue(rack.letters[i]) * letterMultiplier(row, col))
                        {
                            // no horizontal score
                            score = 0;
                        }
                        score *= wordMultiplier(row, col);
                        output[row][col].scoresVertical[rack.letters[i] - 'A'] = score;
                    }
                    // If the letter keeps vertical words valid (for a potential ACROSS move)
                    if (allVerticalWordsAreValid(dict, state))
                    {
                        // Add the letter as allowed for horizontal placement
                        output[row][col].allowedAcross[rack.letters[i] - 'A'] = 1;
                        // Add the score of the vertical cross-word that the letter creates
                        int score = letterValue(rack.letters[i]) * letterMultiplier(row, col);
                        for (int upIndex = row - 1; upIndex > -1 && state.board[upIndex][col] != ' '; upIndex--)
                        {
                            score += letterValue(state.board[upIndex][col]);
                        }
                        for (int downIndex = row + 1; downIndex < 15 && state.board[downIndex][col] != ' '; downIndex++)
                        {
                            score += letterValue(state.board[downIndex][col]);
                        }
                        // if it's just the one letter
                        if (score == letterValue(rack.letters[i]) * letterMultiplier(row, col))
                        {
                            // no vertical score
                            score = 0;
                        }
                        score *= wordMultiplier(row, col);
                        output[row][col].scoresAcross[rack.letters[i] - 'A'] = score;
                    }
                }
                state.board[row][col] = ' ';
            }
        }
    }
}
