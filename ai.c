#include <stdlib.h>
#include <time.h>
#include "ai.h"

// Check if making a move at a given position would create a winning condition
static int is_winning_move(char board[ROWS][COLS], int row, int col, char symbol) {
    int count, i, j;
    
    // Check horizontally
    count = 0;
    for (j = max(0, col - 3); j <= min(COLS - 1, col + 3); j++) {
        if (board[row][j] == symbol) {
            count++;
            if (count == 4) return 1;
        } else {
            count = 0;
        }
    }
    
    // Check vertically
    count = 0;
    for (i = max(0, row - 3); i <= min(ROWS - 1, row + 3); i++) {
        if (board[i][col] == symbol) {
            count++;
            if (count == 4) return 1;
        } else {
            count = 0;
        }
    }
    
    // Check diagonally (down-right)
    count = 0;
    for (i = -3; i <= 3; i++) {
        int r = row + i;
        int c = col + i;
        if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
            if (board[r][c] == symbol) {
                count++;
                if (count == 4) return 1;
            } else {
                count = 0;
            }
        }
    }
    
    // Check diagonally (up-right)
    count = 0;
    for (i = -3; i <= 3; i++) {
        int r = row - i;
        int c = col + i;
        if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
            if (board[r][c] == symbol) {
                count++;
                if (count == 4) return 1;
            } else {
                count = 0;
            }
        }
    }
    
    return 0;
}

// Helper functions for min/max
static int min(int a, int b) {
    return (a < b) ? a : b;
}

static int max(int a, int b) {
    return (a > b) ? a : b;
}

// Try to find a winning move for the given symbol
static int find_winning_move(char board[ROWS][COLS], char symbol) {
    char temp_board[ROWS][COLS];
    int i, j;
    
    // Try each column
    for (j = 0; j < COLS; j++) {
        // Copy the board
        for (i = 0; i < ROWS; i++) {
            for (int k = 0; k < COLS; k++) {
                temp_board[i][k] = board[i][k];
            }
        }
        
        // Try this column
        int row = drop_piece(temp_board, j, symbol);
        if (row != -1) {
            // Check if this creates a win
            if (is_winning_move(temp_board, row, j, symbol)) {
                return j;
            }
        }
    }
    
    return -1; // No winning move found
}

// Try to find a move that blocks opponent from winning
static int find_blocking_move(char board[ROWS][COLS]) {
    return find_winning_move(board, PLAYER_SYMBOL);
}

// Make a strategic move (try to connect 2 or 3 pieces)
static int make_strategic_move(char board[ROWS][COLS]) {
    // Implementation of strategic move logic goes here
    // This is a simplified version of the original AI strategy
    
    // Try to find a move that connects to existing pieces
    for (int j = 0; j < COLS; j++) {
        int row = drop_piece(board, j, CPU_SYMBOL);
        if (row != -1) {
            // Check if this move connects to other pieces
            // Implementation details omitted for brevity
            // You can expand this with your original strategic logic
            
            // Undo the move
            board[row][j] = EMPTY_CELL;
            return j;
        }
    }
    
    return -1;
}

int make_cpu_move(char board[ROWS][COLS]) {
    static int initialized = 0;
    int column;
    
    // Initialize random number generator
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }
    
    // Strategy 1: Try to win in one move
    column = find_winning_move(board, CPU_SYMBOL);
    if (column != -1) {
        drop_piece(board, column, CPU_SYMBOL);
        return column;
    }
    
    // Strategy 2: Block player's winning move
    column = find_blocking_move(board);
    if (column != -1) {
        drop_piece(board, column, CPU_SYMBOL);
        return column;
    }
    
    // Strategy 3: Make a strategic move
    column = make_strategic_move(board);
    if (column != -1) {
        drop_piece(board, column, CPU_SYMBOL);
        return column;
    }
    
    // Strategy 4: Choose a random valid column
    do {
        column = rand() % COLS;
    } while (board[0][column] != EMPTY_CELL);
    
    drop_piece(board, column, CPU_SYMBOL);
    return column;
}