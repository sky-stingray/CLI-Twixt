#include <stdio.h>
#include "game_types.h"
#include "board.h"
#include "game_logic.h"
#include "win_checker.h"
#include "input_handler.h"

int main() {
    GameState game;
    init_board(&game);
    
    Status status = CONTINUE;
    while (status == CONTINUE) {
        printf("\033[2J\033[H"); // clrscr();
        display_board(&game);
        get_player_move(&game);
        status = check_winner(&game);
    }

    display_board(&game);

    if (status == RED_WINS) {
        printf("\n RED WINS! \n");
    } else if (status == BLACK_WINS) {
        printf("\n BLUE WINS! \n");
    }

    return 0;
}