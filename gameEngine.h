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

/**
 * Prints a char* on the console
 * A bit slow
 *
 * @param buff          - array containing the characters to print
 * @param length        - length of the buffer
 * @param withLineBreak - whether it should print with linebreaks
 * @param lineWidth     - the line width
 */
void print2d(char* buff, int length, bool withLineBreak, int lineWidth);

/**
 * Renders the char* of the Screen to the console of the Screen
 * using the win32 api calls making it much faster the the print2d function
 *
 * @param screen    - the screen to render
 */
void render2d(Screen* screen);

/**
 * Hides the cursor on a given Screen's console
 *
 * @param screen    - the Screen
 */
void hideCursor(Screen* screen);

/**
 * Shows the cursor on a given Screen's console
 *
 * @param screen    - the Screen
 */
void showCursor(Screen* screen);

/**
 * Handles the dynamic memory allocation when creating a Screen and also
 * allocates memory for the char*
 *
 * @param size      - size of the char* in the Screen
 * @param title     - title of the console
 * @return          - the Screen
 */
Screen* createScreen(int size, char* title);

/**
 * Destroys the Screen by freeing up its memory
 *
 * @param screen    - the Screen
 */
void destroyScreen(Screen* screen);

/**
 * Sets the active Screen and enables raw input mode, and also hides the cursor
 *
 * @param screen    - preferred active Screen
 */
void activateScreen(Screen* screen);

/**
 * Reverts changes made when calling activateScreen
 *
 * @param screen    - the Screen
 */
void deactivateScreen(Screen* screen);