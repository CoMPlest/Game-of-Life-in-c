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

int main(int argc, char *argv[]) {
    //Settings
    char *title = "Game of life";
    int escapeKey = KEY_ESCAPE;
    int width;
    int height;
    char *loadFileName;
    char *savefileName;
    int iterationCount = 0;

    //Some initializations
    srand(time(NULL));
    econio_set_title(title);

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

    FILE* fp;
    if (strcmp(loadFileName, "r")) {
        fp = fopen(loadFileName, "r");
            loadStateFromFile(fp, gameState);
        fclose(fp);
    } else
        randomizeCells(gameState);

    //game loop
    if (dispGraphics) {
        econio_rawmode();
        econio_clrscr();
        hideCursor();

        while (!(econio_kbhit() && econio_getch() == escapeKey)) {
            renderState(gameState);
            stepGame(&gameState);
        }
    } else {
        for (int i = 0; i < iterationCount; i++)
            stepGame(&gameState);
    }

    if (savefileName != "n") {
        fp = fopen(savefileName, "w");
            saveStateToFile(fp, gameState);
        fclose(fp);
    }

    destroyGameState(gameState);
    if (!dispGraphics)
        showCursor();
    //printf("\n\n"); //Helpful for debugging

    return 0;
}
