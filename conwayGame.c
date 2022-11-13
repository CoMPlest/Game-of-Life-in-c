//
// Created by szeke on 11/13/2022.
//

#include "conwayGame.h"
#include <stdlib.h>


int getCellIndex(Vector coords, int width) {
    return coords.x + coords.y*width;
}

GameState* createNewState(int width, int height) {
    GameState* game = malloc(sizeof(GameState));
    game->width = width;
    game->height = height;
    game->cells = malloc(sizeof(bool)*width*height);
    return game;
}

void destroyGameState(GameState* game) {
    free(game->cells);
    free(game);
}

void clearCells(GameState* game) {
    for(int i = 0; i < (game->height * game->width); i++) {
        game->cells[i] = 0;
    }
}
void randomizeCells(GameState* game) {
    bool* cells = game->cells;

    for(int i = 0; i < (game->height * game->width); i++) {
        cells[i] = rand() & 1;
    }
}

int countAliveNeighbours(Vector* pos, GameState* game) {
    int count = 0;
    int x = pos->x;
    int y = pos->y;

    Vector topLeft = {x-1, y-1};
    Vector top = {x, y-1};
    Vector topRight = {x+1, y-1};

    Vector left = {x-1, y};
    Vector right = {x+1, y};

    Vector bottomRight = {x-1, y+1};
    Vector bottom = {x, y+1};
    Vector bottomLeft = {x+1, y+1};

    Vector neighbours[8] = {topLeft, top, topRight, left, right, bottomRight, bottom, bottomLeft};

    for (int j = 0; j < 8; j++) {
        Vector coords = neighbours[j];
        if ((coords.x >= 0 && coords.x < game->width) &&
            (coords.y >= 0 && coords.y < game->height) &&
            (game->cells[getCellIndex(coords, game->width)]))
                count++;
    }

    return count;
}

GameState* calculateNextState(GameState* game) {
    GameState* next_state = createNewState(game->width, game->height);

    for(int i = 0; i < game->width * game->height; i++) {

        Vector currentCoords = {i % game->width, i / game->width};
        int aliveNeighbours = countAliveNeighbours(&currentCoords, game);

        if (aliveNeighbours == 2) {
            next_state->cells[i] = game->cells[i];
        } else if(aliveNeighbours == 3) {
            next_state->cells[i] = true;
        } else {
            next_state->cells[i] = false;
        }
    }
    return next_state;
}