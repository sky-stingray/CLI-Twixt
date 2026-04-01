#include "board.h"
#include <stdio.h>

void init_board(GameState *game) {
    for(short int i = 0; i < BOARD_SIZE; ++i) {
        for(short int j = 0; j < BOARD_SIZE; ++j) {
            game->Board[i][j] = EMPTY;
            game->pegs[i][j] = NULL;
            for(short int connection = 0; connection < MAX_CONNECTION; ++connection) {
                game->connections[i][j][connection] = NULL;
            }
        }
    }
    game->current = RED;
}

char* get_player_symbol(Player p, bool colored) {
    if(p == RED && colored) return "\033[31;1mR\033[0m";
    if(p == BLACK && colored) return "\033[34;1mB\033[0m";
    if(p == RED) return "R";
    if(p == BLACK) return "B";
    return ".";
}

bool get_if_color(int row, int col, GameState *game) {
    if(game->connections[row][col][0] != NULL) {    
        return true;
    }
    return false;
}

void display_board(GameState* game) {
    printf("\n    ");
    printf(" 0  ");
    for (short unsigned i = 1; i < BOARD_SIZE-1; i++)
    {
        printf("%2d ", i);
    }
    printf(" %d", BOARD_SIZE - 1);
    printf("\n   +");
    for (short unsigned i = 0; i < BOARD_SIZE; i++)
    {
        printf("---");
    }
        printf("--");
    printf("+\n");
    
    for (short unsigned i = 0; i < BOARD_SIZE; i++)
    {
        printf("%2d |", i);
        for (short unsigned j = 0; j < BOARD_SIZE; j++)
        {   
            if((i == 0 && j == 0) || (i==BOARD_SIZE-1 && j==0)) {
                printf("   \033[34;1m|\033[0m");
                continue;
            }
            if((i==0 && j==BOARD_SIZE-1) || (i==BOARD_SIZE-1 && j==BOARD_SIZE-1)) {
                printf("\033[34;1m|\033[0m   ");
                continue;
            }
            bool color = get_if_color(i, j, game);
            if(j == 0)
                printf(" %s \033[34;1m|\033[0m", get_player_symbol(game->Board[i][j], color));
            else if (j==BOARD_SIZE-1) 
                printf("\033[34;1m|\033[0m %s ", get_player_symbol(game->Board[i][j], color));
            else 
                printf(" %s ", get_player_symbol(game->Board[i][j], color));
        }
        printf("|\n");
        printf("   |");
        if(i==0 || i == BOARD_SIZE-2) {
            for (short unsigned j = 0; j < BOARD_SIZE; j++)
            {
                printf("\033[31;1m---\033[0m");
            }
                printf("\033[31;1m--\033[0m");
        } else {
            for (short unsigned j = 0; j < BOARD_SIZE; j++)
            {
                if(j == 0)
                    printf("   \033[34;1m|\033[0m");
                else if (j==BOARD_SIZE-1)
                    printf("\033[34;1m|\033[0m   ");
                else
                    printf("   ");
            }
            
        }
        printf("|\n");
        
    }

    printf("   +");
    for(short unsigned i = 0; i < BOARD_SIZE; ++i) {
        printf("---");
    }
    printf("--");
    printf("+\n");
}