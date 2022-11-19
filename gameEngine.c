//
// Created by szeke on 11/18/2022.
//

#include "gameEngine.h"

void print2d(char* buff, int length, bool withLineBreak, int lineWidth) {
    econio_gotoxy(0, 0);

    for(int i = 0; i < length; i++) {
        //Print new line if at the end of the line
        if (withLineBreak && i % lineWidth == 0) {
            putchar('\n');
        }
        putchar(buff[i]);
    }
}

void render2d(Screen* screen) {
    WriteConsoleOutputCharacterA(
            screen->console,
            screen->chars,
            screen->size,
            (COORD) {0, 0 },
            &(screen->bytesOnScreen)
    );
}

void hideCursor(Screen* screen) {
    screen->conCurInf.dwSize = 10;
    screen->conCurInf.bVisible = FALSE;

    SetConsoleCursorInfo(screen->console, &(screen->conCurInf));
}

void showCursor(Screen* screen) {
    screen->conCurInf.dwSize = 10;
    screen->conCurInf.bVisible = TRUE;

    SetConsoleCursorInfo(screen->console, &(screen->conCurInf));
}

Screen* createScreen(int size, char* title) {
    Screen* screen = malloc(sizeof(Screen));
    screen->chars = malloc(sizeof(char)*size);
    screen->size = size;
    screen->bytesOnScreen = 0;
    screen->title = title;
    screen->console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    return screen;
}

void destroyScreen(Screen* screen) {
    free(screen->chars);
    free(screen);
}

void activateScreen(Screen* screen) {
    econio_set_title(screen->title);
    econio_rawmode();
    econio_clrscr();
    hideCursor(screen);
    SetConsoleActiveScreenBuffer(screen->console);
}

void deactivateScreen(Screen* screen) {
    econio_clrscr();
    showCursor(screen);
}


