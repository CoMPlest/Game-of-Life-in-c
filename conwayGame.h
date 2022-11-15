//
// Created by szeke on 11/13/2022.
//

#ifndef NAGY_HAZI_CONWAYGAME_H
#define NAGY_HAZI_CONWAYGAME_H

#endif //NAGY_HAZI_CONWAYGAME_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Vector {
    int x;
    int y;
} Vector;

typedef struct GameState {
    int width;
    int height;
    bool* cells;
} GameState;

int getCellIndex(Vector coords, int width);

GameState* createNewState(int width, int height);

void destroyGameState(GameState* state);

void clearCells(GameState* game);

void randomizeCells(GameState* game);

int countAliveNeighbours(Vector* pos, GameState* game);

GameState* calculateNextState(GameState* game);

void loadStateFromFile(FILE* fp, GameState* game);

void saveStateToFile(FILE* fp, GameState* game);
