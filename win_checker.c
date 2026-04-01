#include "win_checker.h"
#include <stddef.h>
#include <stdbool.h>

Status check_winner(GameState* game) {
    if (has_path_to_goal(game, RED)) {
        return RED_WINS;
    }
    if (has_path_to_goal(game, BLACK)) {
        return BLACK_WINS;
    }
    return CONTINUE;
}

bool has_path_to_goal(GameState* game, Player player) {
    bool visited[BOARD_SIZE][BOARD_SIZE] = {false};
    
    if (player == RED) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (game->Board[0][col] == RED) {
                if (dfs_path(game, 0, col, RED, visited)) {
                    return true;
                }
            }
        }
    } else {
        for (int row = 0; row < BOARD_SIZE; row++) {
            if (game->Board[row][0] == BLACK) {
                if (dfs_path(game, row, 0, BLACK, visited)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool dfs_path(GameState* game, int row, int col, Player player, bool visited[BOARD_SIZE][BOARD_SIZE]) {
    if (visited[row][col]) {
        return false;
    }
    
    visited[row][col] = true;
    
    if (player == RED && row == BOARD_SIZE - 1) {
        return true;
    }
    if (player == BLACK && col == BOARD_SIZE - 1) {
        return true;
    }
    
    if (game->pegs[row][col] != NULL) {
        Connection* conn = game->pegs[row][col]->connections;
        while (conn != NULL) {
            int nr = conn->to.row;
            int nc = conn->to.col;
            if (dfs_path(game, nr, nc, player, visited)) {
                return true;
            }
            conn = conn->next;
        }
    }
    
    return false;
}