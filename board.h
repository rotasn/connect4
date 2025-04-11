#ifndef BOARD_H
#define BOARD_H

#define ROWS 7
#define COLS 7
#define EMPTY_CELL '.'
#define PLAYER_SYMBOL '*'
#define CPU_SYMBOL '+'

// Initialize the game board with empty cells
void initialize_board(char board[ROWS][COLS]);

// Print the game board to console (will be replaced with SDL rendering later)
void print_board(char board[ROWS][COLS]);

// Handle player move - returns 1 if successful, 0 if invalid
int make_player_move(char board[ROWS][COLS], int column);

// Drop a piece in the specified column - returns the row where piece landed or -1 if column is full
int drop_piece(char board[ROWS][COLS], int column, char symbol);

// Check if the board is full (draw condition)
int is_board_full(char board[ROWS][COLS]);

#endif