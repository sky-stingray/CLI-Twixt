#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>

bool is_valid_position(GameState *game, int row, int col) {
    if(row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        printf("Invalid move: Co-ordinates outside board.\n");
        return false;
    }
    if(game->current == RED && (col == 0 || col == BOARD_SIZE-1)) {
        printf("Invalid move: Cannot place in BLUE's side.\n");
        return false;
    }
    if(game->current == BLACK && (row == 0 || row == BOARD_SIZE-1)) {
        printf("Invalid move: Cannot place in RED's side.\n");
        return false;
    }
    if(game->Board[row][col] != EMPTY) {
        printf("Invalid move: Already a peg present.\n");
        return false;
    }
    return true;
}

static int ccw(Position A, Position B, Position C) {
    return (C.row - A.row) * (B.col - A.col) - (B.row - A.row) * (C.col - A.col);
}

static int sgn(int val) {
    return (val > 0) - (val < 0);
}

bool connections_intersect(Position a1, Position a2, Position b1, Position b2) {
    int d1 = sgn(ccw(b1, b2, a1));
    int d2 = sgn(ccw(b1, b2, a2));
    int d3 = sgn(ccw(a1, a2, b1));
    int d4 = sgn(ccw(a1, a2, b2));

    if(!(d1 && d2 && d3 && d4)) {
        return false;
    }
    
    if(d1 != d2 && d3 != d4) {
        return true;
    }
    return false;
}

bool is_valid_connection(GameState* game, Position from, Position to) {
    if (game->Board[from.row][from.col] != game->Board[to.row][to.col]) {
        return false;
    }
    
    int dr = abs(from.row - to.row);
    int dc = abs(from.col - to.col);
    if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2))) {
        return false;
    }
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->pegs[i][j] != NULL) {
                Connection* conn = game->pegs[i][j]->connections;
                while (conn != NULL) {
                    if (connections_intersect(from, to, conn->from, conn->to)) {
                        printf("Intersection Found!\n");
                        if (!((from.row == conn->from.row && from.col == conn->from.col &&
                               to.row == conn->to.row && to.col == conn->to.col) ||
                              (from.row == conn->to.row && from.col == conn->to.col &&
                               to.row == conn->from.row && to.col == conn->from.col))) {
                            return false;
                        }
                    }
                    conn = conn->next;
                }
            }
        }
    }
    return true;
}

bool add_connection(GameState *game, Position from, Position to) {
    if (!is_valid_connection(game, from, to)) {
        return false;
    }
    
    printf("Add Connection is called\n");
    Connection* conn1 = (Connection*)malloc(sizeof(Connection));
    conn1->from = from;
    conn1->to = to;
    conn1->next = game->pegs[from.row][from.col]->connections;
    game->pegs[from.row][from.col]->connections = conn1;
    game->pegs[from.row][from.col]->connection_count++;

    Connection* conn2 = (Connection*)malloc(sizeof(Connection));
    conn2->from = to;
    conn2->to = from;
    conn2->next = game->pegs[to.row][to.col]->connections;
    game->pegs[to.row][to.col]->connections = conn2;
    game->pegs[to.row][to.col]->connection_count++;

    bool stored = false;
    for (int i = 0; i < MAX_CONNECTION; ++i) {
        if (game->connections[from.row][from.col][i] == NULL) {
            game->connections[from.row][from.col][i] = conn1;
            stored = true;
            break;
        }
    }
    for (int i = 0; i < MAX_CONNECTION; ++i) {
        if (game->connections[to.row][to.col][i] == NULL) {
            game->connections[to.row][to.col][i] = conn1;
            break;
        }
    }

    if (!stored) {
        free(conn1);
        return false;
    }
    return true;
}

bool place_peg(GameState *game, int row, int col) {
    if(is_valid_position(game, row, col)) {
        game->Board[row][col] = game->current;

        Peg* new_peg = (Peg*)malloc(sizeof(Peg));
        new_peg->pos.row = row;
        new_peg->pos.col = col;
        new_peg->owner = game->current;
        new_peg->connections = NULL;
        new_peg->connection_count = 0;
        game->pegs[row][col] = new_peg;

        int row_offset[] = {-2, -2, -1, -1, 1, 1, 2, 2};
        int col_offset[] = {-1, 1, -2, 2, -2, 2, -1, 1};

        for(int i = 0; i < 8; i++) {
            int row_pos = row + row_offset[i];
            int col_pos = col + col_offset[i];

            if (row_pos >= 0 && row_pos < BOARD_SIZE && col_pos >= 0 && col_pos < BOARD_SIZE) {
                if(game->Board[row_pos][col_pos] == game->current) {
                    Position from = {row, col};
                    Position to = {row_pos, col_pos};
                    add_connection(game, from, to);
                }
            }
        }

        game->current = (game->current == RED) ? BLACK : RED;
        return true;
    }
    return false;
}