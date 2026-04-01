#ifndef WIN_CHECKER_H
#define WIN_CHECKER_H

#include "game_types.h"

Status check_winner(GameState* game);


bool has_path_to_goal(GameState* game, Player player);

bool dfs_path(GameState* game, int row, int col, Player player, bool visited[BOARD_SIZE][BOARD_SIZE]);

#endif