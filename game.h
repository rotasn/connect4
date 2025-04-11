#ifndef GAME_H
#define GAME_H

#include "board.h"

// Game modes
#define MODE_EXIT 0
#define MODE_PLAY 1

// Winner states
#define NO_WINNER 0
#define PLAYER_WINS 1
#define CPU_WINS 2
#define DRAW 3

// Get user's choice of game mode
int select_game_mode();

// Check if there is a winner
int check_winner(char board[ROWS][COLS], char last_symbol);

// Handle a complete turn (player makes move, then CPU)
int play_turn(char board[ROWS][COLS]);

#endif