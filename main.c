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
    econio_gotoxy(0, 0);

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

void hideCursor() {
    HANDLE hOut;
    CONSOLE_CURSOR_INFO ConCurInf;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    ConCurInf.dwSize = 10;
    ConCurInf.bVisible = FALSE;

    SetConsoleCursorInfo(hOut, &ConCurInf);
}

void showCursor() {
    HANDLE hOut;
    CONSOLE_CURSOR_INFO ConCurInf;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    ConCurInf.dwSize = 10;
    ConCurInf.bVisible = TRUE;

    SetConsoleCursorInfo(hOut, &ConCurInf);
}




int main() {
    //Some initializations
    srand(time(NULL));
    econio_set_title("Game of life");
    econio_rawmode();
    econio_clrscr();
    hideCursor();

    GameState* gameState = createNewState(120, 30);
    clearCells(gameState);


    //game loop
    while (!(econio_kbhit() && econio_getch() == KEY_ESCAPE)) {
        renderState(gameState);

        GameState* prev_state = gameState;
        gameState = calculateNextState(gameState);
        destroyGameState(prev_state);
    }

    destroyGameState(gameState);
    showCursor();
    //printf("\n\n"); //Helpful for debugging

    return 0;
}
