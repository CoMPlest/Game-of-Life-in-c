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

void renderStateWithBuff(GameState* game, char* buff) {
    econio_gotoxy(0, 0);

    for(int i = 0; i < game->width * game->height; i++) {

        //Print new line if at the end
        if (i % game->width == 0) {
            buff[i] = '\n';
        }

        //Print the appropriate character
        if (game->cells[i]) {
            buff[i] = 'X';
        } else {
            buff[i] = ' ';
        }
    }

    printf("%s", buff);
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
    int width = 120;
    int height = 30;
    char *loadFileName;
    char *savefileName;
    int iterationCount = 0;

    //Some initializations
    srand(time(NULL));
    econio_set_title(title);

    bool dispGraphics = true;

    GameState* gameState = createNewState(width, height);
    clearCells(gameState);
    randomizeCells(gameState);

    FILE* fp;
    if (loadFileName != NULL) {
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

        char* buffer = malloc(sizeof(char)*(gameState->width+1)*gameState->height);

        while (!(econio_kbhit() && econio_getch() == escapeKey)) {
            renderStateWithBuff(gameState, buffer);
            stepGame(&gameState);
        }
        free(buffer);
    }

    if (savefileName != NULL) {
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
