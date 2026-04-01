#include "input_handler.h"
#include "game_logic.h"
#include "board.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void get_player_move(GameState *game) {
    char input[100];
    int row, col;

    while(1)  {
        printf("\n%s's move :> ", get_player_symbol(game->current, true));
        if(fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }

        if(strncmp(input, "quit", 4) == 0) {
            exit(0);
        }

        if (sscanf(input, "%d %d", &row, &col) == 2) {
            if(place_peg(game, row, col))
                return;
            else continue;
        }
        else {
            printf("Invalid input format!\n");
        }
    }
}