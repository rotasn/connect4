# Connect Four Game

A classic Connect Four game implementation with both text-based and graphical interfaces. The game lets you play against a computer AI that uses various strategies to make its moves.

## Features

- Play Connect Four against the computer
- Both text-based and SDL graphical interfaces
- Computer AI with strategic moves
- Modular C code for easy maintenance and extension



### Prerequisites

To build the SDL version, you'll need:
- C compiler (gcc or clang)
- SDL2 library
- SDL2_ttf library

#### Installing Dependencies

On Windows:
- Download and install MinGW with SDL2 and SDL2_ttf libraries


### Text-based Version

- Follow the on-screen instructions
- Choose a column (0-6) to drop your piece
- Try to connect four of your pieces in a row, column, or diagonal before the computer does

### SDL Graphical Version

- Click on a column to drop your piece
- You can also use number keys 0-6 to select columns
- Press ESC to exit the game
- Click anywhere after a game ends to start a new game

## Game Controls

- **Mouse Click**: Drop a piece in the selected column
- **Number Keys (0-6)**: Drop a piece in the corresponding column
- **ESC**: Exit the game

## Code Structure

- `main.c`: Entry point for text-based game
- `sdl_ui.c`: SDL-based graphical interface
- `board.c/h`: Board representation and operations
- `game.c/h`: Game logic and win detection
- `ai.c/h`: Computer AI strategy

## Extending the Game

You can modify the game in various ways:
- Adjust board dimensions in `board.h`
- Add new AI strategies in `ai.c`
- Future ideas could include: multiplayer, difficulty levels, sound effects


## License

This project is released under the MIT License.

## Acknowledgments

- Original code by Nikolaos Rotas
- SDL2 library: https://www.libsdl.org/
- SDL2_ttf library: https://github.com/libsdl-org/SDL_ttf
