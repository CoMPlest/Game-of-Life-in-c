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
    int iterationCount = 0;
    bool dispGraphics;

    //Some initializations
    srand(time(NULL));

    if (argc < 3) {
        printf("Please specify the width of the game: ");
        scanf("%d", &width);

        printf("Please specify the height of the game: ");
        scanf("%d", &height);

        printf("Input file max 50 chars (r if random state): ");
        loadFileName = malloc(sizeof(char)*51);
        loadFileName[50] = '\0';
        scanf("%s", loadFileName);

        printf("Output file max 50 chars (n if no save): ");
        savefileName = malloc(sizeof(char)*51);
        savefileName[50] = '\0';
        scanf("%s", savefileName);


        printf("How many iteration (0 if infinite with graphics)?: ");
        scanf("%d", &iterationCount);
    } else {
        sscanf(argv[1], "%d", &width);
        sscanf(argv[2], "%d", &height);

        // Load filename args into fileName vars
        if (argc > 3)
            loadFileName = argv[3];

        if (argc > 4)
            savefileName = argv[4];

        if (argc > 5)
            sscanf(argv[5], "%d", &iterationCount);
    }

    dispGraphics = iterationCount == 0;

    GameState* gameState = createNewState(width, height);
    clearCells(gameState);

    if (( loadFileName != NULL && !strcmp(loadFileName, "r") ) || !loadStateFromFile(loadFileName, gameState))
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

    if (savefileName != NULL && strcmp(savefileName, "n"))
        saveStateToFile(savefileName, gameState);

    if (argc < 3) {
        free(loadFileName);
        free(savefileName);
    }
    destroyGameState(gameState);
    //printf("\n\n"); //Helpful for debugging

    return 0;
}
