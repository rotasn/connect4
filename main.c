#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "game.h"

// Text-based game loop for now - will be replaced with SDL version
void text_game_loop() {
    char board[ROWS][COLS];
    int game_mode, game_status;
    
    // Initialize the board
    initialize_board(board);
    
    // Select game mode
    game_mode = select_game_mode();
    if (game_mode == MODE_EXIT) {
        printf("Thanks for playing!\n");
        return;
    }
    
    // Game loop
    game_status = NO_WINNER;
    while (game_status == NO_WINNER) {
        // Print the current state of the board
        print_board(board);
        
        // Play a turn
        game_status = play_turn(board);
        
        // Handle invalid moves
        if (game_status == 0) {
            continue; // Invalid move, try again
        }
    }
    
    // Print final board state
    print_board(board);
    
    // Announce the result
    switch (game_status) {
        case PLAYER_WINS:
            printf("Congratulations! You win!\n");
            break;
        case CPU_WINS:
            printf("The computer wins. Better luck next time!\n");
            break;
        case DRAW:
            printf("It's a draw! The board is full.\n");
            break;
    }
}

int main(int argc, char *argv[]) {
    // For now, just run the text-based game
    text_game_loop();
    
    return 0;
}