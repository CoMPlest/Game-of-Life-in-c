#include "conwayGame.h"
#include <stdlib.h>

/*
 * Returns the index of a cell based on a 2d vector
 */
int getCellIndex(Vector coords, int width) {
    return coords.x + coords.y*width;
}

/*
 * Handles the dynamic memory allocation for the GameState
 */
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

    // Vectors for the neighbours in the 3x3 square:
    // # # #
    // # - #
    // # # #
    //Top row
    Vector topLeft = {x-1, y-1};
    Vector top = {x, y-1};
    Vector topRight = {x+1, y-1};

    //Left and Right
    Vector left = {x-1, y};
    Vector right = {x+1, y};

    //Bottom row
    Vector bottomRight = {x-1, y+1};
    Vector bottom = {x, y+1};
    Vector bottomLeft = {x+1, y+1};

    //Array so we can use a loop to handle things
    //and for our code to be more compact
    Vector neighbours[8] = {
                            topLeft,     top,       topRight,
                            left,      /* curr cell */  right,
                            bottomRight, bottom,    bottomLeft
    };
    //Also in the future we could make this a separate function
    //to return Vectors for the neighbours for now we use it in one place only so it does not matter

    for (int j = 0; j < 8; j++) {
        Vector coords = neighbours[j];
        if ((coords.x >= 0 && coords.x < game->width) &&             // Check if the neighbour's coordinates are in bounds
            (coords.y >= 0 && coords.y < game->height) &&
            (game->cells[getCellIndex(coords, game->width)]))  //If cell is alive increase count
                count++;
    }

    return count;
}

GameState* calculateNextState(GameState* game) {
    GameState* next_state = createNewState(game->width, game->height);

    // Loop through the array of cells
    for(int i = 0; i < game->width * game->height; i++) {

        Vector currentCoords = {i % game->width, i / game->width}; // The current cell's coordinates x and y
        int aliveNeighboursCount = countAliveNeighbours(&currentCoords, game);

        if (aliveNeighboursCount == 2) {                     // If the cell has 2 alive neighbours it's sate remains the same
            next_state->cells[i] = game->cells[i];
        } else if(aliveNeighboursCount == 3) {               // If the cell has 3 alive neighbours it will be an alive cell
            next_state->cells[i] = true;
        } else {
            next_state->cells[i] = false;               // In every other scenario the cell becomes dead
        }
    }
    return next_state;
}

void loadStateFromFile(FILE* fp, GameState* game) {
    char current;
    int i = 0;
    while((current = fgetc(fp)) != EOF) {
        if (i == game->width * game->height)
            break;

        if (current == '\n')
            i = game->width * (i / game->width + 1);
        else
            game->cells[i++] = current == 'X';
    }
}

void saveStateToFile(FILE* fp, GameState* game) {
    for (int i = 0; i < game->width * game->height; i++) {
        if (i != 0 && i%game->width == 0)
            fputc('\n', fp);

        if (game->cells[i])
            fputc('X', fp);
        else
            fputc('-', fp);
    }
}