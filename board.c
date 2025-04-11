#include <stdio.h>
#include "board.h"

void initialize_board(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = EMPTY_CELL;
        }
    }
}

void print_board(char board[ROWS][COLS]) {
    printf("  0 1 2 3 4 5 6\n"); // Column numbers for player reference
    printf(" ---------------\n");
    
    for (int i = 0; i < ROWS; i++) {
        printf("|");
        for (int j = 0; j < COLS; j++) {
            printf("%c|", board[i][j]);
        }
        printf("\n");
    }
    printf(" ---------------\n");
}

int make_player_move(char board[ROWS][COLS], int column) {
    return drop_piece(board, column, PLAYER_SYMBOL);
}

int drop_piece(char board[ROWS][COLS], int column, char symbol) {
    // Check if column is valid
    if (column < 0 || column >= COLS) {
        return -1;
    }
    
    // Check if column is full
    if (board[0][column] != EMPTY_CELL) {
        return -1;
    }
    
    // Find the lowest empty cell in the column
    int row;
    for (row = ROWS - 1; row >= 0; row--) {
        if (board[row][column] == EMPTY_CELL) {
            board[row][column] = symbol;
            return row;
        }
    }
    
    return -1; // Should never reach here if column validity is checked
}

int is_board_full(char board[ROWS][COLS]) {
    for (int j = 0; j < COLS; j++) {
        if (board[0][j] == EMPTY_CELL) {
            return 0; // At least one column is not full
        }
    }
    return 1; // All columns are full
}