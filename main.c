#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conwayGame.h"

int main(int argc, char *argv[]) {
    //Settings
    char *title = "Game of life";
    int escapeKey = KEY_ESCAPE;
    int fps = 10;

    //Parameters
    int width;
    int height;
    char *loadFileName = NULL;
    char *savefileName = NULL;
    int iterationCount;
    bool dispGraphics;

    //Some initializations
    srand(time(NULL));

    GameState* gameState = createNewState(width, height);
    clearCells(gameState);

    if (!loadStateFromFile(loadFileName, gameState))
        randomizeCells(gameState);


    if (dispGraphics) {

        Screen* screen = createScreen(gameState->width*gameState->height, title);
        activateScreen(screen);

        //game loop
        //until a key is pressed and that key matches the escape key
        while (!(econio_kbhit() && econio_getch() == escapeKey)) {
            convertToChar(gameState, screen->chars);
            render2d(screen);
            stepGame(&gameState); //Room for improvement and optimization
            Sleep(1000/fps);
        }

        deactivateScreen(screen);
        destroyScreen(screen);
    } else {
        for(int i = 0; i < iterationCount; i++) {
            stepGame(&gameState);
        }
    }


    saveStateToFile(savefileName, gameState);

    destroyGameState(gameState);
    //printf("\n\n"); //Helpful for debugging

    return 0;
}
