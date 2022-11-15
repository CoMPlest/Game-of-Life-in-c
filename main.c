#include <stdio.h>
#include <time.h>
#include "conwayGame.h"
#include "econio.h"
#include <Windows.h>

/*
 * Prints the game state on the console
 * A bit slow
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
    //Settings
    bool loadFromFile = true;
    char *fileName = "C:\\Users\\szeke\\Documents\\Nagy_HZ\\game.txt";
    char *title = "Game of life";
    int escapeKey = KEY_ESCAPE;
    int width = 120;
    int height = 30;
    bool saveToFile = false;
    char *savefileName = "C:\\Users\\szeke\\Documents\\Nagy_HZ\\save.txt";

    //Some initializations
    srand(time(NULL));
    econio_set_title(title);
    econio_rawmode();
    econio_clrscr();
    hideCursor();
    FILE* fp;

    GameState* gameState = createNewState(width, height);
    clearCells(gameState);

    if (loadFromFile && fileName != NULL) {
        fp = fopen(fileName, "r");
            loadStateFromFile(fp, gameState);
        fclose(fp);
    } else
        randomizeCells(gameState);

    //game loop
    while (!(econio_kbhit() && econio_getch() == escapeKey)) {
        renderState(gameState);

        GameState* prev_state = gameState;
        gameState = calculateNextState(gameState);
        destroyGameState(prev_state);
    }

    if (saveToFile && savefileName != NULL) {
        fp = fopen(savefileName, "w");
            saveStateToFile(fp, gameState);
        fclose(fp);
    }

    destroyGameState(gameState);
    showCursor();
    //printf("\n\n"); //Helpful for debugging

    return 0;
}
