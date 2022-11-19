#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conwayGame.h"

typedef struct Settings {
    char *title;
    int escapeKey;
    int fps;
    int maxFileNameLength;
    char randomStartChar;
    char noSaveChar;
} Settings;

typedef struct Params {
    int width;
    int height;
    char *loadFileName;
    char *saveFileName;
    int iterationCount;
} Params;

void questionUser(Params* params, Settings* settings);
void parseArguments(int argc, char** argv, Params* params);


int main(int argc, char *argv[]) {
    //Settings
    Settings* settings = malloc(sizeof(Settings));
    settings->title = "Game of life";
    settings->escapeKey = KEY_ESCAPE;
    settings->fps = 10;
    settings->maxFileNameLength = 50;
    settings->randomStartChar = 'r';
    settings->noSaveChar = 'n';

    //Some initializations
    srand(time(NULL));
    Params* params = malloc(sizeof(Params));
    bool didQuestionUser = false;

    // If there are not enough arguments
    if (argc < 3) {
        questionUser(params, settings);
        didQuestionUser = true;
    } else parseArguments(argc, argv, params);

    //create game state
    GameState* gameState = createNewState(params->width, params->height);
    clearCells(gameState); //not essential, could remove it

    if (params->loadFileName != NULL && strcmp(params->loadFileName, &(settings->randomStartChar)) != 0)
        loadStateFromFile(params->loadFileName, gameState);
    else
        randomizeCells(gameState);

    if (params->iterationCount == 0) {
        Screen* screen = createScreen(gameState->width*gameState->height, settings->title);
        activateScreen(screen);

        //game loop
        //until a key is pressed and that key matches the escape key
        while (!(econio_kbhit() && econio_getch() == settings->escapeKey)) {
            convertToChar(gameState, screen->chars);
            render2d(screen);
            stepGame(&gameState); //Room for improvement and optimization
            Sleep(1000/settings->fps);
        }

        deactivateScreen(screen);
        destroyScreen(screen);

    } else {
        for (int i = 0; i < params->iterationCount; i++)
            stepGame(&gameState);
    }

    if (params->saveFileName != NULL && strcmp(params->saveFileName, &(settings->noSaveChar)) != 0)
        saveStateToFile(params->saveFileName, gameState);

    if (didQuestionUser) {
        free(params->loadFileName);
        free(params->saveFileName);
    }
    destroyGameState(gameState);
    free(params);
    free(settings);

    return 0;
}


void questionUser(Params* params, Settings* settings) {
    printf("Please specify the width of the game: ");
    scanf("%d", &(params->width));

    printf("Please specify the height of the game: ");
    scanf("%d", &(params->height));

    printf("Input file max %d chars (%c if random state): ", settings->maxFileNameLength, settings->randomStartChar);
    params->loadFileName = malloc(sizeof(char)*(settings->maxFileNameLength + 1));
    params->loadFileName[settings->maxFileNameLength] = '\0';
    scanf("%s", params->loadFileName);

    printf("Output file max %d chars (%c if no save): ", settings->maxFileNameLength, settings->noSaveChar);
    params->saveFileName = malloc(sizeof(char)*(settings->maxFileNameLength + 1));
    params->saveFileName[settings->maxFileNameLength] = '\0';
    scanf("%s", params->saveFileName);


    printf("How many iteration (0 if infinite with graphics)?: ");
    scanf("%d", &(params->iterationCount));
}

void parseArguments(int argc, char** argv, Params* params) {
    sscanf(argv[1], "%d", &(params->width));
    sscanf(argv[2], "%d", &(params->height));

    // Load filename args into fileName vars
    params->loadFileName = argc > 3 ? argv[3] : NULL;
    params->saveFileName = argc > 4 ? argv[4] : NULL;


    if (argc > 5)
        sscanf(argv[5], "%d", &(params->iterationCount));
    else
        params->iterationCount = 0;
}