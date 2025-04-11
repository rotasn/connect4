#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "game.h"
#include "ai.h"

// Window dimensions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Board rendering constants
#define BOARD_PADDING 50
#define CELL_SIZE 70
#define PIECE_RADIUS 30

// Colors
SDL_Color BACKGROUND_COLOR = {30, 50, 150, 255};     // Dark blue
SDL_Color BOARD_COLOR = {50, 100, 200, 255};         // Blue
SDL_Color EMPTY_CELL_COLOR = {220, 220, 220, 255};   // Light gray
SDL_Color PLAYER_COLOR = {220, 50, 50, 255};         // Red
SDL_Color CPU_COLOR = {220, 220, 50, 255};           // Yellow
SDL_Color TEXT_COLOR = {255, 255, 255, 255};         // White

// SDL resources
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Game state
char game_board[ROWS][COLS];
int game_status = NO_WINNER;
int hover_column = -1;

// Function prototypes
int init_sdl();
void cleanup_sdl();
void render_board();
void render_text(const char *text, int x, int y, int size);
int handle_events();
void run_game_loop();

int init_sdl() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    
    // Create window
    window = SDL_CreateWindow("Connect Four", 
                             SDL_WINDOWPOS_UNDEFINED, 
                             SDL_WINDOWPOS_UNDEFINED, 
                             WINDOW_WIDTH, 
                             WINDOW_HEIGHT, 
                             SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    
    return 1;
}

void cleanup_sdl() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void render_board() {
    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);
    
    // Calculate board dimensions
    int board_width = COLS * CELL_SIZE;
    int board_height = ROWS * CELL_SIZE;
    int board_x = (window_width - board_width) / 2;
    int board_y = (window_height - board_height) / 2;
    
    // Clear screen
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_RenderClear(renderer);
    
    // Draw board background
    SDL_SetRenderDrawColor(renderer, BOARD_COLOR.r, BOARD_COLOR.g, BOARD_COLOR.b, BOARD_COLOR.a);
    SDL_Rect board_rect = {board_x, board_y, board_width, board_height};
    SDL_RenderFillRect(renderer, &board_rect);
    
    // Draw hover column highlight
    if (hover_column >= 0 && hover_column < COLS && game_status == NO_WINNER) {
        SDL_SetRenderDrawColor(renderer, 100, 150, 250, 255); // Light blue highlight
        SDL_Rect hover_rect = {board_x + hover_column * CELL_SIZE, board_y, CELL_SIZE, board_height};
        SDL_RenderFillRect(renderer, &hover_rect);
    }
    
    // Draw cells and pieces
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // Calculate cell position
            int cell_x = board_x + j * CELL_SIZE;
            int cell_y = board_y + i * CELL_SIZE;
            
            // Draw empty cell (circular hole)
            SDL_SetRenderDrawColor(renderer, EMPTY_CELL_COLOR.r, EMPTY_CELL_COLOR.g, EMPTY_CELL_COLOR.b, EMPTY_CELL_COLOR.a);
            
            // Draw filled circle
            int cx = cell_x + CELL_SIZE / 2;
            int cy = cell_y + CELL_SIZE / 2;
            
            // Draw the pieces
            if (game_board[i][j] == PLAYER_SYMBOL) {
                SDL_SetRenderDrawColor(renderer, PLAYER_COLOR.r, PLAYER_COLOR.g, PLAYER_COLOR.b, PLAYER_COLOR.a);
                
                // Draw filled circle for player piece
                for (int dy = -PIECE_RADIUS; dy <= PIECE_RADIUS; dy++) {
                    for (int dx = -PIECE_RADIUS; dx <= PIECE_RADIUS; dx++) {
                        if (dx*dx + dy*dy <= PIECE_RADIUS*PIECE_RADIUS) {
                            SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
                        }
                    }
                }
            } 
            else if (game_board[i][j] == CPU_SYMBOL) {
                SDL_SetRenderDrawColor(renderer, CPU_COLOR.r, CPU_COLOR.g, CPU_COLOR.b, CPU_COLOR.a);
                
                // Draw filled circle for CPU piece
                for (int dy = -PIECE_RADIUS; dy <= PIECE_RADIUS; dy++) {
                    for (int dx = -PIECE_RADIUS; dx <= PIECE_RADIUS; dx++) {
                        if (dx*dx + dy*dy <= PIECE_RADIUS*PIECE_RADIUS) {
                            SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
                        }
                    }
                }
            }
            else {
                // Draw empty cell
                SDL_SetRenderDrawColor(renderer, EMPTY_CELL_COLOR.r, EMPTY_CELL_COLOR.g, EMPTY_CELL_COLOR.b, EMPTY_CELL_COLOR.a);
                
                // Draw filled circle for empty cell
                for (int dy = -PIECE_RADIUS; dy <= PIECE_RADIUS; dy++) {
                    for (int dx = -PIECE_RADIUS; dx <= PIECE_RADIUS; dx++) {
                        if (dx*dx + dy*dy <= PIECE_RADIUS*PIECE_RADIUS) {
                            SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
                        }
                    }
                }
            }
        }
    }
    
    // Draw game status
    if (game_status != NO_WINNER) {
        const char *status_text;
        switch (game_status) {
            case PLAYER_WINS:
                status_text = "You Win! Click to play again.";
                break;
            case CPU_WINS:
                status_text = "CPU Wins! Click to play again.";
                break;
            case DRAW:
                status_text = "It's a Draw! Click to play again.";
                break;
            default:
                status_text = "";
        }
        
        render_text(status_text, window_width / 2, 30, 24);
    }
    
    // Update screen
    SDL_RenderPresent(renderer);
}

void render_text(const char *text, int x, int y, int size) {
    // NOTE: This is a placeholder. For proper text rendering, you would use
    // SDL_ttf library. In this simplified version, we're skipping actual text rendering.
    // In a full implementation, you would:
    // 1. Load a font with TTF_OpenFont
    // 2. Render text with TTF_RenderText_Solid
    // 3. Create texture from surface
    // 4. Render the texture
    
    // For now, we'll just print to console
    printf("TEXT: %s\n", text);
}

int handle_events() {
    SDL_Event e;
    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);
    
    // Calculate board dimensions
    int board_width = COLS * CELL_SIZE;
    int board_height = ROWS * CELL_SIZE;
    int board_x = (window_width - board_width) / 2;
    int board_y = (window_height - board_height) / 2;
    
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                return 0; // Exit game
                
            case SDL_MOUSEMOTION:
                // Calculate hover column
                if (e.motion.x >= board_x && e.motion.x < board_x + board_width &&
                    e.motion.y >= board_y && e.motion.y < board_y + board_height) {
                    hover_column = (e.motion.x - board_x) / CELL_SIZE;
                } else {
                    hover_column = -1;
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button == SDL_BUTTON_LEFT) {
                    // If game is over, restart
                    if (game_status != NO_WINNER) {
                        initialize_board(game_board);
                        game_status = NO_WINNER;
                        break;
                    }
                    
                    // Calculate clicked column
                    if (e.button.x >= board_x && e.button.x < board_x + board_width &&
                        e.button.y >= board_y && e.button.y < board_y + board_height) {
                        int column = (e.button.x - board_x) / CELL_SIZE;
                        
                        // Make player move
                        int row = drop_piece(game_board, column, PLAYER_SYMBOL);
                        if (row != -1) {
                            // Check for win
                            game_status = check_winner(game_board, PLAYER_SYMBOL);
                            
                            // If game continues, make CPU move
                            if (game_status == NO_WINNER) {
                                make_cpu_move(game_board);
                                game_status = check_winner(game_board, CPU_SYMBOL);
                            }
                        }
                    }
                }
                break;
                
            case SDL_KEYDOWN:
                // Allow ESC to exit
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    return 0; // Exit game
                }
                // Allow 1-7 keys to make moves
                if (e.key.keysym.sym >= SDLK_1 && e.key.keysym.sym <= SDLK_7) {
                    int column = e.key.keysym.sym - SDLK_1;
                    
                    // Make player move
                    int row = drop_piece(game_board, column, PLAYER_SYMBOL);
                    if (row != -1) {
                        // Check for win
                        game_status = check_winner(game_board, PLAYER_SYMBOL);
                        
                        // If game continues, make CPU move
                        if (game_status == NO_WINNER) {
                            make_cpu_move(game_board);
                            game_status = check_winner(game_board, CPU_SYMBOL);
                        }
                    }
                }
                break;
        }
    }
    
    return 1; // Continue game
}

void run_game_loop() {
    // Initialize game board
    initialize_board(game_board);
    game_status = NO_WINNER;
    
    // Main game loop
    int running = 1;
    while (running) {
        // Handle events
        running = handle_events();
        
        // Render the board
        render_board();
        
        // Small delay to prevent using 100% CPU
        SDL_Delay(16); // Roughly 60 FPS
    }
}

int main(int argc, char *argv[]) {
    // Initialize SDL
    if (!init_sdl()) {
        return 1;
    }
    
    // Run the game
    run_game_loop();
    
    // Clean up
    cleanup_sdl();
    
    return 0;
}