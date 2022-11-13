#include <stdio.h>
#include <time.h>
#include "conwayGame.h"
#include "econio.h"
#include <Windows.h>

/*
 * Prints the game state on the console
 * A bit slow
 *
 * TODO
 * improve behaviour with console
 */
void renderState(GameState* game) {
    econio_clrscr();

    for(int i = 0; i < game->width * game->height; i++) {

        //Print new line if at the end
        if (i % game->width == 0) {
            printf("\n");
        }

        //Print the appropriate character
        if (game->cells[i]) {
            printf("X");
        } else {
            printf(" ");
        }
    }
}



int main() {
    //Some initializations
    srand(time(NULL));
    econio_set_title("Game of life");
    econio_rawmode();

    GameState* gameState = createNewState(30, 30);

    //randomizeCells(gameState);
    clearCells(gameState);

    //Glider
    gameState->cells[70] = true;
    gameState->cells[100] = true;
    gameState->cells[130] = true;
    gameState->cells[129] = true;
    gameState->cells[98] = true;

    renderState(gameState);

    //game loop
    while (!(econio_kbhit() && econio_getch() == KEY_ESCAPE)) {
        GameState* prev_state = gameState;
        gameState = calculateNextState(gameState);
        destroyGameState(prev_state);

        renderState(gameState);
        Sleep(50);
    }

    destroyGameState(gameState);
    //printf("\n\n"); //Helpful for debugging

    return 0;
}
