#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <stdbool.h>

#define BOARD_SIZE 24
#define MAX_CONNECTION 8

typedef enum {
    EMPTY = 0,
    RED = 1,
    BLACK = 2
} Player;

typedef enum Status {
    CONTINUE = 0,
    RED_WINS = 1,
    BLACK_WINS = 2,
    INVALID = 3
} Status;

typedef struct Position {
    unsigned int row;
    unsigned int col;
} Position;

typedef struct Connection {
    Position to;
    Position from;
    struct Connection *next;
} Connection;

typedef struct {
    Position pos;
    Player owner;
    Connection* connections;
    int connection_count;
} Peg;

typedef struct GameState {
    Player Board[BOARD_SIZE][BOARD_SIZE];
    Peg* pegs[BOARD_SIZE][BOARD_SIZE];
    Player current;
    Connection *connections[BOARD_SIZE][BOARD_SIZE][MAX_CONNECTION];
} GameState;

#endif  