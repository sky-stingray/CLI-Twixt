#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "game_types.h"

bool is_valid_position(GameState *game, int row, int col);

bool connections_intersect(Position a1, Position a2, Position b1, Position b2);

bool is_valid_connection(GameState* game, Position from, Position to);

bool add_connection(GameState *game, Position from, Position to);

bool place_peg(GameState *game, int row, int col);

#endif