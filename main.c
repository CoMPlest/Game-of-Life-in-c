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
            putchar('\n');
        }

        //Print the appropriate character
        if (game->cells[i]) {
            putchar('X');
        } else {
            putchar(' ');
        }
    }
}

void renderStateWithBuff(GameState* game, char *buff, HANDLE hConsole, DWORD *dwBytesWritten ) {
    for(int i = 0; i < game->width * game->height; i++) {
        //Print the appropriate character
        if (game->cells[i]) {
            buff[i] = 'X';
        } else {
            buff[i] = ' ';
        }
    }

    WriteConsoleOutputCharacterA(hConsole, (LPCSTR) buff, game->width*game->height, (COORD) {0, 0 }, dwBytesWritten);
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

        char *screen = malloc(sizeof(char )*(gameState->width*gameState->height));
        HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(hConsole);
        DWORD dwBytesWritten = 0;

        while (!(econio_kbhit() && econio_getch() == escapeKey)) {
            renderStateWithBuff(gameState, screen, hConsole, &dwBytesWritten);
            stepGame(&gameState);
            Sleep(100);
        }
        free(screen);
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
