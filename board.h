#ifndef BOARD_H
#define BOARD_H

#include "game_types.h"

void init_board(GameState *game);

void display_board(GameState* game);

char* get_player_symbol(Player p, bool colored);

bool get_if_color(int row, int col, GameState *game);

#endif