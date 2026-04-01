CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = 

GAME_OBJS = main.o board.o game_logic.o win_checker.o input_handler.o

all: game

game: $(GAME_OBJS)
	$(CC) $(CFLAGS) -o game $(GAME_OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c game_types.h board.h game_logic.h win_checker.h input_handler.h
board.o: board.c board.h game_types.h
game_logic.o: game_logic.c game_logic.h game_types.h
win_checker.o: win_checker.c win_checker.h game_types.h
input_handler.o: input_handler.c input_handler.h game_types.h board.h game_logic.h

clean:
	rm -f *.o game


run_game: game
	./game

.PHONY: all clean run_tests run_game