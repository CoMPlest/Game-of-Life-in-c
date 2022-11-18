//
// Created by szeke on 11/18/2022.
//

#ifndef MAIN_C_GAMEENGINE_H
#define MAIN_C_GAMEENGINE_H

#endif //MAIN_C_GAMEENGINE_H

#include "econio.h"
#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

typedef struct Screen {
    char* chars;
    int size;
    char* title;
    HANDLE console;
    DWORD bytesOnScreen;
    CONSOLE_CURSOR_INFO conCurInf;
} Screen;

void print2d(char* buff, int length, bool withLineBreak, int lineWidth);

void render2d(Screen* screen);

void hideCursor(Screen* screen);

void showCursor(Screen* screen);

Screen* createScreen(int size, char* title);

void destroyScreen(Screen* screen);

void activateScreen(Screen* screen);

void deactivateScreen(Screen* screen);