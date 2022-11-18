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

    if (argc < 3) {
        printf("Please specify the width of the game: ");
        scanf("%d", &width);

        printf("Please specify the height of the game: ");
        scanf("%d", &height);

        printf("How many iteration (0 if infinite with graphics)?: ");
        scanf("%d", &iterationCount);

        printf("Input file (r if random state): ");
        scanf("%s", &loadFileName);

        printf("Output file (n if no save): ");
        scanf("%s", &savefileName);
    } else {
        sscanf(argv[2], "%d", &width);
        sscanf(argv[3], "%d", &height);

        if (argc >= 4)
            sscanf(argv[4], "%d", &iterationCount);

        // Load filename args into fileName vars
        if (argc >= 5)
            loadFileName = &(argv[5]);

        if (argc >= 6)
            savefileName = &(argv[6]);
    }

    bool dispGraphics = iterationCount == 0;


    GameState* gameState = createNewState(width, height);
    clearCells(gameState);

    if (!loadStateFromFile(loadFileName, gameState))
        randomizeCells(gameState);

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
