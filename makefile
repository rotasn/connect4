CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJS = board.o game.o ai.o

# Default target: Build the text-based game
all: connect4

# SDL target: Build the SDL-based game
sdl: CFLAGS += -DUSE_SDL `sdl2-config --cflags`
sdl: LIBS = `sdl2-config --libs` -lSDL2_ttf
sdl: connect4_sdl

# Text-based version
connect4: $(OBJS) main.o
	$(CC) -o $@ main.o $(OBJS) $(LIBS)

# SDL version
connect4_sdl: $(OBJS) sdl_ui.o
	$(CC) -o $@ sdl_ui.o $(OBJS) $(LIBS)

# Object files
main.o: main.c board.h game.h
	$(CC) $(CFLAGS) -c main.c

sdl_ui.o: sdl_ui.c board.h game.h ai.h
	$(CC) $(CFLAGS) -c sdl_ui.c

board.o: board.c board.h
	$(CC) $(CFLAGS) -c board.c

game.o: game.c game.h board.h ai.h
	$(CC) $(CFLAGS) -c game.c

ai.o: ai.c ai.h board.h
	$(CC) $(CFLAGS) -c ai.c

# Clean up
clean:
	rm -f *.o connect4 connect4_sdl

# Install SDL2 and SDL2_ttf (Ubuntu/Debian)
install-deps-debian:
	sudo apt-get update
	sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev

# Install SDL2 and SDL2_ttf (macOS with Homebrew)
install-deps-mac:
	brew update
	brew install sdl2 sdl2_ttf

.PHONY: all clean sdl install-deps-debian install-deps-mac