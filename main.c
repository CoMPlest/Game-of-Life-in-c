#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conwayGame.h"

int main(int argc, char *argv[]) {
    //Settings
    char *title = "Game of life";
    int escapeKey = KEY_ESCAPE;
    int width = 120;
    int height = 30;
    char *loadFileName = NULL;
    char *savefileName = NULL;
    int iterationCount = 0;

    //Some initializations
    srand(time(NULL));

    GameState* gameState = createNewState(width, height);
    clearCells(gameState);

    if (!loadStateFromFile(loadFileName, gameState))
        randomizeCells(gameState);

    bool dispGraphics = true;
    if (dispGraphics) {

        Screen* screen = createScreen(gameState->width*gameState->height, title);
        activateScreen(screen);

        //game loop
        while (!(econio_kbhit() && econio_getch() == escapeKey)) {
            convertToChar(gameState, screen->chars);
            render2d(screen);
            stepGame(&gameState);
            Sleep(100);
        }

        deactivateScreen(screen);
        destroyScreen(screen);
    }


    saveStateToFile(savefileName, gameState);

    destroyGameState(gameState);
    //printf("\n\n"); //Helpful for debugging

    return 0;
}
