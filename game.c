#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "ai.h"

int select_game_mode() {
    int mode;
    char buffer[100];
    
    printf("Connect Four Game\n");
    printf("----------------\n");
    printf("To play against the computer, press 1\n");
    printf("To exit, press 0\n");
    printf("Your choice: ");
    
    // Read input as string first to prevent crashes with non-integer input
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return MODE_EXIT;
    }
    
    if (sscanf(buffer, "%d", &mode) != 1) {
        return MODE_EXIT; // Default to exit if input is not an integer
    }
    
    return (mode == MODE_PLAY) ? MODE_PLAY : MODE_EXIT;
}

int check_winner(char board[ROWS][COLS], char last_symbol) {
    int i, j;
    
    // Check horizontally
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS - 3; j++) {
            if (board[i][j] != EMPTY_CELL && 
                board[i][j] == board[i][j+1] && 
                board[i][j+1] == board[i][j+2] && 
                board[i][j+2] == board[i][j+3]) {
                
                return (board[i][j] == PLAYER_SYMBOL) ? PLAYER_WINS : CPU_WINS;
            }
        }
    }
    
    // Check vertically
    for (j = 0; j < COLS; j++) {
        for (i = 0; i < ROWS - 3; i++) {
            if (board[i][j] != EMPTY_CELL && 
                board[i][j] == board[i+1][j] && 
                board[i+1][j] == board[i+2][j] && 
                board[i+2][j] == board[i+3][j]) {
                
                return (board[i][j] == PLAYER_SYMBOL) ? PLAYER_WINS : CPU_WINS;
            }
        }
    }
    
    // Check diagonally (down-right)
    for (i = 0; i < ROWS - 3; i++) {
        for (j = 0; j < COLS - 3; j++) {
            if (board[i][j] != EMPTY_CELL && 
                board[i][j] == board[i+1][j+1] && 
                board[i+1][j+1] == board[i+2][j+2] && 
                board[i+2][j+2] == board[i+3][j+3]) {
                
                return (board[i][j] == PLAYER_SYMBOL) ? PLAYER_WINS : CPU_WINS;
            }
        }
    }
    
    // Check diagonally (up-right)
    for (i = 3; i < ROWS; i++) {
        for (j = 0; j < COLS - 3; j++) {
            if (board[i][j] != EMPTY_CELL && 
                board[i][j] == board[i-1][j+1] && 
                board[i-1][j+1] == board[i-2][j+2] && 
                board[i-2][j+2] == board[i-3][j+3]) {
                
                return (board[i][j] == PLAYER_SYMBOL) ? PLAYER_WINS : CPU_WINS;
            }
        }
    }
    
    // Check for draw
    if (is_board_full(board)) {
        return DRAW;
    }
    
    return NO_WINNER;
}

int play_turn(char board[ROWS][COLS]) {
    int column, player_result, cpu_result;
    char buffer[100];
    
    // Player's turn
    printf("Your turn. Enter column (0-6): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1; // Error reading input
    }
    
    if (sscanf(buffer, "%d", &column) != 1) {
        printf("Invalid input. Please enter a number between 0 and 6.\n");
        return 0; // Try again
    }
    
    if (column < 0 || column >= COLS) {
        printf("Column must be between 0 and 6.\n");
        return 0; // Try again
    }
    
    player_result = make_player_move(board, column);
    if (player_result == -1) {
        printf("Column %d is full. Choose another column.\n", column);
        return 0; // Try again
    }
    
    // Check if player won
    int winner = check_winner(board, PLAYER_SYMBOL);
    if (winner != NO_WINNER) {
        return winner;
    }
    
    // CPU's turn
    printf("CPU is thinking...\n");
    cpu_result = make_cpu_move(board);
    if (cpu_result == -1) {
        // This should never happen if is_board_full is checked properly
        printf("CPU could not make a move. The board may be full.\n");
        return DRAW;
    }
    
    // Check if CPU won
    return check_winner(board, CPU_SYMBOL);
}