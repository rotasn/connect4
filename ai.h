#ifndef AI_H
#define AI_H

#include "board.h"

// Make a move for the CPU - returns the column where move was made or -1 if no move possible
int make_cpu_move(char board[ROWS][COLS]);

#endif